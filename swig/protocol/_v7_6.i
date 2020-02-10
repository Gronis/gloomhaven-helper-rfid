%module v7_6
%feature("autodoc", "3");

%{
#include "protocol/v7_6/protocol.hpp"
using namespace ghh;
using namespace ghh::protocol;
%}
%include "protocol/v7_6/protocol.hpp"