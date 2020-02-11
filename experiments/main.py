#!/usr/bin/env python

import ghh
import asyncio

TCP_IP = '127.0.0.1'
TCP_PORT = 58888


async def on_game_state(message_number, game_state):
    print("Got game state {}, round {}".format(message_number, game_state.round))


async def main():
    client = ghh.Client(TCP_IP, TCP_PORT)
    client.on_game_state = on_game_state
    await client.connect()
    print("Connected")

asyncio.get_event_loop().create_task(main())
asyncio.get_event_loop().run_forever()
