#include "Particle.h"
#include "Wire.h"
// MAX30102 CONFIG
#define MAX30102_ADDRESS 0x57
#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03
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
#define NO_FINGER_IR 8000

// LCD PIN CONFIG
// ===============
// LCD 1  -> GND
// LCD 2  -> VUSB / 5V
// LCD 3  -> potentiometer middle pin
// LCD 4  -> D2
// LCD 5  -> GND
// LCD 6  -> D3
// LCD 11 -> D4
// LCD 12 -> D5
// LCD 13 -> D6
// LCD 14 -> D7
// LCD 15 -> VUSB / 5V
// LCD 16 -> GND

#define LCD_RS D2
#define LCD_E D3
#define LCD_D4 D4
#define LCD_D5 D5
#define LCD_D6 D6
#define LCD_D7 D7

// GLOBAL STATE
// =============
struct Sample {
    uint32_t red;
    uint32_t ir;
    bool ok;
};
float dc = 0;
float acAbs = 0;
float latestAC = 0;
float latestTH = 0;
bool wasAbove = false;
uint32_t lastBeatMs = 0;
float bpm = 0;
uint32_t currentIR = 0;
bool fingerDetected = false;
bool fifoSeen = false;
uint32_t lastSerialPrintMs = 0;
uint32_t lastLcdUpdateMs = 0;

// LCD LOW LEVEL DRIVER
void lcdPulseEnable() {
    digitalWrite(LCD_E, LOW);
    delayMicroseconds(2);
    digitalWrite(LCD_E, HIGH);
    delayMicroseconds(2);
    digitalWrite(LCD_E, LOW);
    delayMicroseconds(100);
}
void lcdWrite4Bits(uint8_t value) {
    digitalWrite(LCD_D4, (value >> 0) & 0x01);
    digitalWrite(LCD_D5, (value >> 1) & 0x01);
    digitalWrite(LCD_D6, (value >> 2) & 0x01);
    digitalWrite(LCD_D7, (value >> 3) & 0x01);
    lcdPulseEnable();
}
void lcdSend(uint8_t value, bool rs) {
    digitalWrite(LCD_RS, rs);
    lcdWrite4Bits(value >> 4);
    lcdWrite4Bits(value & 0x0F);
}
void lcdCommand(uint8_t value) {
    lcdSend(value, LOW);
}
void lcdData(uint8_t value) {
    lcdSend(value, HIGH);
}
void lcdClear() {
    lcdCommand(0x01);
    delay(3);
}
void lcdSetCursor(uint8_t col, uint8_t row) {
    uint8_t rowOffset = (row == 0) ? 0x00 : 0x40;
    lcdCommand(0x80 | (col + rowOffset));
}
void lcdPrint(const char* text) {
    while (*text) {
        lcdData(*text++);
    }
}
void lcdPrintLine(uint8_t row, const char* text) {
    lcdSetCursor(0, row);
    uint8_t i = 0;
    while (text[i] && i < 16) {
        lcdData(text[i]);
        i++;
    }
    while (i < 16) {
        lcdData(' ');
        i++;
    }
}
void lcdInit() {
    pinMode(LCD_RS, OUTPUT);
    pinMode(LCD_E, OUTPUT);
    pinMode(LCD_D4, OUTPUT);
    pinMode(LCD_D5, OUTPUT);
    pinMode(LCD_D6, OUTPUT);
    pinMode(LCD_D7, OUTPUT);
    digitalWrite(LCD_RS, LOW);
    digitalWrite(LCD_E, LOW);
    digitalWrite(LCD_D4, LOW);
    digitalWrite(LCD_D5, LOW);
    digitalWrite(LCD_D6, LOW);
    digitalWrite(LCD_D7, LOW);
    delay(100);
    lcdWrite4Bits(0x03);
    delay(5);
    lcdWrite4Bits(0x03);
    delay(5);
    lcdWrite4Bits(0x03);
    delay(5);
    lcdWrite4Bits(0x02);
    delay(5);
    lcdCommand(0x28);
    delay(2);
    lcdCommand(0x0C);
    delay(2);
    lcdCommand(0x06);
    delay(2);
    lcdClear();
}
// MAX30102 I2C BASIC
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
uint8_t getFifoCount() {
    uint8_t wr = readReg(REG_FIFO_WR_PTR);
    uint8_t rd = readReg(REG_FIFO_RD_PTR);
    if (wr >= rd) {
        return wr - rd;
    }
    return 32 + wr - rd;
}

// MAX30102 INIT
void initMAX30102() {
    writeReg(REG_MODE_CONFIG, 0x40);
    delay(300);
    for (int i = 0; i < 20; i++) {
        uint8_t mode = readReg(REG_MODE_CONFIG);
        if ((mode & 0x40) == 0) {
            break;
        }
        delay(50);
    }
    clearInterrupts();
    clearFIFO();
    writeReg(REG_INTR_ENABLE_1, 0x00);
    writeReg(REG_INTR_ENABLE_2, 0x00);
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
    Serial.println("----- MAX30102 CHECK -----");
    Serial.print("PART ID: ");
    Serial.println(part, HEX);
    Serial.print("MODE: ");
    Serial.println(mode, HEX);
    Serial.print("SPO2 CONFIG: ");
    Serial.println(spo2, HEX);
    Serial.print("RED LED: ");
    Serial.println(red, HEX);
    Serial.print("IR LED: ");
    Serial.println(ir, HEX);
    return part == 0x15 &&
           mode == 0x03 &&
           spo2 == SPO2_CONFIG_VALUE &&
           red == LED_POWER &&
           ir == LED_POWER;
}

