#ifndef __LED_H__
#define __LED_H__

// TODO: Move constant elsewhere
#define ANALOG_WRITE_HIGH 1024

#include <Arduino.h>

namespace ghr
{

class Led
{
private:
    unsigned long __end_time;
    unsigned int __pin;
    double __speed;

public:
    Led(unsigned int pin) : __pin(pin)
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
        this->blink(0, 1.0);
    }
    void blink(unsigned int nr_blinks, double speed)
    {
        this->__speed = speed;
        this->__end_time = millis() + (unsigned long)(314 * nr_blinks / __speed);
        update();
    }

    void update()
    {
        long deltaTime = (long)millis() - this->__end_time;
        if (deltaTime > 0)
        {
            analogWrite(this->__pin, 0);
        }
        else
        {
            analogWrite(this->__pin, ANALOG_WRITE_HIGH * (1 - cos(((double)deltaTime * __speed) / 50)) / 2);
        }
    }
};

} // namespace ghr

#endif // __LED_H__
