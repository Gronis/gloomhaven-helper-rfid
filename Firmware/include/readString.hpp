#ifndef __READ_STRING_H__
#define __READ_STRING_H__

#include <string>

// TODO remove print
#include <cstdlib>
#include "print.hpp"

#include "encoding.hpp"
#include "optional.hpp"

namespace ghr
{

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
    // print("buffer:\n");
    // for (int i = 0; i < 20; i++)
    // {
    //     char val[2];
    //     itoa(data[i], val, 16);
    //     print(val, " ");
    // }
    // print("\n");
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

}; // namespace ghr

#endif // __READ_STRING_H__