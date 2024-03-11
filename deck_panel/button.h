#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h> //needed for Serial.println
#include <string.h>  //needed for memcpy
//
class Button {
  public:
    Button(uint16_t x, uint16_t y, uint8_t id, uint8_t size);
    Button();

    uint16_t x;
    uint16_t y;

    bool inButtonArea(uint16_t x, uint16_t y) {
        return x > this->x && x < (this->x + this->width) && y > this->y &&
               y < (this->y + this->height);
    }

    uint8_t getId() { return this->id; }

  private:
    uint8_t width;
    uint8_t height;
    uint8_t id;
};

Button::Button(uint16_t x, uint16_t y, uint8_t id, uint8_t size) {
    this->height = size;
    this->width = size;
    this->x = x;
    this->y = y;
    this->id = id;
}

Button::Button() {}

#endif // BUTTON_H
