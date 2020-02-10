#include <vector>
#include <stdint.h>
#include <iostream>

#include "encoding.hpp"

uint8_t input_buffer[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

int main()
{
    std::size_t pos = 0;
    ghh::writeInt(input_buffer, pos, 0, true);
    ghh::writeInt(input_buffer, pos, 2147483647, true);
    ghh::writeInt(input_buffer, pos, -2147483648, false);
    for (int i = 0; i < 15; i++)
    {
        std::cout << "b: " << (int)input_buffer[i] << " ";
    }
    std::cout << "\n";
    pos = 0;
    std::cout << ghh::readInt(input_buffer, pos, true) << "\n";
    std::cout << ghh::readInt(input_buffer, pos, true) << "\n";
    std::cout << ghh::readInt(input_buffer, pos, false) << "\n";
}