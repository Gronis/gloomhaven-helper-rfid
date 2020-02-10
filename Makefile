CC=gcc
CPP=g++
CFLAGS:=`python3-config --cflags`
LDFLAGS:=`python3-config --ldflags`
FLAGS:=--std=c++11
OUTDIR:=out/ghh
INCLUDE:=include
SOURCE:=src
WRAPPER_DIR:=swig
PYTHON_DIR:=python

PYTHON_SO:= \
	_ghh.so \
	protocol/_v7_6.so \
	protocol/_v8_0.so \

OBJS:= \
	protocol/v7_6/protocol.o \
	protocol/v8_0/protocol.o \
	protocol/deserializer.o \
	protocol/serializer.o \

.PHONY: python test clean

python: $(OUTDIR) $(addprefix $(OUTDIR)/, $(PYTHON_SO) __init__.py)

main: $(OUTDIR) $(OUTDIR)/main
test: $(OUTDIR) $(OUTDIR)/test

clean:
	rm -rf $(OUTDIR)

$(OUTDIR)/main: $(addprefix $(OUTDIR)/, $(OBJS) main.o)
	$(CPP)  $(FLAGS) -I$(INCLUDE) -o $@ $^

$(OUTDIR)/test: $(addprefix $(OUTDIR)/, $(OBJS) test.o)
	$(CPP)  $(FLAGS) -I$(INCLUDE) -o $@ $^

$(OUTDIR):
	cp -r $(PYTHON_DIR) $(OUTDIR)

# $(OUTDIR)/__init__.py: $(PYTHON_DIR)/__init__.py
# 	cp $(PYTHON_DIR)/__init__.py $(OUTDIR)/__init__.py

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
