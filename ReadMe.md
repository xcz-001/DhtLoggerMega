# DHT22 Data Logger

## Project Overview
The **DHT Logger** is an Arduino Mega 2560–based data logging system designed for **research applications**. It continuously monitors temperature and humidity using three DHT22 sensors and records the data at configurable intervals to an SD card with accurate timestamps provided by a DS3231 Real-Time Clock (RTC).

The system also features a **20x4 LCD display** that shows the current date and time along with real-time readings from all three sensors, allowing users to verify system operation at a glance.

---

## Features
- Uses **3× DHT22 temperature and humidity sensors**
- **Adjustable logging interval** (`DataLogIntervalMinutes` in code)
- Accurate timestamping via **DS3231 RTC module**
- Data storage on **SD card (SPI interface)**
- **SD card error detection** built into the system
- **NaN / sensor read error handling** (invalid DHT readings replaced with `ERR`)
- **20x4 LCD display** for real-time monitoring
- **Non-blocking sensor reads and LCD updates** (smooth operation)
- **Button-triggered new file creation** without restarting the system
- **Serial output for debugging and testing**
- Designed for **long-term research data collection**

---

## Hardware Components
- Arduino Mega 2560
<img src=images/mega.png width=500>
*(All pins referenced in code: DHTs: 3,4,5 | Button: 2 | SD CS: 53 | LCD I2C: SDA/SCL pins 20/21)*

- 3 × DHT22 Temperature & Humidity Sensors
<img src=images/DHT22.png height=120% width=500>
*(DHT1: Pin 3, DHT2: Pin 4, DHT3: Pin 5)*

- DS3231 RTC Module
<img src=images/rtc.png height=120% width=500>
*(SDA → 20, SCL → 21, VCC → 5V, GND → GND)*

- SPI SD Card Module (FAT32 formatted recommended)
<img src=images/sd.png height=120% width=500>
*(CS → 53, MOSI → 51, MISO → 50, SCK → 52, VCC → 5V, GND → GND)*

- 20x4 Character LCD (with I2C backpack)
<img src=images/lcd.png height=120% width=500>
*(SDA → 20, SCL → 21, VCC → 5V, GND → GND)*

- 5.5 x 2.1 DC Barrel Jack
<img src=images/Fjack.png height=120% width=500>

- Push Button
<img src=images/button.png height=120% width=500>
*(Digital Pin 2, external pulldown resistor)*

---

## High-Level Wiring Overview
> **Note:** Pin numbers are configurable in the code.

### DHT22 Sensors
- Each DHT22 data pin connected to a separate digital pin on the Arduino Mega
- VCC → 5V
- GND → GND

### RTC Module (DS3231)
- Uses I2C communication
- SDA → Pin 20
- SCL → Pin 21
- VCC → 5V
- GND → GND

### SD Card Module (SPI)
- CS → Pin 53
- MOSI → Pin 51
- MISO → Pin 50
- SCK → Pin 52
- VCC → 5V (or 3.3V depending on module)
- GND → GND

### 20x4 LCD Display (I2C)
- SDA → Pin 20
- SCL → Pin 21
- VCC → 5V
- GND → GND

### Push Button
- Connected to Digital Pin 2
- Pulled low externally (external pulldown)
- Used to create a new data file without restarting

---

## Data Logging Format
Data is saved to the SD card in a **CSV-style format**, making it easy to import into Excel, Python, MATLAB, or other data analysis tools.

### Column Order
    Timestamp,DHT1_Temp,DHT1_Hum,DHT2_Temp,DHT2_Hum,DHT3_Temp,DHT3_Hum

### Sample Log Entry
    2026-01-19 12:15:00,32.4,65.4,31.7,64.6,32.6,64.7

- **Timestamp** is sourced from the DS3231 RTC
- Temperature values are in **°C**
- Humidity values are in **%RH**
- Invalid readings are logged as `ERR`

---

## How to Use
1. Plug in 5v power adapter to the DC jack.
2. Insert a formatted SD card into the SD card module
3. Upload the Arduino sketch to the Mega 2560(optional)
7. The system will automatically log sensor data every **`DataLogIntervalMinutes`** without user interaction
8. Press the button to create a new CSV file manually if needed

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
- Add dynamic adjustment of logging interval via serial or UI
- Error detection for SD card write failures
- Data validation and sensor fault detection
- Export logged data via USB or wireless connection

---

## Client Notes
This project is designed to be **reliable, easy to operate, and maintainable** for research use. Minimal user interaction is required once deployed, and all collected data is stored in a standard, analysis-friendly format.
