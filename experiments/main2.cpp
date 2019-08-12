#include <SoftwareSerial.h>
#include <Arduino.h>
const int BUFFER_SIZE = 14;      // RFID DATA FRAME FORMAT: 1byte head (value: 2), 10byte data (2byte version + 8byte tag), 2byte checksum, 1byte tail (value: 3)
const int DATA_SIZE = 10;        // 10byte data (2byte version + 8byte tag)
const int DATA_VERSION_SIZE = 2; // 2byte version (actual meaning of these two bytes may vary)
const int DATA_TAG_SIZE = 8;     // 8byte tag
const int CHECKSUM_SIZE = 2;     // 2byte checksum
SoftwareSerial ssrfid = SoftwareSerial(13, 3);
uint8_t buffer[BUFFER_SIZE]; // used to store an incoming data frame
int buffer_index = 0;
unsigned long previous_tag = 0L;

long hexstr_to_value(char *str, unsigned int length)
{ // converts a     hexadecimal value (encoded as ASCII string) to a numeric value
    char *copy = (char *)malloc((sizeof(char) * length) + 1);
    memcpy(copy, str, sizeof(char) * length);
    copy[length] = '\0';
    // the variable "copy" is a copy of the parameter "str". "copy" has an additional '\0' element to make sure that "str" is null-terminated.
    long value = strtol(copy, NULL, 16); // strtol converts a null-terminated string to a long value
    free(copy);                          // clean up
    return value;
}

unsigned extract_tag()
{
    uint8_t msg_head = buffer[0];
    uint8_t *msg_data = buffer + 1; // 10 byte => data contains 2byte version + 8byte tag
    uint8_t *msg_data_version = msg_data;
    uint8_t *msg_data_tag = msg_data + 2;
    uint8_t *msg_checksum = buffer + 11; // 2 byte
    uint8_t msg_tail = buffer[13];
    // print message that was sent from RDM630/RDM6300
    Serial.println("--------");
    Serial.print("Message-Head: ");
    Serial.println(msg_head);
    Serial.println("Message-Data (HEX): ");
    for (int i = 0; i < DATA_VERSION_SIZE; ++i)
    {
        Serial.print(char(msg_data_version[i]));
    }
    Serial.println(" (version)");
    for (int i = 0; i < DATA_TAG_SIZE; ++i)
    {
        Serial.print(char(msg_data_tag[i]));
    }
    Serial.println(" (tag)");
    Serial.print("Message-Checksum (HEX): ");
    for (int i = 0; i < CHECKSUM_SIZE; ++i)
    {
        Serial.print(char(msg_checksum[i]));
    }
    Serial.println("");
    Serial.print("Message-Tail: ");
    Serial.println(msg_tail);
    Serial.println("--");
    long tag = hexstr_to_value((char *)msg_data_tag, DATA_TAG_SIZE);
    Serial.print("Extracted Tag: ");
    Serial.println(tag);
    long checksum = 0;
    for (int i = 0; i < DATA_SIZE; i += CHECKSUM_SIZE)
    {
        long val = hexstr_to_value((char *)msg_data + i, CHECKSUM_SIZE);
        checksum ^= val;
    }
    Serial.print("Extracted Checksum (HEX): ");
    Serial.print(checksum, HEX);
    if (checksum == hexstr_to_value((char *)msg_checksum, CHECKSUM_SIZE))
    {                          // compare calculated checksum to retrieved checksum
        Serial.print(" (OK)"); // calculated checksum corresponds to transmitted checksum!
    }
    else
    {
        Serial.print(" (NOT OK)"); // checksums do not match
    }
    Serial.println("");
    Serial.println("--------");
    return tag;
}

void setup2()
{
    pinMode(8, OUTPUT); //led pin
    Serial.begin(115200);

    ssrfid.begin(9600);
    ssrfid.listen();

    Serial.println("INIT DONE");
}
void loop2()
{
    if (ssrfid.available() > 0)
    {
        bool call_extract_tag = false;

        int ssvalue = ssrfid.read(); // read
        if (ssvalue == -1)
        { // no data was read
            return;
        }
        if (ssvalue == 2)
        { // RDM630/RDM6300 found a tag => tag incoming
            buffer_index = 0;
        }
        else if (ssvalue == 3)
        {                            // tag has been fully transmitted
            call_extract_tag = true; // extract tag at the end of the function call
        }
        if (buffer_index >= BUFFER_SIZE)
        { // checking for a buffer overflow (It's very unlikely that an buffer overflow comes up!)
            Serial.println("Error: Buffer overflow detected!");
            return;
        }

        buffer[buffer_index++] = ssvalue; // everything is alright => copy current value to buffer
        if (call_extract_tag == true)
        {
            if (buffer_index == BUFFER_SIZE)
            {
                unsigned tag = extract_tag();
                Serial.println(tag);
                if (previous_tag == tag)
                {
                    Serial.println("Same tag ===================");
                }
                // else
                // {
                //     //put the triggering code here digitalWrite(8,1); delay(1000); digitalWrite(8,0); //Serial.println(previoustag); //Serial.println(tag); previoustag = tag; }
                // }
                else
                { // something is wrong... start again looking for preamble (value: 2)
                    buffer_index = 0;
                    return;
                }
            }
        }
        else
        {
            //    Serial.println("NO Cards detected ++++++++++++++++++++++++++++++++++");
            previous_tag = 0L; // reset previous tag
        }
    }
    delay(10);
}