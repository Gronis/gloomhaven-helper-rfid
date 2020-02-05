#ifndef __ENCODING_H__
#define __ENCODING_H__

#include <stdint.h>
#include <string>

#include "print.hpp"
#include "optional.hpp"

namespace ghr
{

void writeByte(uint8_t *data, std::size_t &pos, uint8_t value)
{
    data[pos++] = value;
}

void writeInt(uint8_t *data, std::size_t &pos, int value, bool optimizePositive)
{
    unsigned int val = value;
    if (!optimizePositive)
    {
        if (value >= 0)
        {
            val = ((unsigned int)value) << 1;
        }
        else
        {
            val = (((unsigned int)-value) << 1) - 1;
        }
    }
    uint8_t carry = true;
    for (auto i = 0; i < 5 && carry; i++)
    {
        uint8_t b = (uint8_t)val;
        val = val >> 7;
        carry = val > 0;
        b |= carry ? 0x80 : 0;
        writeByte(data, pos, b);
    }
}

void writeAscii(uint8_t *data, std::size_t &pos, std::string value)
{
    ghr::print("Wrote ascii string: ");
    for (std::size_t i = 0; i < value.length(); i++, pos++)
    {
        ghr::print("Wrote ascii string: [", i, "]\n");
        ghr::print(value[i]);
        data[pos] = value[i];
    };
    ghr::print("\n");
    data[pos - 1] |= 0x80;
}

void writeString(uint8_t *data, std::size_t &pos, tl::optional<std::string> value)
{
    // Assume we use ASCII encoding
    bool useAscii = true; //value.has_value() && value.value().length() != 0;
    if (useAscii)
    {
        std::string v = " ";
        v[0] = (char)0;
        writeAscii(data, pos, value ? value.value() : v);
    }
    else
    {
        // TODO Implement UTF-8 support
    }
}

} // namespace ghr

#endif // __ENCODING_H__