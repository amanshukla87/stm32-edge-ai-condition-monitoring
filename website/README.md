# Project Website

This folder contains the website and dashboard work for **STM32-Based Multisensor Industrial Condition Monitoring, Anomaly Detection, and Predictive Maintenance Using TinyML**.

The goal is to build a clear, useful interface for presenting sensor readings, motor and system status, measurement trends, and possible anomaly indicators as the project develops.

## Technologies and Tools

- **HTML** — page structure and content.
- **CSS** — layout, styling, and responsive design.
- **JavaScript** — dashboard interactions and, as development progresses, handling and displaying data.
- **Figma** — for planning the interface, creating wireframes, and exploring dashboard layouts and visual design.
- **Grafana** — a possible option for monitoring dashboards and visualising time-series data; it has not yet been confirmed as part of the implementation.

The tools listed here describe the current development approach and options being considered. They do not mean that every tool has already been integrated into the working website.

## Current Status

The website is under development. The current `index.html` is a basic placeholder while the layout and dashboard features are being prepared.

Any sensor values, charts, status indicators, anomaly alerts, or predictive-maintenance outputs used during interface development must be treated as **simulated/demo data** unless they are explicitly connected to the hardware and verified. They are not evidence of live measurements or validated diagnostic results.

## Folder Structure

- `index.html` — main entry page for the website.
- `code/` — website source code and supporting notes.
- `images/` — website screenshots, design references, and project images.
- `assets/` — additional website assets as needed.
- `docs/` — approved project documents or supporting material intended to be linked from the website.

## Planned Development

The next steps are to develop the page layout and styling, add dashboard interactions, and organise the interface around the project's sensors and monitoring functions. Figma can help plan the interface before or during implementation, while Grafana may be evaluated if it suits the data-visualisation requirements.

Later, the website may be connected to the STM32-based monitoring system through a suitable data-transfer method. The exact communication approach will depend on what is implemented and tested. Any anomaly-detection or TinyML features will also need evaluation with collected data before their outputs can be described as validated.

**Live hardware integration, confirmed Grafana integration, and model validation are not claimed as completed features.** This README will be updated as the work progresses.
