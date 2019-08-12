
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
#include <ESP8266WiFi.h>
#else
#endif // ARDUINO

#include "gameState.hpp"
#include "protocol.hpp"
#include "message.hpp"
#include "print.hpp"
#include "inputStream.hpp"
#include "outputStream.hpp"
#include "header.hpp"
#include "utils.hpp"
#include "led.hpp"

#define RDM6300_RX_PIN 13 // can be only 13 - on esp8266 force hardware uart!

#define READ_LED_PIN 16 // D0

// SoftwareSerial rfid = SoftwareSerial(RDM6300_RX_PIN, 3);

Rdm6300 rdm6300;

const char *ssid = "Gloomhaven Helper";
const char *password = "gloom123";
const uint16_t port = 58888;
String last_host;
WiFiClient client;
boolean waitingDHCP = false;
char last_mac[18];

// Manage incoming device connection on ESP access point
void onNewStation(WiFiEventSoftAPModeStationConnected sta_info)
{
  Serial1.println("New Station :");
  sprintf(last_mac, "%02X:%02X:%02X:%02X:%02X:%02X", MAC2STR(sta_info.mac));
  Serial1.printf("MAC address : %s\n", last_mac);
  Serial1.printf("Id : %d\n", sta_info.aid);
  waitingDHCP = true;
}

boolean deviceIP(char *mac_device, String &cb)
{

  struct station_info *station_list = wifi_softap_get_station_info();

  while (station_list != NULL)
  {
    char station_mac[18] = {0};
    sprintf(station_mac, "%02X:%02X:%02X:%02X:%02X:%02X", MAC2STR(station_list->bssid));
    String station_ip = IPAddress((&station_list->ip)->addr).toString();

    if (strcmp(mac_device, station_mac) == 0)
    {
      waitingDHCP = false;
      cb = station_ip;
      return true;
    }

    station_list = STAILQ_NEXT(station_list, next);
  }

  wifi_softap_free_station_info();
  cb = "";
  return false;
}

// const std::vector<const int32_t> input_buffer{0x00, -1, 0x01, -1, 0x73, -1, -1};
const std::vector<int32_t> dummy_input_buffer{
    0x00, -1, 0x05, 0x76, -1, 0x20, 0x37, -1, 0x2e, 0x36, -1, 0x00, 0x00, 0x01, 0x73, -1,
    0xae, 0x01, 0x00, 0x00, 0x0c, 0xfb,
    0x08, 0x3a, 0x03, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00, 0x03, 0x12, 0x00, 0x08,
    0x01, 0x05, 0x03, 0x08, 0x01, 0x01, 0x00, 0x00, 0x08, 0x08, 0x08, 0x03, 0x06, 0x08, 0x08, 0x00,
    0x09, 0x04, 0x02, 0x00, 0x00, 0x02, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07, 0x02, 0x06, 0x00,
    0x01, 0x03, 0x07, 0x05, 0x01, 0x00, 0x00, 0x03, 0x0f, 0x0d, 0x09, 0x06, 0x00, 0x00, 0x08, 0x35,
    0x33, 0x37, 0x31, 0x36, 0x34, 0x30, 0x32, 0x0a, 0x00, 0x00, 0x08, 0x53, 0x56, 0x50, 0x51, 0x55,
    0x52, 0x57, 0x54, 0x0b, 0x00, 0x00, 0x08, 0x5c, 0x5f, 0x58, 0x5d, 0x5e, 0x5b, 0x59, 0x5a, 0x0d,
    0x00, 0x00, 0x07, 0x6c, 0x68, 0x6f, 0x6d, 0x69, 0x6e, 0x6b, 0x03, 0x01, 0x80, 0x02, 0x04, 0x06,
    0x0a, 0x01, 0x01, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00,
    0x02, 0x06, 0x01, 0x00, 0x0a, 0x0a, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x04, 0x09, 0x01, 0x06,
    0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x02, 0x02, 0x01, 0x00, 0x05, 0x08, 0x00, 0x00,
    0x00, 0x06, 0x00, 0x00, 0x02, 0x05, 0x00, 0x00, 0x00, 0x00};
