#!/usr/bin/env python3

import os
import asyncio
import threading
import time

from . import ghh
from . import protocol

Actor = ghh.Actor
AttackModifier = ghh.AttackModifier
CharacterClass = ghh.CharacterClass
Condition = ghh.Condition
ElementState = ghh.ElementState
GameState = ghh.GameState
MonsterAbilityDeck = ghh.MonsterAbilityDeck
MonsterActor = ghh.MonsterActor
MonsterInstance = ghh.MonsterInstance
MonsterType = ghh.MonsterType
PlayerActor = ghh.PlayerActor
PlayerInit = ghh.PlayerInit
SummonColor = ghh.SummonColor

Buffer = ghh.Buffer
Header = ghh.Header

readHeader = ghh.readHeader
writeHeader = ghh.writeHeader


_game_state_reader_versions = {
    '7.6': protocol.v7_6.readGameState,
    '8.0': protocol.v8_0.readGameState,
    '8.1': protocol.v8_0.readGameState,
    '8.2': protocol.v8_0.readGameState,
    '8.3': protocol.v8_0.readGameState,
    '8.4': protocol.v8_4.readGameState,
}


_game_state_writer_versions = {
    '7.6': protocol.v7_6.writeGameState,
    '8.0': protocol.v8_0.writeGameState,
    '8.1': protocol.v8_0.writeGameState,
    '8.2': protocol.v8_0.writeGameState,
    '8.3': protocol.v8_0.writeGameState,
    '8.4': protocol.v8_4.writeGameState,
}


def is_version_supported(version):
    return version in _game_state_reader_versions.keys()


def read_game_state_message(version, data):
    '''
    Read the game state from a byte array or ghh.Buffer object
    Provide the version, to use when parsing and the byte array/ghh.Buffer object
    '''
    def error(a, b):
        raise ValueError("Cannot read game state with version {}".format(version))
    if is_version_supported(version):
        game_state_buffer = ghh.Buffer()
        game_state = ghh.GameState()
        if isinstance(data, bytes):
            for c in data:
                game_state_buffer.writeByte(c)
        elif isinstance(data, ghh.Buffer):
            while(data.getSize() > 0):
                game_state_buffer.writeByte(data.readByte())
        message_nr = game_state_buffer.readFullInt()
        read = _game_state_reader_versions.get(version, error)
        read(game_state, game_state_buffer)
        return message_nr, game_state
    return None, None


def write_game_state_message(version, message_nr, game_state):
    '''
    Write the game state into a bytes array.
    Provide the version, which message number to use, and the game state.
    '''
    if is_version_supported(version):
        game_state_buffer = ghh.Buffer()
        game_state_buffer.writeFullInt(message_nr)
        write = _game_state_writer_versions.get(version)
        write(game_state, game_state_buffer)
        game_state_buffer.writeByte(False)  # Avoid buffer overflow in ghh java app.
        size = game_state_buffer.getSize()
        data = bytes([game_state_buffer.readByte() for _ in range(size)])
        return data


def write_game_state_header(data):
    '''
    Write the game state header to a new bytes array.
    Provid the serialized game state data from write_game_state_message
    '''
    write_buffer = ghh.Buffer()
    header = ghh.Header()
    header.event = "s"
    header.length = len(data)
    writeHeader(header, write_buffer)
    size = write_buffer.getSize()
    return bytes([write_buffer.readByte() for _ in range(size)]) + data


def write_version_header(version):
    '''
    Write the version header to a new bytes array.
    Provide the version.
    '''
    write_buffer = ghh.Buffer()
    header = ghh.Header()
    header.event = "v"
    header.payload = version
    writeHeader(header, write_buffer)
    size = write_buffer.getSize()
    return bytes([write_buffer.readByte() for _ in range(size)])


async def read_network_loop(reader, callbacks, default_version=None):
    receive_buffer = ghh.Buffer()
    header = ghh.Header()
    version = default_version
    has_header = False
    done = False

    async def read_network_data_and_put_in_buffer():
        data = await reader.read(1024)
        done = len(data) <= 0
        if not done:
            receive_buffer.flush()
            for c in data:
                receive_buffer.writeByte(c)
        return done

    async def no_callback(*args):
        pass

    while not done:
        await asyncio.sleep(0)
        done = await read_network_data_and_put_in_buffer()
        while True:
            has_header = readHeader(header, receive_buffer)
            if not has_header:
                break
            while header.length > receive_buffer.getSize():
                done = await read_network_data_and_put_in_buffer()

            if header.event == 'v':
                version = header.payload
                await callbacks.get('v', no_callback)(version)
            if header.event == 's':
                res = read_game_state_message(version, receive_buffer)
                await callbacks.get('s', no_callback)(*res)


