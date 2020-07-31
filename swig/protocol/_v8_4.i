%module v8_4
%feature("autodoc", "3");

%{
#include "protocol/v8_4/protocol.hpp"
using namespace ghh;
using namespace ghh::protocol;
%}
%include "protocol/v8_4/protocol.hpp"
