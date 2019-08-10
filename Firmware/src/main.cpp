
/*
 * A simple example to interface with rdm6300 rfid reader using esp8266.
 * We use hardware uart "Serial" instead of the default software uart driver.
 *
 * Note:
 *     The esp8266 let us use 1.5 uarts:
 *     Serial=uart0=rx+tx, Serial1=uart1=tx only (rx pin is used as flash io).
 *     Here we sawp the uart pins so uart0_rx goes to the rdm6300_tx,
 *     and uart1_tx goes to what used to be uart0_tx-
 *     so debug message goes from Serial1.print(...) to the pc debug terminal.
 *     https://github.com/esp8266/Arduino/blob/master/doc/reference.rst#serial
 *
 * Connections:
 *     | esp8266 | nodemcu | rdm6300 | notes                               |
 *     |---------+---------+---------|-------------------------------------|
 *     | GPIO-01 | TX      |         | This is TXD0, connect it to GPIO-02 |
 *     | GPIO-02 | D4      |         | This is TXD1, connect it to GPIO-01 |
 *     | GPIO-03 | RX      |         | Leave it unconnected for flashing   |
 *     | GPIO-13 | D7      | TX      | TX level is same 3.3V as esp8266    |
 *     |         | VU (5V) | VCC     | The rdm6300 must be powered with 5V |
 *     | GND     | GND     | GND     |                                     |
 *
 *     * GPIO-01 to GPIO-02 is for debug terminal output.
 *
 * Arad Eizen (https://github.com/arduino12).
 */

#include <vector>
#include <stdint.h>

#ifdef ARDUINO
#include <Arduino.h>
#include <rdm6300.h>
#include <SoftwareSerial.h>
#else
#include <iostream>
#endif // ARDUINO

#include "message.hpp"
#include "led.hpp"
#include "packet.hpp"
#include "print.hpp"
#include "inputStream.hpp"
#include "utils.hpp"

#define RDM6300_RX_PIN 13 // can be only 13 - on esp8266 force hardware uart!

#define READ_LED_PIN 16 // D0

// SoftwareSerial rfid = SoftwareSerial(RDM6300_RX_PIN, 3);

Rdm6300 rdm6300;

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
    ghr::print("Got int: ", msg.readInt(true));
    ghr::print("Got int: ", msg.readInt(true));
    ghr::print("Got int: ", msg.readInt(true));
    ghr::print("Got byte: ", msg.readBoolean());
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
ghr::Led led(READ_LED_PIN);

void setup()
{
  /* Serial1 is the debug! remember to bridge GPIO-01 to GPIO-02 (D4 and TX on ESP8266) */
  Serial1.begin(115200);

  rdm6300.begin(RDM6300_RX_PIN);
  Serial1.println("\nPlace RFID tag near the rdm6300...");
}

void loop()
{
  /* if non-zero tag_id, update() returns true- a new tag is near! */
  if (rdm6300.update())
  {
    Serial1.println(rdm6300.get_tag_id(), DEC);
    led.blink(1);
  }
  led.update();
  packet.update();
  delay(10);
}