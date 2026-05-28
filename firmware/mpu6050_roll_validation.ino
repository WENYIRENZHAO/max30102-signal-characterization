#include <Wire.h>

// ===== Pins =====
const byte MPU_ADDR = 0x68;
const int LED_PIN = 13;

// ===== MPU6050 Data =====
int16_t AcX, AcY, AcZ;
int16_t GyX, GyY, GyZ;

bool readMPU6050() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  byte error = Wire.endTransmission(false);
  if (error != 0) {
    Serial.print("I2C error: ");
    Serial.println(error);
    return false;
  }
  int count = Wire.requestFrom(MPU_ADDR, (byte)14, (byte)true);
  if (count != 14) {
    Serial.print("Bad read. Bytes received: ");
    Serial.println(count);
    return false;
  }
  AcX = (Wire.read() << 8) | Wire.read();
  AcY = (Wire.read() << 8) | Wire.read();
  AcZ = (Wire.read() << 8) | Wire.read();
  // Temperature, ignored
  Wire.read();
  Wire.read();
  GyX = (Wire.read() << 8) | Wire.read();
  GyY = (Wire.read() << 8) | Wire.read();
  GyZ = (Wire.read() << 8) | Wire.read();
  return true;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(100000);

  pinMode(LED_PIN, OUTPUT);
  Serial.println("=================================");
  Serial.println("MPU6050 Tilt Measurement Mode");
  Serial.println("Output: AcX, AcY, AcZ, RollRaw");
  Serial.println("No servo control. No roll constraint.");
  Serial.println("=================================");
  // Wake up MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);

  byte error = Wire.endTransmission(true);
  if (error == 0) {
    Serial.println("MPU6050 wake-up successful.");
  } else {
    Serial.print("MPU6050 wake-up failed. Error: ");
    Serial.println(error);
  }

  delay(1000);
}
void loop() {
  bool ok = readMPU6050();
  if (!ok) {
    digitalWrite(LED_PIN, LOW);
    Serial.println("MPU read failed.");
    delay(300);
    return;
  }
  digitalWrite(LED_PIN, HIGH);
  // Raw roll angle from accelerometer
  float rollRaw = atan2((float)AcY, (float)AcZ) * 180.0 / PI;
  Serial.print("AcX: ");
  Serial.print(AcX);
  Serial.print(" | AcY: ");
  Serial.print(AcY);
  Serial.print(" | AcZ: ");
  Serial.print(AcZ);
  Serial.print(" | RollRaw: ");
  Serial.println(rollRaw);

  delay(100);
}
