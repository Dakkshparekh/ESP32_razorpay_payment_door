# RazorPay Payment Operated Door Prototype

## Project Overview

This project demonstrates a prototype system where a door mechanism is controlled through RazorPay payments. When a payment is made via RazorPay, the payment event is forwarded through a webhook pipeline (using Pipedream) to Adafruit IO MQTT feed. An ESP32 subscribes to this feed in real-time and triggers a servo motor to open or close the door accordingly. Optionally, a thermal printer may be integrated to print payment receipts.

This solution bridges digital payment events with physical IoT hardware control, showcasing a unique use case for automated access control systems.

---

## Key Features

- **Real-time IoT Control:** ESP32-based system that listens for payments via MQTT.
- **RazorPay Integration:** Payment events routed via Pipedream webhook into Adafruit IO.
- **Adafruit IO MQTT Broker:** Secure and reliable feed for payment data delivery.
- **Servo Motor Actuation:** Door opens and closes based on payment receipt.
- **Thermal Printer Support:** (Optional) Print payment receipts on the spot.
- **Robust Connectivity:** Auto reconnect for WiFi and MQTT to maintain uptime.

---

## Hardware Components

| Component       | Description                            | Notes                            |
|-----------------|------------------------------------|---------------------------------|
| ESP32 Board     | Main microcontroller                 | Use any ESP32 development board |
| Servo Motor     | To actuate the door                  | Connected to GPIO 4              |
| Thermal Printer | (Optional) Print payment receipts   | Connect to Serial2 (TX/RX)      |
| Power Supply    | 3.3V or 5V as required               | Supply sufficient current       |
| Connecting Wires| For all connections                  |                                 |

---

## Software & Libraries

- **Arduino ESP32 Core**
- **Libraries:**
  - `WiFi.h` — WiFi connection management
  - `ESP32Servo.h` — Servo motor control
  - `Adafruit_MQTT.h` and `Adafruit_MQTT_Client.h` — MQTT client for Adafruit IO
  - `Adafruit_Thermal.h` — (Optional) Thermal printer control

---

## System Architecture & Workflow

1. **Payment Gateway (RazorPay):**
   - User makes payment via RazorPay.
   - RazorPay triggers webhook events on completed payments.

2. **Webhook Processing (Pipedream):**
   - Listens to RazorPay payment webhook.
   - Extracts relevant data (payment amount, id, etc.).
   - Forwards this data to Adafruit IO MQTT feed (`amount` feed).

3. **MQTT Broker (Adafruit IO):**
   - Hosts the MQTT feed subscribed by the ESP32.
   - Delivers payment data to subscribing IoT devices.

4. **ESP32 Device:**
   - Connects to WiFi.
   - Subscribes to the MQTT payment feed.
   - Upon receiving payment data, executes servo motor movement to open/close door.
   - Optionally triggers printing of receipt on a thermal printer.

---

## Hardware Connections

| Wire/Signal        | ESP32 Pin | Peripheral           |
|--------------------|-----------|---------------------|
| Servo Signal       | GPIO 4    | Servo Motor          |
| Thermal Printer RX | GPIO 16*  | Thermal Printer RX   |
| Thermal Printer TX | GPIO 17*  | Thermal Printer TX   |
| Power (3.3V/5V)    | 3.3V/5V   | Servo, Printer       |
| Ground             | GND       | Common Ground        |

*Adjust pins for Serial2 as necessary in code and physical wiring.

---

## Getting Started

### 1. Set up RazorPay and Pipedream Pipeline

- Create RazorPay account & configure webhook URLs.
- Create a Pipedream workflow:
  - Receive RazorPay webhook events.
  - Transform and forward payment data to the Adafruit IO MQTT feed.

### 2. Create Adafruit IO Account

- Sign up at [Adafruit IO](https://io.adafruit.com).
- Create a new feed (e.g., `amount`).
- Retrieve your **AIO Key** and **Username**.

### 3. Configure and Upload ESP32 Code

- Update WiFi credentials (`WLAN_SSID` and `WLAN_PASS`) in `main.cpp`.
- Update Adafruit IO username and key (`AIO_USERNAME`, `AIO_KEY`).
- Connect hardware (servo to GPIO 4, thermal printer if available).
- Upload code to ESP32 via Arduino IDE:

#define WLAN_SSID "YOUR_WIFI_SSID"
#define WLAN_PASS "YOUR_WIFI_PASSWORD"

#define AIO_USERNAME "YOUR_ADAFRUIT_USERNAME"
#define AIO_KEY "YOUR_ADAFRUIT_KEY"


### 4. Test the System

- Make a test RazorPay payment.
- ESP32 should receive data via MQTT.
- Servo motor actuates door open/close.
- Optionally, receipt prints on thermal printer.
- Use Serial Monitor to debug and monitor connections.

---

## Code Snippet (Servo Operation)

if (subscription == &amount) {
Serial.println("Payment received! Operating door...");

// Open door (servo 0° to 90°)
for (pos = 0; pos <= 90; pos += 5) {
myservo.write(pos);
delay(15);
}
delay(5000); // Door open hold time

// Close door (servo 90° to 0°)
for (pos = 90; pos >= 0; pos -= 5) {
myservo.write(pos);
delay(15);
}
delay(5000);
}

---

## Future Improvements

- Secure MQTT connection using TLS/SSL.
- Add user authentication to control door access.
- Implement door position sensors for feedback.
- Expand to multiple doors with unique identifiers.
- Complete thermal receipt printing integration.
- Develop web/mobile dashboard to monitor payments & door status in real-time.

---

## Troubleshooting & Tips

- Ensure ESP32 is in range of WiFi with a strong signal.
- Double-check credentials to avoid MQTT connection errors.
- Check servo wiring and power supply for correct operation.
- Use Serial Monitor for debugging WiFi and MQTT connectivity.
- Verify Pipedream and Adafruit IO MQTT data flows correctly.

---

## Contact & Contributions

For questions, suggestions, or collaboration, feel free to open an issue or reach out directly!
