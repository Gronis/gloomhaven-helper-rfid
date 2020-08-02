Gloomhaven Helper Server Client
=

Supports Gloomhaven Helper with network sync up to version `8.4.x`

This project provides an open source alternative to talk to the application ["Gloomhaven Helper"](http://esotericsoftware.com/gloomhaven-helper).

A simple example would be to run a headless server (no GUI) and connect to it with your clients. The purpose of this would be to have a small inexpensive always-on computer with your gloomhaven game state (like a raspberry pi). With port-forwarding enabled in your router, your friends can connect over the internet and you don't have to forward internet traffic to your personal computer.

Another use-case is to connect as a client and hook it up to a bounch of hardware that changes depending on the game state.

The original purpose for this project was to connect an inexpensive wifi-enabled micro-controller to Gloomhaven Helper app and scan rfid/nfc tags to interact with the game state (like setting initiative or adding/removing characters). Because of this, the core protocol is implemented in c++ and wrapped in swig for python bindings. If you want to use this project with c++ directly, look at the `c++` codebase and the `Makefile` to figure out how to use it.

### Installation

Requires python 3.7 or higher

For an x86 (intel/amd cpu) running linux or macos, install with pip (3):

`pip install gloomhavenhelper`

For arm64/aarch64 systems, install with:

`pip install https://github.com/Gronis/gloomhaven-helper-rfid/releases/download/v0.1.3/gloomhavenhelper-0.1.3-py3-none-linux_aarch64.whl`

for older arm versions, open an issue and I can see if I can provide a build for those systems as well.

Windows is not supported or tested. If you have a windows machine and want to host a server, I recommend just to use the original Gloomhaven Helper java application as a server.

### Usage:

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
    # To understand what you can get out of the game state print it out
    ghh.print(game_state)


async def main():
    client = ghh.Client(TCP_IP, TCP_PORT)
    client.on_game_state = on_game_state
    await client.connect()
    print("Connected")

asyncio.get_event_loop().create_task(main())
asyncio.get_event_loop().run_forever()

```

### Example of projects:
- [Video](https://seafile.robingronberg.se/f/2d55fbea824c434eb6b7/) of scanning a tag/card and changing initiative


### Build

This project requires
- `g++/clang++` or another c++ compiler
- `python3`
- `pip`
- `swig`
- `wheels` (build wheels binary)
- `setuptools` (wheels binary)
- `twine` (upload wheels binary to pypi repository)

#### Install build tools

Make sure the required tools are installed:

```bash
apt update
apt install build-essential swig python3-dev python3-pip
```

#### How to build

Run `make python` to build python code. The python module is placed in a `out` directory. Change to this directory and `import gloomhavenhelper` to try out your new build: `cd out && python`.

Run `make wheel` to build wheel library. The build will be located in `dist` folder. You can then install it with `pip install dist/gloomhavenhelper-<some-more-stuff-here>.whl`

If you want to clean up the workspace because your build is failing because of some old cached file, or you just want to clean up the workspace, use `make clean`.

License: MIT

Contributers

    - Robin Grönberg
