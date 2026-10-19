# Project Website

This folder contains the website interface for the **STM32-Based Multisensor Industrial Condition Monitoring, Anomaly Detection, and Predictive Maintenance Using TinyML** project.

The dashboard is being developed to bring the project's sensor information and monitoring features into one place. It includes sections for the overall dashboard, sensor monitoring, and anomaly-detection concepts.

## Website Screenshots

The screenshots below show the current website running locally during development. They document the interface as it looks at this stage.

### 1. Dashboard Overview

![Dashboard overview](images/website-dashboard-overview.jpeg)

This view gives an overall look at the monitoring dashboard and its main status panels.

### 2. Sensor Monitoring

![Sensor monitoring dashboard](images/website-sensor-monitoring.jpeg)

This view focuses on the sensor readings and monitoring panels used to demonstrate how information from different parts of the system can be presented.

### 3. Anomaly Detection

![Anomaly detection dashboard](images/website-anomaly-detection.jpeg)

This view shows the interface for presenting anomaly-related information as the detection workflow develops.

## Technologies and Tools

- **HTML** structures the pages and dashboard content.
- **CSS** controls the layout, styling, and responsive appearance.
- **JavaScript** adds interactive dashboard behaviour.
- **Figma** may be used to plan and refine interface layouts.
- **Grafana** is an option being considered for future data visualisation; it is not currently claimed as an integrated feature.

## Current Development Status

The website is a work in progress. The screenshots show the interface running locally, not a completed connection to the physical monitoring hardware.

**Important:** Sensor values, charts, status indicators, and anomaly-related outputs shown in the current dashboard are simulated/demo data unless a feature is specifically connected to the hardware and verified. They should not be interpreted as live sensor measurements or validated TinyML predictions. Hardware communication, data handling, and model outputs will be documented more precisely as they are implemented and tested.

## Planned Live Hardware Data Integration

The next planned stage is to connect the STM32 NUCLEO-F446RE to the website through an ESP32 Wi-Fi gateway. The ESP32 is a planned addition, not a currently completed or tested connection.

Proposed data path:

1. The STM32 reads available sensors and formats measurements into a serial message.
2. The ESP32 receives those messages over UART and forwards them over Wi-Fi.
3. A suitable data service, such as an MQTT broker or web API, makes the readings available to website clients.
4. The website updates sensor cards, trend graphs, and connection indicators from received measurements.

The ESP32 is not the only possible solution. A computer connected to the STM32 over USB serial, or a Raspberry Pi gateway, could also forward the data. The ESP32 route is being considered for wireless monitoring.

**Until the end-to-end connection is implemented and tested, the website remains in demonstration mode.** Live values, data storage, authentication, and any remote access must be configured and verified before claiming live monitoring.

See [Live Hardware Data Integration Plan](docs/LIVE_HARDWARE_DATA_INTEGRATION.md) for the proposed architecture and implementation stages.

## Folder Structure

- `index.html` — main website entry page.
- `code/` — dashboard source code and related notes.
- `images/` — website screenshots and project images.
- `assets/` — additional website resources, as needed.
- `docs/` — project documents intended to be linked from the website, including the live-data integration plan.

## Next Steps

I'll keep improving the dashboard layout and features, then connect it to the STM32 system once the communication method is working and tested. Anomaly-detection and TinyML results will be reported after testing them with actual collected data.

I'll keep this README updated as the website and hardware integration progress.
