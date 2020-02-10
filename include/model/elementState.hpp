#ifndef __ELEMENT_STATE_H__
#define __ELEMENT_STATE_H__

#include <vector>

#include "print.hpp"

namespace ghh
{
struct ElementStateNS
{
    enum Value{
        Inert,
        Strong,
        Waning,
    };
};

using ElementState = ElementStateNS::Value;

inline std::vector<ElementState> getElementStateValues()
{
    return {
        ElementState::Inert,
        ElementState::Strong,
        ElementState::Waning};
}

inline void print(const ElementState &arg)
{
    switch (arg)
    {
    case ElementState::Inert:
        ghh::print("Inert");
        break;
    case ElementState::Waning:
        ghh::print("Waning");
        break;
    case ElementState::Strong:
        ghh::print("Strong");
        break;
    }
}

} // namespace ghh

#endif // __ELEMENT_STATE_H__