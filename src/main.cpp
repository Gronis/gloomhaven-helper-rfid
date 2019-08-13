
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
#include <ESP8266WiFi.h>
#else
#endif // ARDUINO

#include "gameState.hpp"
#include "protocol/v7_6/protocol.hpp"
#include "protocol/v8_0/protocol.hpp"
#include "message.hpp"
#include "print.hpp"
#include "inputStream.hpp"
#include "outputStream.hpp"
#include "client.hpp"
#include "utils.hpp"
#include "led.hpp"

#define RDM6300_RX_PIN 13 // can be only 13 (D7) - on esp8266 force hardware uart!

#define READ_LED_PIN 16 // D0

Rdm6300 rdm6300;

const char *ssid = "Gloomhaven Helper";
const char *password = "gloom123";
const uint16_t port = 58888;
String last_host;
String server_host;
WiFiClient tcp_connection;
ghr::GameState state;
int message_number = 0;
ghr::Led led(READ_LED_PIN);

std::function<void(ghr::GameState &, ghr::Message &)> readGameState;
std::function<void(const ghr::GameState &, ghr::Message &)> writeGameState;

boolean waitingDHCP = false;
char last_mac[18];

// Manage incoming device connection on ESP access point
void on_new_station(WiFiEventSoftAPModeStationConnected sta_info)
{
  sprintf(last_mac, "%02X:%02X:%02X:%02X:%02X:%02X", MAC2STR(sta_info.mac));
  ghr::print("New Station, MAC: ", last_mac, ", id: ", sta_info.aid, "\n");
  waitingDHCP = true;
}

bool get_device_ip(char *mac_device, String &cb)
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

void connect_to_host(String host)
{
  if (host.length() > 0)
  {
    led.blink(0, 1.0);
    ghr::print("Connecting to ", host, "\n");
    WiFiClient tmp_tcp_connection;
    if (tmp_tcp_connection.connect(host, port))
    {
      ghr::print("Connected!\n");
      tcp_connection = tmp_tcp_connection;
      server_host = host;
      led.blink(3, 4.0);
    }
  }
}

void on_packet(std::string event, std::string payload, uint8_t *data, std::size_t dataLength)
{
  ghr::print("Event: ", event, ", payload: ", payload, ", data length: ", dataLength, "\n");
  if (tcp_connection.connected())
  {
    ghr::Message msg(data, dataLength);
    if (event[0] == 's')
    {
      std::vector<ghr::AttackModifier> attackModifiers = {};
      // 4 bytes representing message number. Throw away for now
      message_number = msg.readFullInt();
      state.clear();
      readGameState(state, msg);
    }
    if (event[0] == 'v')
    {
      std::string version = payload;
      if (version == "7.6")
      {
        ghr::print("version ", version, " is supported\n");
        readGameState = ghr::protocol::v7_6::readGameState;
        writeGameState = ghr::protocol::v7_6::writeGameState;
      }
      else if (version == "8.0")
      {
        ghr::print("version ", version, " is supported\n");
        readGameState = ghr::protocol::v8_0::readGameState;
        writeGameState = ghr::protocol::v8_0::writeGameState;
      }
      else
      {
        ghr::print("version ", version, " is unsupported. Disconnecting\n");
        tcp_connection.stop();
        last_host = "";
        led.blink(10000, 0.15);
      }
    }
  }
}

void send_packet(ghr::Client client)
{
  message_number = message_number + 1;
  std::size_t dataCapacity = 1024;
  uint8_t data[dataCapacity];
  ghr::Message msg(data, dataCapacity);
  msg.writeFullInt(message_number);
  writeGameState(state, msg);
  int dataLength = msg.getPosition();
  client.send_data("s", "", data, dataLength);
  ghr::print("\nSent state\n");
}

const int read()
{
  if (tcp_connection.available())
  {
    return tcp_connection.read();
  }
  return -1;
}

const uint8_t write(uint8_t value)
{
  std::size_t result = tcp_connection.write(static_cast<char>(value));
  if (result == 1)
  {
    ghr::print((uint32_t)value, " ");
  }
  else
  {
    ghr::print("-- ");
  }
  if (tcp_connection.available())
  {
  }
}

const std::size_t bufferCapacity = 1024;
uint8_t input_buffer[bufferCapacity];
uint8_t output_buffer[bufferCapacity];
ghr::InputStream input(&read, input_buffer, bufferCapacity);
ghr::OutputStream output(&write, output_buffer, bufferCapacity);
ghr::Client client(input, output);

void setup()
{
  /* Serial1 is the debug! remember to bridge GPIO-01 to GPIO-02 (D4 and TX on ESP8266) */
  Serial1.begin(115200);

  rdm6300.begin(RDM6300_RX_PIN);
  led.blink(10000, 0.15);
  client.on_data = on_packet;

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password); // Start the access point

  ghr::print("Access Point: ", ssid, " started\n");
  ghr::print("IP address: ", WiFi.softAPIP(), "\n");

  static WiFiEventHandler e1;
  e1 = WiFi.onSoftAPModeStationConnected(on_new_station);
}

void loop()
{
  if (waitingDHCP)
  {
    if (get_device_ip(last_mac, last_host))
    {
      ghr::print("New host connected: ", last_host, "\n");
    }
  }
  if (!tcp_connection.connected())
  {
    connect_to_host(last_host);
  }
  if (!tcp_connection.connected())
  {
    connect_to_host(server_host);
  }
  if (tcp_connection.connected())
  {
    /* if non-zero tag_id, update() returns true- a new tag is near! */
    if (rdm6300.update())
    {
      ghr::print(rdm6300.get_tag_id(), "\n");
      led.blink(1, 1.0);
      send_packet(client);
    }
  }
  led.update();
  client.update();
  delay(10);
}