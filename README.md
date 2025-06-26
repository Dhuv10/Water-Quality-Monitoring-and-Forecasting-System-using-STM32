# Water-Quality-Monitoring-and-Forecasting-System-using-STM32
STM32-based water quality monitoring system with ESP32 cloud integration for real-time IoT.
💧 STM32-Based Water Quality Monitoring and Forecasting System (IoT)

An embedded IoT system built using the **STM32F429I-DISC1** board and **ESP32 Wi-Fi module** to monitor real-time water quality parameters like pH, Dissolved Oxygen (DO), turbidity, and temperature, and send the data to a cloud dashboard for visualization and forecasting.

---

## 📌 Overview

This project combines embedded sensor integration and wireless communication to build a scalable water quality monitoring system. Data is collected, processed on STM32, and sent to the cloud via ESP32 for real-time monitoring and alerts.

---

## 🧰 Technologies Used

- **Microcontroller**: STM32F429I-DISC1
- **Wireless Module**: ESP32 WiFi
- **Sensors**: pH, Dissolved Oxygen (DO), Turbidity, Temperature (DS18B20)
- **Languages**: Embedded C (STM32CubeIDE), Arduino C (ESP32)
- **Protocols**: UART, ADC, MQTT
- **Cloud Services**: ThingSpeak / Firebase / Grafana

---

## 🧠 Features

- Sensor fusion for 4 key water quality parameters
- Periodic sampling and threshold-based anomaly detection
- Real-time cloud transmission using ESP32 WiFi
- Supports remote access and historical data visualization
- Modular design — easy to scale or add more sensors

---

## 📂 Folder Structure

| Folder       | Contents                                     |
|--------------|----------------------------------------------|
| `firmware/`  | STM32 code for reading sensors               |
| `esp32/`     | Code for data transmission via WiFi/MQTT     |
| `schematics/`| Circuit wiring and pin layout                |
| `images/`    | Photos of setup and cloud dashboard          |
| `results/`   | Screenshots and CSV sample data              |
| `docs/`      | Calibration notes, future improvements       |

---

## 🚀 How to Run

### 1. Upload STM32 Code
- Open `firmware/main.c` in STM32CubeIDE
- Configure ADC channels and UART for sensor readings

### 2. Upload ESP32 Code
- Use Arduino IDE for `esp32/wifi_upload.ino`
- Connect ESP32 via UART to STM32
- Add your WiFi and MQTT/ThingSpeak credentials

### 3. Connect Sensors
- pH sensor to ADC1 Channel
- DO sensor to ADC2
- Turbidity sensor to ADC3
- DS18B20 to Digital GPIO

---

## 📈 Sample Output

- pH: 7.3
- DO: 5.6 mg/L
- Turbidity: 12 NTU
- Temp: 26.1°C

---

## 📌 Future Work

- Add SMS alert integration
- Train ML model for predictive analytics
- Build low-power version with deep sleep
