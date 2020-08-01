Gloomhaven Helper Server Client
=

This project provides an open source alternative to talk to the application "Gloomhaven Helper".

A simple example would be to run a headless server (no GUI) and connect to it with your clients. The purpose of this would be to have a small inexpensive always-on computer with your gloomhaven game state (like a raspberry pi). With port-forwarding enabled in your router, your friends can connect over the internet and you don't have to forward internet traffic to your personal computer.

Another use-case is to connect as a client and hook it up to a bounch of hardware that changes depending on the game state.

The original purpose for this project was to connect an inexpensive wifi-enabled micro-controller to Gloomhaven Helper app and scan rfid/nfc tags to interact with the game state (like setting initiative or adding/removing characters)

###Installation

Requires python 3.7 or higher

For an x86 (intel/amd cpu) running linux or macos, install with pip (3):

`pip install gloomhavenhelper`

For arm64/aarch64 systems, install with:

`pip install https://github.com/Gronis/gloomhaven-helper-rfid/releases/download/0.1.1/gloomhavenhelper-0.1.1-py3-none-linux_aarch64.whl`

for older arm versions, open an issue and I can see if I can provide a build for those systems as well.

Windows is not supported or tested. If you have a windows machine and want to host a server, I recommend just to use the original Gloomhaven Helper java application as a server.

###Usage:

Use as a sever

`python -m gloomhavenhelper`

Use as a client

```python
#!/usr/bin/env python

import glommhavenhelper as ghh
import asyncio

TCP_IP = '127.0.0.1' # server ip
TCP_PORT = 58888     # server port


async def on_game_state(message_number, game_state):
    # Do things with the game state.
    print("Got game state {}, round {}".format(message_number, game_state.round))
    # To understand what you can get out of the game state
    # use ghh.print(game_state)


async def main():
    client = ghh.Client(TCP_IP, TCP_PORT)
    client.on_game_state = on_game_state
    await client.connect()
    print("Connected")

asyncio.get_event_loop().create_task(main())
asyncio.get_event_loop().run_forever()

```

###Example of projects:
- [Video](https://seafile.robingronberg.se/f/2d55fbea824c434eb6b7/) of scanning a tag/card and changing initiative




License: MIT

Contributers

    - Robin Grönberg
