#include <ADCTouch.h>
#include <TinyWireM.h>          // I²C master library for tiny

const uint8_t TOUCH_PINS[4]   = {1, 3, 4, 5}; // Arduino pin numbers
const uint8_t THRESHOLD[4]    = {600, 600, 600, 600}; // tune if needed
const uint8_t I2C_SLAVE_ADDR  = 0x20;

uint8_t touchFlags = 0;     // b0…b3 = pad0…pad3

void setup()
{
  // ----- I²C ----------------------------------------------
  TinyWireM.begin();        // SDA on PB0, SCL on PB2

  // ----- LED ----------------------------------------------
  pinMode(0, OUTPUT);       // PB0
}

void loop()
{
  touchFlags = 0;

  for (uint8_t i = 0; i < 4; ++i)
  {
    uint16_t val = ADCTouch.read(TOUCH_PINS[i], __AVR_ATtiny85__);
    if (val > THRESHOLD[i])  bitSet(touchFlags, i);
  }

  // LED = OR of all flags
  digitalWrite(0, touchFlags ? HIGH : LOW);

  // Send byte to I²C slave (remove if not needed)
  TinyWireM.beginTransmission(I2C_SLAVE_ADDR);
  TinyWireM.write(touchFlags);
  TinyWireM.endTransmission();

  delay(10);               // adjust for responsiveness vs. current
}