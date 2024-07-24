#include <Arduino.h>
#include <Wire.h>
#include <BluetoothSerial.h>

// Define ADC pins for the FSRs
const int FSR1_PIN = 13; // GPIO36 (ADC1_CHANNEL_1)
const int FSR2_PIN = 14; // GPIO35 (ADC1_CHANNEL_2)
const int FSR3_PIN = 27; // GPIO14 (ADC2_CHANNEL_3)
const int FSR4_PIN = 26; // GPIO27 (ADC2_CHANNEL_4)
const int FSR5_PIN = 25; // GPIO26 (ADC2_CHANNEL_5)
const int FSR6_PIN = 33; // GPIO25 (ADC2_CHANNEL_6)
const int FSR7_PIN = 32; // GPIO33 (ADC1_CHANNEL_7)
const int FSR8_PIN = 35; // GPIO32 (ADC1_CHANNEL_8)

// Define a threshold for detecting force
const int THRESHOLD = 50; // Adjust this value based on noise level
const float VIN = 3.3; // Input voltage
const float RKNOWN = 10000; // Known resistor value in ohms (10kΩ)
const float RESISTANCE_THRESHOLD = 10; // Resistance threshold to determine no weight

BluetoothSerial SerialBT;

float calculateResistance(int adcValue) {
  if (adcValue == 0) {
    return INFINITY; // Prevent division by zero
  }
  float Vout = (adcValue / 4095.0) * VIN;
  float RFSR = RKNOWN * (VIN / Vout - 1);
  return RFSR;
}

float calculateWeight(float resistance) {
  if (resistance < RESISTANCE_THRESHOLD) {
    return 0.0; // Return 0 if the resistance is below the threshold
  }

  float weight = 0.0;
  
  // Substitute the polynomial coefficients obtained from MATLAB
  float p0 = -4237.9481;
  float p1 = 5.6627;
  float p2 = -0.0015354;
  float p3 = 1.5763e-07;
  
  // Calculate weight using the polynomial equation
  weight = p0 + p1 * resistance + p2 * pow(resistance, 2) + p3 * pow(resistance, 3);

  // Ensure the weight is not negative
  if (weight < 0) {
    weight = 0.0;
  }
  
  return weight;
}


void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause until serial console opens

  SerialBT.begin("ESP32test_FSR"); // Bluetooth device name
  Serial.println("The device started, now you can pair it with Bluetooth!");

  // Initialize ADC resolution and attenuation
  analogReadResolution(12); // 12-bit ADC resolution
  analogSetAttenuation(ADC_11db); // Set attenuation to 11dB (0-3.6V range)
}

void loop() {
  // Read values from the FSRs
  int fsr1Value = analogRead(FSR1_PIN);
  int fsr2Value = analogRead(FSR2_PIN);
  int fsr3Value = analogRead(FSR3_PIN);
  int fsr4Value = analogRead(FSR4_PIN);
  int fsr5Value = analogRead(FSR5_PIN);
  int fsr6Value = analogRead(FSR6_PIN);
  int fsr7Value = analogRead(FSR7_PIN);
  int fsr8Value = analogRead(FSR8_PIN);

  // Convert ADC readings to resistance
  float fsr1Resistance = fsr1Value > THRESHOLD ? calculateResistance(fsr1Value) : 0;
  float fsr2Resistance = fsr2Value > THRESHOLD ? calculateResistance(fsr2Value) : 0;
  float fsr3Resistance = fsr3Value > THRESHOLD ? calculateResistance(fsr3Value) : 0;
  float fsr4Resistance = fsr4Value > THRESHOLD ? calculateResistance(fsr4Value) : 0;
  float fsr5Resistance = fsr5Value > THRESHOLD ? calculateResistance(fsr5Value) : 0;
  float fsr6Resistance = fsr6Value > THRESHOLD ? calculateResistance(fsr6Value) : 0;
  float fsr7Resistance = fsr7Value > THRESHOLD ? calculateResistance(fsr7Value) : 0;
  float fsr8Resistance = fsr8Value > THRESHOLD ? calculateResistance(fsr8Value) : 0;

  // Calculate weight from resistance and convert to kg
  float fsr1Weight = calculateWeight(fsr1Resistance) / 1000.0;
  float fsr2Weight = calculateWeight(fsr2Resistance) / 1000.0;
  float fsr3Weight = calculateWeight(fsr3Resistance) / 1000.0;
  float fsr4Weight = calculateWeight(fsr4Resistance) / 1000.0;
  float fsr5Weight = calculateWeight(fsr5Resistance) / 1000.0;
  float fsr6Weight = calculateWeight(fsr6Resistance) / 1000.0;
  float fsr7Weight = calculateWeight(fsr7Resistance) / 1000.0;
  float fsr8Weight = calculateWeight(fsr8Resistance) / 1000.0;

  // Prepare the data to be sent via Serial and Bluetooth
  String data = "FSR1: " + String(fsr1Weight, 3) + " kg" +
                ", FSR2: " + String(fsr2Weight, 3) + " kg" +
                ", FSR3: " + String(fsr3Weight, 3) + " kg" +
                ", FSR4: " + String(fsr4Weight, 3) + " kg" +
                ", FSR5: " + String(fsr5Weight, 3) + " kg" +
                ", FSR6: " + String(fsr6Weight, 3) + " kg" +
                ", FSR7: " + String(fsr7Weight, 3) + " kg" +
                ", FSR8: " + String(fsr8Weight, 3) + " kg";

  // Print out the values to Serial
  Serial.println(data);
  // Send the data via Bluetooth
  SerialBT.println(data);

  delay(500); // Delay for readability
}
