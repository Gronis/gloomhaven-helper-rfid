I first attempt at exposing python bindings for gloomhaven serilizer/deserilizer using swig.

# Build

Make sure to have `swig` and other common build tools like `make` and a `c++` compiler.
Also requires `python3` to be installed. `python2.7` or less is unsupported.

```bash
make clean # Makefile needs more work. Make sure to have a clean build
make python
```

# Usage

In the `out` directory, there will two files after building, `ghr.py` and `_ghr.so`.
Go to this directory (or copy these files) and launch `python3`

Right now, only the model is exposed. Serializing and Deserializing is unavailable at this moment.


Example usage:
```python
import ghr

s = ghr.GameState()

ghr._print(s)

s.round = 3
s.dark = ghr.ElementState.Strong
```

# TODO:

* Using `tl::optional` does not work very well with swig. Trying to find out how to fix. Right now, all members which are wrapped inside a `tl::optional` might be inaccessible from the python interface.
* Expose a serilize and deserilize interface.
* Verify and possibly implement additional support, if necessary, for `v8.2` `8.3` and `8.3.1` of Gloomhaven Helper.
