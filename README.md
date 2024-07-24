# ESP32 FSR Bluetooth Weight Transmitter

This project demonstrates the use of an ESP32 microcontroller to read values from multiple Force Sensitive Resistors (FSRs), calculate the corresponding resistance and weight, and transmit this data via Bluetooth.

## Features
- Read values from 8 FSRs connected to ADC pins of the ESP32.
- Calculate resistance from ADC values.
- Calculate weight from resistance using a polynomial equation.
- Transmit data via Bluetooth to a paired device.

## Components Used
- ESP32 Dev Module
- 8 Force Sensitive Resistors (FSRs)
- Known Resistor (10kΩ)
- BluetoothSerial library

## Connections
- **FSR1_PIN**: GPIO36 (ADC1_CHANNEL_1)
- **FSR2_PIN**: GPIO35 (ADC1_CHANNEL_2)
- **FSR3_PIN**: GPIO14 (ADC2_CHANNEL_3)
- **FSR4_PIN**: GPIO27 (ADC2_CHANNEL_4)
- **FSR5_PIN**: GPIO26 (ADC2_CHANNEL_5)
- **FSR6_PIN**: GPIO25 (ADC2_CHANNEL_6)
- **FSR7_PIN**: GPIO33 (ADC1_CHANNEL_7)
- **FSR8_PIN**: GPIO32 (ADC1_CHANNEL_8)

## Code Overview

### Libraries
- `Arduino.h`
- `Wire.h`
- `BluetoothSerial.h`

### Constants
- **THRESHOLD**: ADC threshold to detect force.
- **VIN**: Input voltage (3.3V).
- **RKNOWN**: Known resistor value (10kΩ).
- **RESISTANCE_THRESHOLD**: Minimum resistance to determine weight.

### Functions
- `calculateResistance(int adcValue)`: Converts ADC value to resistance.
- `calculateWeight(float resistance)`: Converts resistance to weight using a polynomial equation.

### Setup
```cpp
void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test_FSR");
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
}
