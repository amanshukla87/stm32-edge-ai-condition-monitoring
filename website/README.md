# Project Website — STM32-Based Multisensor Industrial Condition Monitoring

This folder contains the website dashboard being developed for the **STM32-Based Multisensor Industrial Condition Monitoring, Anomaly Detection, and Predictive Maintenance Using TinyML** project.

The aim is to provide a clear interface for viewing sensor measurements, monitoring motor and system conditions, displaying trends, and presenting diagnostic or anomaly indicators.

## Current Status

The website is under development using **HTML, CSS, and JavaScript**. Additional tools, such as **Grafana**, may be evaluated or integrated if they fit the final system design.

At this stage, sensor readings, charts, status indicators, anomaly alerts, and predictive-maintenance outputs shown in the dashboard are **simulated/demo data**. They are for interface development and are not live measurements from the STM32 hardware or validated diagnostic results.

## Folder Structure

- `index.html` — main website entry page.
- `code/` — website source code and supporting notes.
- `images/` — website screenshots, design references, and project images.
- `assets/` — other website assets as needed.
- `docs/` — approved project documents or supporting material intended to be linked from the website.

## Planned Hardware Integration

Once the dashboard and embedded firmware are ready, the plan is to connect the website to the physical monitoring system. The intended workflow is to collect measurements through the STM32 firmware, transfer available data through a suitable communication interface, and display real readings and status information.

Anomaly-detection and TinyML/predictive-maintenance features will need testing with collected data before their outputs can be presented as validated results. The exact communication method and any additional dashboard platform will depend on implementation and testing. **Live hardware integration and model validation are planned work, not completed features.**

## Development Notes

Firmware and hardware development are maintained separately in the repository. This README will be updated as website features are implemented and tested.

**Important:** Until hardware integration and validation are complete, all displayed measurements and diagnostic outputs should be treated as demonstration data.