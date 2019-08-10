#ifndef __ELEMENT_STATE_H__
#define __ELEMENT_STATE_H__

#include <vector>

#include "print.hpp"

namespace ghr
{
enum class ElementState
{
    INERT,
    WANING,
    STRONG
};
std::vector<ElementState> getElementStateValues()
{
    return {
        ElementState::INERT,
        ElementState::WANING,
        ElementState::STRONG};
}

void print(ElementState &arg)
{
    switch (arg)
    {
    case ElementState::INERT:
        ghr::print("inert");
        break;
    case ElementState::WANING:
        ghr::print("waning");
        break;
    case ElementState::STRONG:
        ghr::print("strong");
        break;
    }
}

} // namespace ghr

#endif // __ELEMENT_STATE_H__