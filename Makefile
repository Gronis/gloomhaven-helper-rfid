
SWIG_TEMPLATE="%module $(notdir $(basename $@)) %feature(\"autodoc\", \"3\"); %{ \#define SWIG_PYTHON_EXTRA_NATIVE_CONTAINERS;  \#include \"$(notdir $(basename $@)).hpp\" %} %include \"$(notdir $(basename $@)).hpp\""
CC=clang
CPP=clang++
CFLAGS:=`python3-config --cflags`
LDFLAGS:=`python3-config --ldflags`
FLAGS:=-std=c++11
OUTDIR:=out
INCLUDE:=include
SOURCE:=src
WRAPPER_DIR:=swig

.PHONY: python clean

python: $(OUTDIR) $(OUTDIR)/_ghr.so $(OUTDIR)/ghr.py

clean:
	rm -r $(OUTDIR)

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(OUTDIR)/%.py: $(WRAPPER_DIR)/%.i
	swig -python -c++ -I$(INCLUDE) $<

$(OUTDIR)/%_wrap.cxx: $(WRAPPER_DIR)/%.i
	swig -python -c++ -I$(INCLUDE) -o $@ $<


$(OUTDIR)/_%.so: $(addprefix $(OUTDIR)/, %_wrap.o)
	$(CPP) $(FLAGS) $(LDFLAGS) -I$(INCLUDE) -shared -o $@ $^

$(OUTDIR)/%.o: $(SOURCE)/%.cpp
	$(CPP) $(FLAGS) -c -fPIC -I$(INCLUDE) $(CFLAGS) -o $@ $<

$(OUTDIR)/%_wrap.o: $(OUTDIR)/%_wrap.cxx
	$(CPP) $(FLAGS) -c -fPIC -I$(INCLUDE) $(CFLAGS) -o $@ $<

$(OUTDIR)/%.i:
	@echo $(SWIG_TEMPLATE) > $@
