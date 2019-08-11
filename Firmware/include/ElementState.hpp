#ifndef __ELEMENT_STATE_H__
#define __ELEMENT_STATE_H__

#include <vector>

#include "print.hpp"

namespace ghr
{
enum class ElementState
{
    Inert,
    Waning,
    Strong
};
std::vector<ElementState> getElementStateValues()
{
    return {
        ElementState::Inert,
        ElementState::Waning,
        ElementState::Strong};
}

void print(ElementState &arg)
{
    switch (arg)
    {
    case ElementState::Inert:
        ghr::print("Inert");
        break;
    case ElementState::Waning:
        ghr::print("Waning");
        break;
    case ElementState::Strong:
        ghr::print("Strong");
        break;
    }
}

} // namespace ghr

#endif // __ELEMENT_STATE_H__