#include <vector>
#include <stdint.h>

#include "AttackModifier.hpp"
#include "ElementState.hpp"
#include "playerInit.hpp"
#include "message.hpp"
#include "print.hpp"
#include "inputStream.hpp"
#include "packet.hpp"
#include "utils.hpp"

// const std::vector<const int32_t> input_buffer{0x00, -1, 0x01, -1, 0x73, -1, -1};
const std::vector<int32_t> input_buffer{
    0x00, -1, 0x05, 0x76, -1, 0x20, 0x37, -1, 0x2e, 0x36, -1, 0x00, 0x00, 0x01, 0x73, -1,
    0xa6,
    0x01, 0x00, 0x00, 0x0c, 0x10, 0x01, 0x3a, 0x03, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01,
    0x00, 0x03, 0x14, 0x00, 0x01, 0x01, 0x00, 0x03, 0x03, 0x01, 0x06, 0x05, 0x01, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x01, 0x03, 0x02, 0x04, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
    0x06, 0x00, 0x00, 0x08, 0x35, 0x33, 0x37, 0x31, -1, 0x36, 0x34, 0x30, 0x32, 0x0a, 0x00, 0x00, 0x08,
    0x51, 0x52, 0x57, 0x55, 0x56, 0x54, 0x53, 0x50, 0x0b, 0x00, 0x00, 0x08, 0x5c, 0x5f, 0x58, 0x5d,
    0x5e, 0x5b, 0x59, 0x5a, 0x0d, 0x00, 0x00, 0x08, 0x6c, 0x68, 0x6f, 0x6d, 0x69, 0x6e, 0x6b, 0x6a,
    0x06, 0x01, 0x52, 0x6f, 0x62, 0x69, 0x6e, 0x20, 0x61, 0x64, 0x73, 0x66, 0xe1, 0x06, 0x00, 0x0c,
    0x0c, 0x07, 0x00, 0x00, 0x00, -1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x04, 0x00, 0x04, 0x06,
    0x01, 0x00, 0x31, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x03, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0e, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x06, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00

};
// const std::vector<const int32_t> input_buffer{0x00, -1, 0x01, -1, 0x73, -1, -1, 0x00, -1, 0x05, 0x76, -1, 0x20, 0x37, -1, 0x2e, 0x36};
// const std::vector<const int32_t> input_buffer{0xa0, -1, 0x01};
// const std::vector<const int32_t> input_buffer{0xa0, -1, 0x01, -1, -1, 0xa0, -1, 0x01, 0xa0, 0x01};
std::size_t input_buffer_index = 0;

void receive(std::string event, std::string payload, uint8_t *data, std::size_t dataLength)
{
    ghr::print("Event: ", event, ", payload: ", payload, ", data length: ", dataLength, "\n");
    ghr::Message msg(data, dataLength);
    if (event[0] == 's')
    {

        std::vector<ghr::AttackModifier> attackModifiers = {};

        // 4 bytes representing message number. Throw away for now
        msg.readByte();
        msg.readByte();
        msg.readByte();
        msg.readByte();

        ghr::print("Got round: ", msg.readInt(true), "\n");
        ghr::print("Got scen nr: ", msg.readInt(true), "\n");
        ghr::print("Got scen lvl: ", msg.readInt(true), "\n");
        ghr::print("Got track standees: ", msg.readBoolean(), "\n");
        ghr::print("Got rand standees: ", msg.readBoolean(), "\n");
        ghr::print("Got elite first: ", msg.readBoolean(), "\n");
        ghr::print("Got expire cond: ", msg.readBoolean(), "\n");
        ghr::print("Got solo: ", msg.readBoolean(), "\n");
        ghr::print("Got hide stats: ", msg.readBoolean(), "\n");
        ghr::print("Got calc stats: ", msg.readBoolean(), "\n");
        ghr::print("Got can draw: ", msg.readBoolean(), "\n");
        ghr::print("Got needs shuffle: ", msg.readBoolean(), "\n");
        ghr::print("Got player init: ", msg.readEnum(ghr::getPlayerInitValues()), "\n");
        msg.readEnumArray(attackModifiers, ghr::getAttackModifierValues());

        for (auto &&am : attackModifiers)
        {
            ghr::print("attack modifier: ", am, "\n");
        }

        ghr::print("Got attack modifier 1: ", msg.readEnumOrNull(ghr::getAttackModifierValues()).has_value(), "\n");
        ghr::print("Got attack modifier 2: ", msg.readEnumOrNull(ghr::getAttackModifierValues()).has_value(), "\n");

        ghr::print("Got fire state:  ", msg.readEnum(ghr::getElementStateValues()), "\n");
        ghr::print("Got ics state:   ", msg.readEnum(ghr::getElementStateValues()), "\n");
        ghr::print("Got air state:   ", msg.readEnum(ghr::getElementStateValues()), "\n");
        ghr::print("Got earth state: ", msg.readEnum(ghr::getElementStateValues()), "\n");
        ghr::print("Got light state: ", msg.readEnum(ghr::getElementStateValues()), "\n");
        ghr::print("Got dark state:  ", msg.readEnum(ghr::getElementStateValues()), "\n");

        // state.attackModifier1 = readEnumOrNull(input, AttackModifier.values);
        // state.attackModifier2 = readEnumOrNull(input, AttackModifier.values);
        // state.fire = readEnum(input, ElementState.values);
        // state.ice = readEnum(input, ElementState.values);
        // state.air = readEnum(input, ElementState.values);
        // state.earth = readEnum(input, ElementState.values);
        // state.light = readEnum(input, ElementState.values);
        // state.dark = readEnum(input, ElementState.values);
    }
}

const int32_t read_dummy_data()
{
    if (input_buffer_index < input_buffer.size())
    {
        return input_buffer[input_buffer_index++];
    }
    return -1;
}
const std::size_t bufferCapacity = 1024;
uint8_t buffer[bufferCapacity];
ghr::InputStream input(&read_dummy_data, buffer, bufferCapacity);
ghr::Packet packet(receive, input);

int main()
{
    // input.queueReadVarint([](int32_t result) {
    //     std::cout << "Result: \"" << result << "\"\n";
    // });
    // input.queueReadVarint([](int32_t result) {
    //     std::cout << "Result: \"" << result << "\"\n";
    // });
    // input.queueReadVarint([](int32_t result) {
    //     std::cout << "Result: \"" << result << "\"\n";
    // });
    // input.queueReadUTFString([](std::string result) {
    //     std::cout << "Result: \"" << result << "\"\n";
    // });

    // input.onData([]() {

    // });

    for (auto i = 0; i < 10; i++)
    {
        packet.update();
    }
}
