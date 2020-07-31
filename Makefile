CC=gcc
CPP=g++
CFLAGS:=`python3-config --cflags`
LDFLAGS:=`python3-config --ldflags`
FLAGS:=--std=c++11
OUTDIR:=out/ghh
DIST:=dist
BUILD:=build
INCLUDE:=include
SOURCE:=src
WRAPPER_DIR:=swig
PYTHON_DIR:=python
ARCH:=`uname -m`

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	PLATFORM:=manylinux1
endif
ifeq ($(UNAME_S),Darwin)
	PLATFORM:=macosx_10_12
endif
# See https://stackoverflow.com/questions/714100/os-detecting-makefile

PYTHON_SO:= \
	_ghh.so \
	protocol/_v7_6.so \
	protocol/_v8_0.so \
	protocol/_v8_4.so \

OBJS:= \
	protocol/v7_6/protocol.o \
	protocol/v8_0/protocol.o \
	protocol/v8_4/protocol.o \
	protocol/deserializer.o \
	protocol/serializer.o \
	protocol/header.o \

.PHONY: python test clean wheel upload

wheel:
	# echo ${PLATFORM}
	make python
	python3 setup.py bdist_wheel -p $(PLATFORM)_$(ARCH)

upload:
	make wheel
	twine upload dist/*

python: $(OUTDIR) $(addprefix $(OUTDIR)/, $(PYTHON_SO) __init__.py __main__.py)

main: $(OUTDIR) $(OUTDIR)/main
test: $(OUTDIR) $(OUTDIR)/test

clean:
	rm -rf $(OUTDIR)
	rm -rf $(DIST)
	rm -rf $(BUILD)

$(OUTDIR)/main: $(addprefix $(OUTDIR)/, $(OBJS) main.o)
	$(CPP)  $(FLAGS) -I$(INCLUDE) -o $@ $^

$(OUTDIR)/test: $(addprefix $(OUTDIR)/, $(OBJS) test.o)
	$(CPP)  $(FLAGS) -I$(INCLUDE) -o $@ $^

$(OUTDIR):
	mkdir -p $(dir $@)
	cp -r $(PYTHON_DIR) $(OUTDIR)

$(OUTDIR)/%.py: $(PYTHON_DIR)/%.py
	cp $< $@

$(OUTDIR)/%.py: $(WRAPPER_DIR)/%.i
	swig -python -c++ -I$(INCLUDE) $<

$(OUTDIR)/%_wrap.cxx: $(WRAPPER_DIR)/%.i
	swig -python -c++ -I$(INCLUDE) -o $@ $<

$(OUTDIR)/%.so: $(addprefix $(OUTDIR)/, $(OBJS) %_wrap.o)
	$(CPP) $(FLAGS) $(LDFLAGS) -I$(INCLUDE) -shared -o $@ $^

$(OUTDIR)/%.o: $(SOURCE)/%.cpp
	mkdir -p $(dir $@)
	$(CPP) $(FLAGS) -c -fPIC -I$(INCLUDE) -o $@ $<

$(OUTDIR)/%_wrap.o: $(OUTDIR)/%_wrap.cxx
	$(CPP) $(FLAGS) -c -fPIC -I$(INCLUDE) $(CFLAGS) -o $@ $<
