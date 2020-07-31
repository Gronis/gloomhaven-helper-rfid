I first attempt at exposing python bindings for gloomhaven serilizer/deserilizer using swig.

# Build

Make sure to have `swig` and other common build tools like `make` and a `c++` compiler.
Also requires `python3` to be installed. `python2.7` or less is unsupported.

```bash
make clean # Makefile needs more work. Make sure to have a clean build
make python
```

# Usage

In the `out` directory, there will be a folder called, `ghh` which is the python library.
Go to this directory (or copy these files) and launch `python3`

Right now, only the model and a client that can receive game state is implemented.


Tested with Gloomhaven Helper versions:
- `7.6`
- `8.0`
- `8.1`
- `8.2.1`
- `8.3.0`
- `8.3.1`


Example usage:
```python
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
```

# TODO:

* Add send game state functionality
