# Project Website — STM32-Based Multisensor Industrial Condition Monitoring

This folder contains the website dashboard being developed for the **STM32-Based Multisensor Industrial Condition Monitoring, Anomaly Detection, and Predictive Maintenance Using TinyML** project.

The aim is to provide a clear interface for viewing sensor measurements, monitoring motor and system conditions, displaying trends, and presenting diagnostic or anomaly indicators.

## Current Status

The website is under development. The dashboard layout and visual elements are being built with **HTML, CSS, and JavaScript**. Additional tools, such as **Grafana**, may be evaluated or integrated where they fit the final system design.

At this stage, any sensor readings, charts, status indicators, anomaly alerts, or predictive-maintenance outputs shown in the website are **simulated/demo data**. They are used to develop and test the interface; they are **not live measurements from the STM32 hardware**, and they should not be treated as validated diagnostic results.

## Planned Hardware Integration

After the dashboard and embedded firmware have been developed and tested, the plan is to connect the website to the physical monitoring system. The intended workflow is to:

- Collect measurements from the supported sensors through the STM32 firmware.
- Transfer available measurements and system status to the dashboard through a suitable communication interface.
- Display real sensor readings, motor-condition information, trends, and alerts when the hardware and data link are working.
- Evaluate anomaly-detection and TinyML/predictive-maintenance features with collected data before presenting them as validated results.

The exact communication method and the role of any additional dashboard platform will depend on implementation and testing. Live data integration and model validation are **planned work, not completed features**.

## Folder Structure

- `index.html` — current website entry page.
- `assets/` — website-specific images, icons, stylesheets, and scripts as the site grows.
- `docs/` — approved project documents or supporting material intended to be linked from the website.

## Development Notes

The website is part of the wider embedded project. Firmware and hardware development are maintained separately in the repository. As implementation progresses, this README will be updated to reflect the features that are actually completed and tested.

**Important:** Until hardware integration and validation are complete, all displayed measurements and diagnostic outputs must be understood as demonstration data.
