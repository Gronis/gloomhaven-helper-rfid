#ifndef __ELEMENT_STATE_H__
#define __ELEMENT_STATE_H__

#include <vector>
#include <iostream>

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

} // namespace ghr

std::ostream &operator<<(std::ostream &os, ghr::ElementState const &p)
{
    switch (p)
    {
    case ghr::ElementState::INERT:
        return os << "inert";
    case ghr::ElementState::WANING:
        return os << "waning";
    case ghr::ElementState::STRONG:
        return os << "strong";
    }
    return os;
}

#endif // __ELEMENT_STATE_H__