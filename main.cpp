#include <vector>
#include <iostream>
#include <stdint.h>

#include "stream.hpp"

const std::vector<const int32_t> input_buffer{0x00, -1, 0x01, -1, 0x73, -1, -1};
std::size_t input_buffer_index = 0;

const int32_t read()
{
    if (input_buffer_index < input_buffer.size())
    {
        return input_buffer[input_buffer_index++];
    }
    return -1;
}
const std::size_t bufferCapacity = 1024;
uint8_t buffer[bufferCapacity];
Stream stream(&read, buffer, bufferCapacity);

int main()
{
    stream.asyncReadUTFString([](std::string result) {
        std::cout << result;
    });
    stream.update();
    stream.update();
    stream.update();
    stream.update();
    stream.update();
    stream.update();
    stream.update();
}