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

int readUtf8Length(const uint8_t *data, std::size_t &pos, int b)
{
    int result = b & 0x3F; // Mask all but first 6 bits.
    if ((b & 0x40) != 0)
    { // Bit 7 means another byte, bit 8 means UTF8.
        b = readByte(data, pos);
        result |= (b & 0x7F) << 6;
        if ((b & 0x80) != 0)
        {
            b = readByte(data, pos);
            result |= (b & 0x7F) << 13;
            if ((b & 0x80) != 0)
            {
                b = readByte(data, pos);
                result |= (b & 0x7F) << 20;
                if ((b & 0x80) != 0)
                {
                    b = readByte(data, pos);
                    result |= (b & 0x7F) << 27;
                }
            }
        }
    }
    return result;
}

std::string readUtf8(const uint8_t *data, std::size_t &pos, int charCount)
{
    // Try to read 7 bit ASCII chars.
    char chars[charCount];
    std::size_t charIndex = 0;
    std::size_t position = pos;
    int b;
    while (charIndex < charCount)
    {
        b = readByte(data, position);
        if ((b & 0x80) != 0)
        {
            position--;
            break;
        }
        chars[charIndex++] = (char)b;
    }
    pos = position;
    std::string result;
    result.assign(chars, charIndex);
    return result;
}

std::string readAscii(uint8_t *data, std::size_t &pos)
{
    std::size_t end = pos;
    std::size_t start = pos - 1;
    char *location = (char *)data + start;
    int b;
    do
    {
        b = readByte(data, end);
    } while ((b & 0x80) == 0);
    // end = start;
    // do
    // {
    //     b = readByte(data, end);

    // } while ((b & 0x80) == 0);
    data[end - 1] &= 0x7F; // Mask end of ascii bit.

    std::string value;
    value.assign(location, end - start);
    data[end - 1] |= 0x80;
    pos = end;
    return value;
}

/** Reads the length and string of UTF8 characters, or null. This can read strings written by
 * {@link Output#writeString(String)} , {@link Output#writeString(CharSequence)}, and {@link Output#writeAscii(String)}.
 * @return May be null. */
tl::optional<std::string> readString(uint8_t *data, std::size_t &pos)
{
    int b = readByte(data, pos);
    // ASCII.
    if ((b & 0x80) == 0)
    {
        return readAscii(data, pos);
    }
    // Null, empty, or UTF8.
    int charCount = readUtf8Length(data, pos, b);
    switch (charCount)
    {
    case 0:
        return tl::nullopt;
    case 1:
        return "";
    }
    charCount--;
    return readUtf8(data, pos, charCount);
}

void writeAscii(uint8_t *data, std::size_t &pos, std::string value)
{
    ghr::print("Wrote ascii string: ");
    for (std::size_t i = 0; i < value.length(); i++, pos++)
    {
        ghr::print("Wrote ascii string: [", i , "]\n");
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