// const std::vector<int32_t> input_buffer{
//     0x00, -1, 0x05, 0x76, -1, 0x20, 0x37, -1, 0x2e, 0x36, -1, 0x00, 0x00, 0x01, 0x73, -1,
//     0xa6,
//     0x01, 0x00, 0x00, 0x0c, 0x10, 0x01, 0x3a, 0x03, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01,
//     0x00, 0x03, 0x14, 0x00, 0x01, 0x01, 0x00, 0x03, 0x03, 0x01, 0x06, 0x05, 0x01, 0x00, 0x00, 0x00,
//     0x03, 0x00, 0x01, 0x03, 0x02, 0x04, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
//     0x06, 0x00, 0x00, 0x08, 0x35, 0x33, 0x37, 0x31, -1, 0x36, 0x34, 0x30, 0x32, 0x0a, 0x00, 0x00, 0x08,
//     0x51, 0x52, 0x57, 0x55, 0x56, 0x54, 0x53, 0x50, 0x0b, 0x00, 0x00, 0x08, 0x5c, 0x5f, 0x58, 0x5d,
//     0x5e, 0x5b, 0x59, 0x5a, 0x0d, 0x00, 0x00, 0x08, 0x6c, 0x68, 0x6f, 0x6d, 0x69, 0x6e, 0x6b, 0x6a,
//     0x06, 0x01, 0x52, 0x6f, 0x62, 0x69, 0x6e, 0x20, 0x61, 0x64, 0x73, 0x66, 0xe1, 0x06, 0x00, 0x0c,
//     0x0c, 0x07, 0x00, 0x00, 0x00, -1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x04, 0x00, 0x04, 0x06,
//     0x01, 0x00, 0x31, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x03, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x0e, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x06, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00

// };
// std::size_t input_buffer_index = 0;
ghr::GameState state;
int message_number = 0;

void on_packet(std::string event, std::string payload, uint8_t *data, std::size_t dataLength)
{
  ghr::print("Event: ", event, ", payload: ", payload, ", data length: ", dataLength, "\n");
  ghr::Message msg(data, dataLength);
  if (event[0] == 's')
  {
    std::vector<ghr::AttackModifier> attackModifiers = {};
    // 4 bytes representing message number. Throw away for now
    message_number = msg.readFullInt();
    state.clear();
    ghr::readGameState(msg, state);
  }
}

void send_packet(ghr::Header header)
{
  int new_message_number = message_number + 1;
  std::size_t dataCapacity = 1024;
  uint8_t data[dataCapacity];
  ghr::Message msg(data, dataCapacity);
  msg.writeFullInt(new_message_number);
  ghr::writeGameState(state, msg);
  int dataLength = msg.getPosition();
  header.send_data("s", "", data, dataLength);
  ghr::print("\nSent state\n");
}

// const int32_t read_dummy_data()
// {
//   if (input_buffer_index < input_buffer.size())
//   {
//     return input_buffer[input_buffer_index++];
//   }
//   return -1;
// }

const int read()
{
  if (client.available())
  {
    return client.read();
  }
  return -1;
}

const uint8_t write(uint8_t value)
{
  std::size_t result = client.write(static_cast<char>(value));
  if (result == 1)
  {
    ghr::print((uint32_t)value, " ");
  }
  else
  {
    ghr::print("-- ");
  }
  if (client.available())
  {
  }
}

const std::size_t bufferCapacity = 1024;
uint8_t input_buffer[bufferCapacity];
uint8_t output_buffer[bufferCapacity];
ghr::InputStream input(&read, input_buffer, bufferCapacity);
ghr::OutputStream output(&write, output_buffer, bufferCapacity);
ghr::Header header(input, output);
ghr::Led led(READ_LED_PIN);

void setup()
{
  /* Serial1 is the debug! remember to bridge GPIO-01 to GPIO-02 (D4 and TX on ESP8266) */
  Serial1.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password); // Start the access point
  Serial1.print("Access Point \"");
  Serial1.print(ssid);
  Serial1.println("\" started");

  Serial1.print("IP address:\t");
  Serial1.println(WiFi.softAPIP()); // Send the IP address of the ESP8266 to the computer

  rdm6300.begin(RDM6300_RX_PIN);
  Serial1.println("\nPlace RFID tag near the rdm6300...");
  led.blink(10000, 0.15);
  header.on_data = on_packet;

  static WiFiEventHandler e1;
  e1 = WiFi.onSoftAPModeStationConnected(onNewStation);
}

void loop()
{
  if (waitingDHCP)
  {
    if (deviceIP(last_mac, last_host))
    {
      ghr::print("New host connected: ", last_host, "\n");
    }
  }
  if (last_host.length() > 0 && !client.connected())
  {
    led.blink(0, 1.0);
    ghr::print("Connecting to ", last_host, "\n");
    WiFiClient tmp_client;
    if (tmp_client.connect(last_host, port))
    {
      Serial1.println("Connected!");
      //last_host = "";
      client = tmp_client;
      led.blink(3, 4.0);
    }
  }
  if (client.connected())
  {
    /* if non-zero tag_id, update() returns true- a new tag is near! */
    if (rdm6300.update())
    {
      Serial1.println(rdm6300.get_tag_id(), DEC);
      led.blink(1, 1.0);
      send_packet(header);
    }
  }
  led.update();
  header.update();
  delay(10);
}