class Client:
    def __init__(self, host, port):
        self.__host = host
        self.__port = port

    async def connect(self):
        connected = False
        callbacks = {
            'v': self.on_version,
            's': self.on_game_state,
        }
        while not connected:
            try:
                reader, writer = await asyncio.open_connection(self.__host, self.__port)
                connected = True
                await read_network_loop(reader, callbacks)
            except ConnectionRefusedError:
                await asyncio.sleep(0.2)

    async def on_game_state(self, message_nr, game_state):
        pass

    async def on_version(self, version):
        pass


class Server:
    def __init__(self):
        self.__needs_saving = False
        self.__message_nr = 0
        self.__clients = []
        self.__version = "8.4"
        self.__game_state = ghh.GameState()
        self.__path = "save.bin"

        self.__load_game_state_from_file()

        def save_thread():
            while True:
                self.__save_game_state_to_file()
                time.sleep(20)

        save_thread = threading.Thread(target=save_thread)
        save_thread.daemon = True
        save_thread.start()

    async def serve_handler(self, reader, writer):

        def make_version_message():
            msg = write_version_header(self.__version)
            return msg

        def make_game_state_message():
            body = write_game_state_message(self.__version, self.__message_nr, self.__game_state)
            msg = write_game_state_header(body)
            return msg

        async def send(msg, writer):
            writer.write(msg)
            await writer.drain()

        async def on_game_state(message_nr, game_state):
            if message_nr == self.__message_nr + 1 or self.__message_nr == 0:
                self.__message_nr = message_nr
                self.__game_state = game_state
                msg = make_game_state_message()
                for client_writer in self.__clients:
                    if writer is not client_writer:
                        await send(msg, client_writer)
                # Notify user injected code that the game state was updated.
                await self.on_game_state(message_nr, game_state)
                self.__needs_saving = True
            else:
                # Client is pushing old game state. Send the current game state
                # back to the client
                msg = make_game_state_message()
                await send(msg, writer)

        callbacks = {
            'v': self.on_version,
            's': on_game_state,
        }

        self.__clients.append(writer)
        _print("Client Connected")
        await send(make_version_message(), writer)
        await send(make_game_state_message(), writer)
        await read_network_loop(reader, callbacks, default_version=self.__version)
        _print("Client Disconnected")
        self.__clients.remove(writer)

    async def serve_forever(self, host, port):
        server = await asyncio.start_server(self.serve_handler, host, port)
        async with server:
            await server.serve_forever()

    async def on_game_state(self, message_nr, game_state):
        pass

    async def on_version(self, version):
        pass

    def __save_game_state_to_file(self):
        if self.__needs_saving:
            message_nr = self.__message_nr
            version = self.__version
            game_state = self.__game_state
            path = self.__path
            v_msg = write_version_header(version)
            body = write_game_state_message(version, message_nr, game_state)
            s_msg = write_game_state_header(body)

            with open(path, "wb") as f:
                f.write(v_msg)
                f.write(s_msg)

            self.__needs_saving = False

    def __load_game_state_from_file(self):
        path = self.__path
        if not os.path.exists(path):
            return
        with open(path, "rb") as f:
            data = f.read()
        file_buffer = ghh.Buffer()
        for c in data:
            file_buffer.writeByte(c)
        header = ghh.Header()
        ok = readHeader(header, file_buffer)
        if not ok or header.event != 'v':
            return
        version = header.payload
        ok = readHeader(header, file_buffer)
        if not ok or header.event != 's':
            return
        self.__message_nr, self.__game_state = read_game_state_message(version, file_buffer)


def __init__():
    import inspect

    def __remove_optional_from_property(old_property, make_optional):
        def new_set(self, v):
            old_property.fset(self, make_optional(v))

        def new_get(self):
            v = old_property.fget(self)
            if v.has_value():
                return v.value()
            else:
                return None
        return property(new_get, new_set)

    # Find every property which is using tl::optional and replace them
    __classes = [__c for __c in ghh.__dict__.values() if inspect.isclass(__c)]
    for __c in __classes:
        __properties = {(__n, __p)
                        for (__n, __p) in __c.__dict__.items() if isinstance(__p, property)}
        for __n, __p in __properties:
            if __p.__doc__.find("tl::optional") > 0:
                __opt_name = "Optional" + __p.__doc__.split("tl::optional")[-1].replace(
                    "(", "").replace(")", "").replace("<", "").replace(">", "").split("::")[-1]
                __opt_class = ghh.__dict__.get(__opt_name, None)
                if __opt_class:
                    # print("self {}, replaced {}, {}".format(__c, __n, __p.__doc__))
                    setattr(__c, __n, __remove_optional_from_property(__p, __opt_class))
                else:
                    print("Warning, {} does not have a make_optional function.".format(__p.__doc__))


__init__()
_print = print
print = ghh._print