// SAMPLE READ
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
// HEART RATE LOGIC
void resetBeatState() {
    dc = 0;
    acAbs = 0;
    latestAC = 0;
    latestTH = 0;
    wasAbove = false;
    lastBeatMs = 0;
    bpm = 0;
}
void processSample(Sample s) {
    uint32_t now = millis();
    currentIR = s.ir;
    if (s.ir < NO_FINGER_IR) {
        fingerDetected = false;
        resetBeatState();
        return;
    }
    fingerDetected = true;
    if (dc == 0) {
        dc = (float)s.ir;
    }
    dc = dc * 0.97f + (float)s.ir * 0.03f;
    float ac = (float)s.ir - dc;
    float absAc = ac >= 0 ? ac : -ac;
    acAbs = acAbs * 0.95f + absAc * 0.05f;
    float threshold = acAbs * 1.2f;
    if (threshold < 150.0f) {
        threshold = 150.0f;
    }
    latestAC = ac;
    latestTH = threshold;
    bool above = ac > threshold;
    if (above && !wasAbove) {
        uint32_t delta = now - lastBeatMs;
        if (lastBeatMs > 0 && delta > 300 && delta < 2000) {
            float instantBpm = 60000.0f / (float)delta;
            if (bpm == 0) {
                bpm = instantBpm;
            } else {
                bpm = bpm * 0.75f + instantBpm * 0.25f;
            }
            Serial.print("BEAT DETECTED BPM=");
            Serial.println(bpm, 1);
        }
        lastBeatMs = now;
    }
    wasAbove = above;
    if (now - lastSerialPrintMs > 300) {
        uint8_t wr = readReg(REG_FIFO_WR_PTR);
        uint8_t rd = readReg(REG_FIFO_RD_PTR);
        uint8_t ovf = readReg(REG_OVF_COUNTER);
        uint8_t count = getFifoCount();
        Serial.print("WR=");
        Serial.print(wr);
        Serial.print(" RD=");
        Serial.print(rd);
        Serial.print(" COUNT=");
        Serial.print(count);
        Serial.print(" OVF=");
        Serial.print(ovf);
        Serial.print(" IR=");
        Serial.print((unsigned long)s.ir);
        Serial.print(" AC=");
        Serial.print((int32_t)ac);
        Serial.print(" TH=");
        Serial.print((int32_t)threshold);
        Serial.print(" BPM=");
        Serial.println(bpm, 1);
        if (ovf > 0) {
            writeReg(REG_OVF_COUNTER, 0x00);
        }
        lastSerialPrintMs = now;
    }
}
// LCD DISPLAY UPDATE
void makeBarLine(char* line, int acValue) {
    for (int i = 0; i < 16; i++) {
        line[i] = '-';
    }
    int level = 8 + (acValue / 250);
    if (level < 0) {
        level = 0;
    }
    if (level > 15) {
        level = 15;
    }
    line[level] = '*';
    line[16] = '\0';
}
void updateLCD() {
    uint32_t now = millis();
    if (now - lastLcdUpdateMs < 250) {
        return;
    }
    lastLcdUpdateMs = now;
    static int spinner = 0;
    const char frames[] = {'-', '\\', '|', '/'};
    char line1[17];
    char line2[17];
    if (!fingerDetected) {
        snprintf(line1, sizeof(line1), "Place finger %c", frames[spinner % 4]);
        snprintf(line2, sizeof(line2), "IR:%lu", (unsigned long)currentIR);
    } else if (bpm <= 0) {
        snprintf(line1, sizeof(line1), "Reading... %c", frames[spinner % 4]);
        makeBarLine(line2, (int)latestAC);
    } else {
        int bpmInt = (int)(bpm + 0.5f);
        snprintf(line1, sizeof(line1), "BPM:%3d     %c", bpmInt, frames[spinner % 4]);
        makeBarLine(line2, (int)latestAC);
    }
    spinner++;
    lcdPrintLine(0, line1);
    lcdPrintLine(1, line2);
}

// SETUP / LOOP
void setup() {
    Serial.begin(115200);
    delay(2000);
    Wire.begin();
    Wire.setSpeed(CLOCK_SPEED_100KHZ);
    lcdInit();
    lcdPrintLine(0, "MAX30102 + LCD");
    lcdPrintLine(1, "Starting...");
    Serial.println();
    Serial.println("====================================");
    Serial.println(" MAX30102 + LCD FINAL");
    Serial.println("====================================");
    initMAX30102();
    if (checkMAX30102Init()) {
        Serial.println("INIT OK");
        lcdPrintLine(0, "Sensor INIT OK");
        lcdPrintLine(1, "Waiting FIFO");
    } else {
        Serial.println("INIT FAILED");
        lcdPrintLine(0, "INIT FAILED");
        lcdPrintLine(1, "Check wires");
    }
}
