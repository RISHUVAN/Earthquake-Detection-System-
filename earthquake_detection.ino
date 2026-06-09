#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

int buzzerPin = 10; // Buzzer connected to D10
int ledPin = 11;    // LED connected to D11

int gyroX, gyroY, gyroZ;
int accelX, accelY, accelZ;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  mpu.initialize();
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  if (mpu.testConnection()) {
    Serial.println("MPU6050 connected!");
  } else {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }
}

void loop() {
  // Get gyroscope and acceleration values
  mpu.getRotation(&gyroX, &gyroY, &gyroZ);
  mpu.getAcceleration(&accelX, &accelY, &accelZ);

  // Print values to Serial Monitor
  Serial.print("GyroX: "); Serial.print(gyroX);
  Serial.print(" | GyroY: "); Serial.print(gyroY);
  Serial.print(" | GyroZ: "); Serial.print(gyroZ);
  Serial.print(" || AccelX: "); Serial.print(accelX);
  Serial.print(" | AccelY: "); Serial.print(accelY);
  Serial.print(" | AccelZ: "); Serial.println(accelZ);

  // Thresholds (tweak for sensitivity)
  bool gyroShake = abs(gyroX) > 1000 || abs(gyroY) > 1000 || abs(gyroZ) > 1000;
  bool accelShake = abs(accelX) > 10000 || abs(accelY) > 10000 || abs(accelZ - 16384) > 10000;

  // 16384 ~ 1g for Z-axis at rest (standing vertically)
  if (gyroShake || accelShake) {
    alertEarthquake();
  } else {
    noTone(buzzerPin);
    digitalWrite(ledPin, LOW);
  }

  delay(200);
}

void alertEarthquake() {
  for (int i = 0; i < 5; i++) {
    tone(buzzerPin, 2800);
    digitalWrite(ledPin, HIGH);
    delay(100);
    noTone(buzzerPin);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
}
