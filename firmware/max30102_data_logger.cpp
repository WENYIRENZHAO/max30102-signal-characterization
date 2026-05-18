// MAX30102 data logging firmware_
// This file contains the microcontroller code for reading raw red and IR values.

#include "Particle.h"
#include "Wire.h"
#define MAX30102_ADDRESS 0x57
#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA 0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED1_PA 0x0C
#define REG_LED2_PA 0x0D
#define REG_PART_ID 0xFF
#define SPO2_CONFIG_VALUE 0x23
#define LED_POWER 0x24
struct Sample {
    uint32_t red;
    uint32_t ir;
    bool ok;
};
bool writeReg(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(MAX30102_ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    return Wire.endTransmission() == 0;
}
uint8_t readReg(uint8_t reg) {
    Wire.beginTransmission(MAX30102_ADDRESS);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0) {
        return 0;
    }
    Wire.requestFrom(MAX30102_ADDRESS, (uint8_t)1);
    if (Wire.available()) {
        return Wire.read();
    }
    return 0;
}
void clearInterrupts() {
    readReg(REG_INTR_STATUS_1);
    readReg(REG_INTR_STATUS_2);
}
void clearFIFO() {
    writeReg(REG_FIFO_WR_PTR, 0x00);
    writeReg(REG_OVF_COUNTER, 0x00);
    writeReg(REG_FIFO_RD_PTR, 0x00);
}
void initMAX30102() {
    writeReg(REG_MODE_CONFIG, 0x40);
    delay(300);
    clearInterrupts();
    clearFIFO();
    writeReg(REG_FIFO_CONFIG, 0x5F);
    writeReg(REG_MODE_CONFIG, 0x03);
    writeReg(REG_SPO2_CONFIG, SPO2_CONFIG_VALUE);
    writeReg(REG_LED1_PA, LED_POWER);
    writeReg(REG_LED2_PA, LED_POWER);
    delay(300);
    clearInterrupts();
    clearFIFO();
}
bool checkMAX30102Init() {
    uint8_t part = readReg(REG_PART_ID);
    uint8_t mode = readReg(REG_MODE_CONFIG);
    uint8_t spo2 = readReg(REG_SPO2_CONFIG);
    uint8_t red = readReg(REG_LED1_PA);
    uint8_t ir = readReg(REG_LED2_PA);
    return part == 0x15 &&
           mode == 0x03 &&
           spo2 == SPO2_CONFIG_VALUE &&
           red == LED_POWER &&
           ir == LED_POWER;
}
Sample readSample() {
    Sample s;
    s.red = 0;
    s.ir = 0;
    s.ok = false;
    Wire.beginTransmission(MAX30102_ADDRESS);
    Wire.write(REG_FIFO_DATA);
    if (Wire.endTransmission(false) != 0) {
        return s;
    }
    Wire.requestFrom(MAX30102_ADDRESS, (uint8_t)6);
    if (Wire.available() < 6) {
        return s;
    }
    s.red |= ((uint32_t)Wire.read()) << 16;
    s.red |= ((uint32_t)Wire.read()) << 8;
    s.red |= Wire.read();
    s.red &= 0x3FFFF;
    s.ir |= ((uint32_t)Wire.read()) << 16;
    s.ir |= ((uint32_t)Wire.read()) << 8;
    s.ir |= Wire.read();
    s.ir &= 0x3FFFF;
    s.ok = true;
    return s;
}
void setup() {
    Serial.begin(115200);
    delay(2000);
    Wire.begin();
    Wire.setSpeed(CLOCK_SPEED_100KHZ);
    initMAX30102();
    if (checkMAX30102Init()) {
        Serial.println("time_ms,red,ir");
    } else {
        Serial.println("INIT_FAILED");
    }
}
void loop() {
    Sample s = readSample();
    if (s.ok) {
        Serial.print(millis());
        Serial.print(",");
        Serial.print(s.red);
        Serial.print(",");
        Serial.println(s.ir);
    }
    delay(100);
}
