%module v8_0
%feature("autodoc", "3");

%{
#include "protocol/v8_0/protocol.hpp"
using namespace ghh;
using namespace ghh::protocol;
%}
%include "protocol/v8_0/protocol.hpp"