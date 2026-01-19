# DHT22 Data Logger

## Project Overview
The **DHT Logger** is an Arduino Mega 2560–based data logging system designed for **research applications**. It continuously monitors temperature and humidity using three DHT22 sensors and records the data at fixed intervals to an SD card with accurate timestamps provided by a DS3231 Real-Time Clock (RTC).

The system also features a **20x4 LCD display** that shows the current date and time along with real-time readings from all three sensors, allowing users to verify system operation at a glance.

---

## Features
- Uses **3× DHT22 temperature and humidity sensors**
- **Adjustable logging interval** (configured in code)
- Accurate timestamping via **DS3231 RTC module**
- Data storage on **SD card (SPI interface)**
- **SD card error detection** built into the system
- **NaN / sensor read error handling** (invalid DHT readings replaced with `ERR`)
- **20x4 LCD display** for real-time monitoring
- **Serial output for debugging and testing**
- Designed for **long-term research data collection**

---


## Hardware Components
- Arduino Mega 2560
- 3 × DHT22 Temperature & Humidity Sensors
- DS3231 RTC Module
- SPI SD Card Module
- 20x4 Character LCD (with or without I2C backpack)
- SD Card (FAT32 formatted recommended)
- Jumper wires & appropriate pull-up resistors (for DHT22)

---

---------------+
|  DHT22 Sensor 1  |
+------------------+
           |
+------------------+
|  DHT22 Sensor 2  |
+------------------+        +------------------+
           |                |   DS3231 RTC     |
+------------------+        +------------------+
|  DHT22 Sensor 3  |                  |
+------------------+                  |
           |                           |
           +-----------+---------------+
                       |
               +------------------+
               |  Arduino Mega    |
               |     2560         |
               +------------------+
                  |        |        \
                  |        |         \
        +----------------+ |  +------------------+
        |   20x4 LCD     | |  |   SD Card Module |
        +----------------+ |  +------------------+
                           |
                     Serial Monitor
                     (Testing / Debug)
```

---

## High-Level Wiring Overview
> **Note:** Pin numbers may be adjusted in the code if needed.

### DHT22 Sensors
- Each DHT22 data pin connected to a separate digital pin on the Arduino Mega
- VCC → 5V
- GND → GND
- 10kΩ pull-up resistor between VCC and DATA

Example:
- DHT22 #1 → Digital Pin 22
- DHT22 #2 → Digital Pin 24
- DHT22 #3 → Digital Pin 26

### RTC Module (DS3231)
- Uses I2C communication
- SDA → Pin 20 (Mega)
- SCL → Pin 21 (Mega)
- VCC → 5V
- GND → GND

### SD Card Module (SPI)
- CS → Pin 53
- MOSI → Pin 51
- MISO → Pin 50
- SCK → Pin 52
- VCC → 5V (or 3.3V depending on module)
- GND → GND

### 20x4 LCD Display
- If using I2C:
  - SDA → Pin 20
  - SCL → Pin 21
  - VCC → 5V
  - GND → GND

---

## Data Logging Format
Data is saved to the SD card in a **CSV-style format**, making it easy to import into Excel, Python, MATLAB, or other data analysis tools.

### Column Order
```
Timestamp,Temperature1,Temperature2,Temperature3,Humidity1,Humidity2,Humidity3
```

### Sample Log Entry
```
2026-01-18 14:30,25.6,26.1,25.9,61.2,60.8,62.0
```

- **Timestamp** is sourced from the DS3231 RTC
- Temperature values are in **°C**
- Humidity values are in **%RH**

---

## How to Use
1. Assemble the hardware according to the wiring overview
2. Insert a formatted SD card into the SD card module
3. Upload the Arduino sketch to the Mega 2560
4. Set the RTC time (one-time setup)
5. Power the system
6. Verify real-time readings on the LCD display
7. Allow the system to run continuously for data collection

The system will automatically log sensor data every **15 minutes** without user interaction.

---

## Troubleshooting

### LCD Powers On but No Data is Logged
- Verify SD card is properly inserted and formatted (FAT32 recommended)
- Check CS pin definition in the code matches the wiring
- Look for SD card error messages via Serial output

### `ERR` Appears in Logged Data
- Indicates a failed DHT22 sensor reading (NaN detected)
- Check sensor wiring and pull-up resistors
- Ensure sensors are not placed too close to heat sources or airflow extremes

### Incorrect Date or Time
- RTC time may not be set or battery may be depleted
- Replace the CR2032 battery if needed
- Re-run the RTC time-setting sketch

### No Serial Output
- Ensure correct baud rate is selected in Serial Monitor
- Serial output is primarily for testing and debugging

### SD Card Write Failures
- Power cycle the system and reinsert the SD card
- Try a different SD card if errors persist
- Ensure stable power supply during operation

---

## Application Notes
- Suitable for environmental monitoring, laboratory studies, and long-term data collection
- Ensure the SD card is not removed while the system is powered
- Periodically back up logged data for analysis

---

## Future Improvements (Optional)
- Adjustable logging interval
- Error detection for SD card write failures
- Data validation and sensor fault detection
- Serial or USB data export

---

## Client Notes
This project is designed to be **reliable, easy to operate, and maintainable** for research use. Minimal user interaction is required once deployed, and all collected data is stored in a standard, analysis-friendly format.

