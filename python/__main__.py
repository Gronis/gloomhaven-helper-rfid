import asyncio
from . import ghh
from . import Server


async def on_game_state(message_nr, game_state):
    print("Got game state", message_nr)


async def start_server():
    server = Server()
    server.on_game_state = on_game_state
    await server.serve_forever('0.0.0.0', 58888)
    print("Exiting")

if __name__ == "__main__":
    asyncio.run(start_server())
