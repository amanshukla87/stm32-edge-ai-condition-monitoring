# Live Hardware Data Integration Plan

**Project:** STM32-Based Multisensor Industrial Condition Monitoring, Anomaly Detection, and Predictive Maintenance Using TinyML  
**Status:** Planned — hardware-to-website live data path has not yet been verified.

## Objective

Extend the existing website dashboard so it can display measurements collected by the STM32 NUCLEO-F446RE. Preserve the current interface and clearly distinguish simulated values from verified hardware readings.

## Proposed Architecture

```text
Sensors
  |
  v
STM32 NUCLEO-F446RE
  | UART serial messages
  v
ESP32 Wi-Fi gateway
  | Wi-Fi / MQTT or HTTPS
  v
Data service or MQTT broker
  | WebSocket, API, or another supported client connection
  v
Existing website dashboard
```

The ESP32 is a proposed gateway, not a requirement. A computer reading STM32 USB serial or a Raspberry Pi gateway could serve a similar role. The chosen route should be confirmed before implementing the live connection.

## Planned Data Flow

1. Read available sensors on the STM32 and validate the measurements.
2. Send a clearly defined serial message from STM32 to ESP32 over UART.
3. Have the ESP32 parse the message, connect to the configured Wi-Fi network, and forward the measurements to the selected service.
4. Have the website subscribe to or fetch data from that service using a suitable client interface.
5. Update dashboard values and graphs only when valid incoming data is received.
6. Show connection status and the time of the latest valid measurement. Mark stale or missing data instead of silently retaining it as live.
7. Test the end-to-end path against serial logs and known sensor readings before labelling any dashboard panel as live.

## Initial Measurements to Integrate

Integrate only sensors that are actually connected and returning verified data. Candidate measurements include:

- **DS18B20:** temperature in °C.
- **INA219:** bus voltage in V, current in A, and power in W.
- **MPU6050:** accelerometer and gyroscope readings; derived tilt or vibration features only after their calculation and units are validated.
- **Motor / encoder:** RPM and pulse counts after the motor and encoder integration is completed and tested.
- **CAN / RS-485:** protocol status and received values only after those interfaces are implemented and verified.

## Suggested Message Format

The following is an illustrative JSON payload for planning only; it is not an implemented firmware protocol:

```json
{
  "device_id": "stm32-condition-monitor",
  "sequence": 1,
  "temperature_c": 32.4,
  "voltage_v": 12.46,
  "current_a": 1.32,
  "power_w": 16.43
}
```

Use actual measurements and a consistent schema in the final implementation. Include a sequence number or timestamp so the receiver can detect stale or repeated updates. Do not send example values and label them as measured hardware data.

## Website Requirements

- Preserve the existing dashboard design and current navigation.
- Keep the current demo/simulation mode available for UI development.
- Add a clear data-source indicator such as **DEMO MODE**, **LIVE**, **STALE**, or **DISCONNECTED**.
- Switch to live status only after valid data is received.
- Append received measurements to the trend history; do not generate random values while claiming live mode.
- Display units, input validation, and the latest successful update time.
- Handle Wi-Fi, broker, server, or serial disconnections gracefully.

## Security and Deployment Notes

- Do not commit Wi-Fi passwords, MQTT credentials, API tokens, or private keys to this repository or frontend JavaScript.
- Use authenticated access and encrypted connections for remotely accessible services.
- A public static website needs a reachable data service; GitHub Pages alone does not act as a live telemetry backend.
- If the dashboard is accessed from another network, verify service hosting, firewall rules, TLS, authentication, and browser connection requirements.
- Use a private or appropriately secured broker for real hardware data; avoid relying on a public test broker for a deployed monitoring system.

## Implementation Stages

1. **Define protocol:** settle UART baud rate, message format, units, and error handling.
2. **Verify STM32 output:** confirm sensor values and serial messages in a terminal.
3. **Verify ESP32 gateway:** receive serial messages and forward test measurements over Wi-Fi.
4. **Choose data transport:** configure MQTT with a broker and browser-compatible access, or build an authenticated web API.
5. **Connect frontend:** replace simulated updates with validated incoming data when live mode is enabled.
6. **Test failure cases:** disconnect sensor, UART, Wi-Fi, and server; confirm stale/disconnected states appear.
7. **Validate results:** compare dashboard readings with the STM32 serial output and instruments where appropriate.

## Current Status

This document records the planned architecture only. It does not mean that the ESP32 firmware, broker/API, or live website connection has already been implemented. The dashboard must continue to identify its existing values as simulated until the full data path has been tested.
