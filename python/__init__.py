import socket
import asyncio

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


class Client:
    def __init__(self, host, port):
        self.__host = host
        self.__port = port
        self.__sock = None

    async def connect(self):
        connected = False
        while not connected:
            try:
                self.__sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.__sock.connect((self.__host, self.__port))
                connected = True
            except ConnectionRefusedError:
                await asyncio.sleep(0.2)
        asyncio.create_task(self.__read_loop())

    async def on_game_state(self, message_nr, game_state):
        pass

    async def __read_loop(self):
        receive_buffer = ghh.Buffer()
        header = ghh.Header()
        game_state = ghh.GameState()
        game_state_buffer = ghh.Buffer()

        def read():
            data = self.__sock.recv(1024)
            if len(data) > 0:
                return data
            return None

        def read_game_state(a, b): return None

        def write_game_state(a, b): return None

        read_game_state_versions = {
            '7.6': protocol.v7_6.readGameState,
            '8.0': protocol.v8_0.readGameState,
            '8.1': protocol.v8_0.readGameState,
        }

        write_game_state_versions = {
            '7.6': protocol.v7_6.writeGameState,
            '8.0': protocol.v8_0.writeGameState,
            '8.1': protocol.v8_0.writeGameState,
        }

        has_header = False
        need_more_data = False
        is_connected = True
        while is_connected:
            await asyncio.sleep(0)
            need_more_data = False
            data = read()
            is_connected = data is not None
            if is_connected:
                for c in data:
                    receive_buffer.writeByte(c)
            while not need_more_data:
                if not has_header:
                    has_header = ghh.readHeader(header, receive_buffer)
                    need_more_data = not has_header
                print("event: ", header.event, ",",
                      "payload: ", header.payload, ",",
                      "length: ", header.length)
                if header.event == 'v':
                    version = header.payload
                    read_game_state = read_game_state_versions.get(version, None)
                    write_game_state = write_game_state_versions.get(version, None)
                    version_is_supported = read_game_state is not None
                    if version_is_supported:
                        print("Using supported version {}".format(version))
                    else:
                        print("Unsupported version {}".format(version))
                        exit(1)
                    has_header = False
                if header.event == 's':
                    if header.length <= receive_buffer.getSize():
                        game_state_buffer.clear()
                        for _ in range(header.length):
                            game_state_buffer.writeByte(receive_buffer.readByte())
                        message_nr = game_state_buffer.readFullInt()
                        read_game_state(game_state, game_state_buffer)
                        await self.on_game_state(message_nr, game_state)
                        receive_buffer.flush()
                        has_header = False
                    else:
                        need_more_data = True


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
                    print("self {}, replaced {}, {}".format(__c, __n, __p.__doc__))
                    setattr(__c, __n, __remove_optional_from_property(__p, __opt_class))
                else:
                    print("Warning, {} does not have a make_optional function.".format(__p.__doc__))


__init__()
#print = ghh._print
