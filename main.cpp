#include <vector>
#include <iostream>
#include <stdint.h>

#include "inputStream.hpp"

// const std::vector<const int32_t> input_buffer{0x00, -1, 0x01, -1, 0x73, -1, -1};
// const std::vector<const int32_t> input_buffer{0x00, -1, 0x05, 0x76, -1, 0x20, 0x37, -1, 0x2e, 0x36};
// const std::vector<const int32_t> input_buffer{0x00, -1, 0x01, -1, 0x73, -1, -1, 0x00, -1, 0x05, 0x76, -1, 0x20, 0x37, -1, 0x2e, 0x36};
const std::vector<const int32_t> input_buffer{0xa0, -1, 0x01};
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
InputStream input(&read, buffer, bufferCapacity);

int main()
{
    input.asyncReadVarint([](int32_t result) {
        std::cout << "Result: \"" << result << "\"\n";
    });
    // input.asyncReadUTFString([](std::string result) {
    //     std::cout << "Result: \"" << result << "\"\n";
    // });
    for (auto i = 0; i < 10; i++)
    {
        input.update();
    }
}