#ifndef __ENCODING_H__
#define __ENCODING_H__

#include <stdint.h>
#include <string>

namespace ghr
{

uint8_t readByte(const uint8_t *data, std::size_t &pos)
{
    return data[pos++];
}
void writeByte(uint8_t *data, std::size_t &pos, uint8_t value)
{
    data[pos++] = value;
}

int readInt(const uint8_t *data, std::size_t &pos, bool optimizePositive)
{
    int b = readByte(data, pos);
    int result = b & 0x7F;
    if ((b & 0x80) != 0)
    {
        b = readByte(data, pos);
        result |= (b & 0x7F) << 7;
        if ((b & 0x80) != 0)
        {
            b = readByte(data, pos);
            result |= (b & 0x7F) << 14;
            if ((b & 0x80) != 0)
            {
                b = readByte(data, pos);
                result |= (b & 0x7F) << 21;
                if ((b & 0x80) != 0)
                {
                    b = readByte(data, pos);
                    result |= (b & 0x7F) << 28;
                }
            }
        }
    }
    return optimizePositive ? result : ((((unsigned int)result) >> 1) ^ -(result & 1));
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

} // namespace ghr

#endif // __ENCODING_H__