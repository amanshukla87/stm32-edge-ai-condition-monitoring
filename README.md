# STM32-Based Lightweight Edge AI for Multisensor Industrial Condition Monitoring

**BEC 753 – Project - I | 7th Semester Project**

**Project Team**

- **Aman Shukla** — Project Team Lead
- **Govind Shukla** — Project Team Member

---

## Overview

This project focuses on developing a small embedded system for monitoring different operating conditions of an industrial setup using an STM32 microcontroller and multiple sensors.

The system is being developed around the **STM32 NUCLEO-F446RE** and is intended to collect parameters such as temperature, vibration/motion, and electrical measurements. The collected data will be processed by the microcontroller, displayed locally, and used to explore lightweight Edge AI techniques for identifying changes in operating conditions.

The project is being developed step by step. The initial work focuses on understanding the STM32 platform and interfacing individual sensors and peripherals. The later stages will combine the sensor data and move toward data analysis and lightweight on-device inference.

---

## Project Objectives

The main objectives of this project are:

- To develop a multisensor monitoring system using the STM32 NUCLEO-F446RE.
- To interface and collect data from different sensors.
- To monitor parameters relevant to equipment operating conditions.
- To process sensor data locally on the microcontroller.
- To provide local indication of important readings and abnormal conditions.
- To study communication interfaces such as **CAN and RS485**.
- To collect and prepare sensor data for condition analysis.
- To explore a lightweight Edge AI approach suitable for embedded hardware.
- To understand the practical limitations of running data processing and AI inference on a microcontroller.

---

## System Architecture

The planned system follows a simple data flow:

```text
              ┌──────────────────────┐
              │       Sensors        │
              │                      │
              │ Temperature          │
              │ Vibration / Motion   │
              │ Current / Power      │
              └──────────┬───────────┘
                         │
                         ▼
              ┌──────────────────────┐
              │    STM32 F446RE      │
              │                      │
              │ Sensor Acquisition   │
              │ Data Processing       │
              │ Feature Extraction   │
              │ Decision / Analysis  │
              └──────────┬───────────┘
                         │
              ┌──────────┼───────────┐
              │          │           │
              ▼          ▼           ▼
           OLED       Buzzer      Communication
          Display      Alert       CAN / RS485
                         │
                         ▼
                Edge AI / Condition
                     Analysis
```

The final architecture will be updated as the hardware and firmware implementation develops.

---

## Hardware

The project hardware is organized around the STM32 NUCLEO-F446RE development platform and a set of sensing, communication, display, power, and interface components. These components are being used to build and test the complete monitoring system through incremental hardware integration.

| Component | Purpose |
|---|---|
| STM32 NUCLEO-F446RE | Main microcontroller and embedded development platform |
| DS18B20 | Temperature measurement |
| MPU6050 | Accelerometer and gyroscope data acquisition for motion and vibration analysis |
| INA219 | Current, voltage, and power monitoring |
| SN65HVD230 ×2 | CAN bus communication and two-node CAN experiments |
| MAX3485 | RS485 communication interface |
| SSD1306 OLED | Local display of sensor measurements and system information |
| Buzzer | Audible alarm and abnormal-condition indication |
| 7-Segment LED ×2 | Numerical status and parameter indication |
| LM2596 Buck Converter | Regulated power supply for the prototype hardware |
| A3144 Hall-Effect Sensor | Magnetic field and rotational/position sensing |
| N20 6V 100RPM Motor with Encoder | Motor and encoder-based sensing/actuation experiments |
| L298N Motor Driver | Motor control and actuator interface |
| 5V Relay Module | Switching and actuator control |
| Tactile Switches | User input and control interface |
| 120 Ω Resistors | CAN bus termination |
| 4.7 kΩ Resistors | Pull-up and sensor/interface circuitry |
| 100 nF Capacitors | Local supply decoupling and noise reduction |
| 22 pF Capacitor | Timing/oscillator-related hardware support where required |
| Breadboards and Jumper Wires | Hardware prototyping and interconnection |
| USB A–Mini-B Cable | STM32 programming, debugging, and serial development connection |

The Hall-effect sensor, N20 motor with encoder, relay module, L298N motor driver, tactile switches, and passive components are part of the project hardware and will be used during the implementation and testing stages. They provide additional interfaces for studying rotational behaviour, actuator control, switching, user input, signal conditioning, and practical embedded-system integration.

The hardware is therefore not limited to sensor acquisition alone. The project also includes **measurement, signal interfacing, local indication, communication, power regulation, actuator control, and experimental validation**, allowing the STM32 platform to be evaluated as an integrated embedded monitoring controller.

---

## Software and Technologies

The project is being developed using:

- **Embedded C**
- **STM32 NUCLEO-F446RE**
- **STM32CubeIDE / STM32 development tools**
- **GPIO**
- **ADC**
- **I2C**
- **UART**
- **CAN**
- **RS485**
- Sensor interfacing
- Embedded data processing
- Lightweight Edge AI
- Git and GitHub

The exact software tools and libraries will be documented as they are used in the implementation.

---

## Firmware

The firmware is being developed incrementally rather than as one large program.

The initial firmware work involves understanding the STM32 development environment, configuring peripherals, and testing individual hardware modules.

### Current Firmware Work

The firmware development will include:

- STM32 project setup
- GPIO configuration
- Basic peripheral testing
- I2C communication
- Sensor interfacing
- Temperature data acquisition
- Motion/vibration data acquisition
- Current and power measurement
- OLED display interfacing
- Buzzer control
- Communication interface testing

As each part is implemented and tested, the corresponding code will be added to the repository.

### Planned Firmware Development

The later firmware stages will focus on:

- Combining multiple sensor readings
- Periodic data acquisition
- Data filtering and preprocessing
- Feature extraction
- Communication between monitoring nodes
- Condition classification
- Lightweight Edge AI inference

---

## Sensor Data and Condition Monitoring

The project uses multiple types of sensor information because a single parameter may not always provide enough information about the operating condition of equipment.

For example:

- **Temperature** can indicate thermal changes.
- **Vibration/motion data** can provide information about mechanical behaviour.
- **Current and power measurements** can indicate changes in electrical loading.
- **Rotational sensing** can provide additional information about motor or mechanical behaviour.

The goal is to combine these measurements and study whether meaningful operating conditions can be identified from the collected data.

The actual parameters, sampling rates, features, and classification method will be finalized after the initial hardware experiments.

---

## Edge AI

A major part of the project is the exploration of a lightweight Edge AI approach for condition monitoring.

Instead of sending all sensor data to an external computer for every decision, the project investigates whether useful analysis can be performed closer to the sensors on embedded hardware.

The planned workflow is:

```text
Sensor Data
     ↓
Data Collection
     ↓
Preprocessing
     ↓
Feature Extraction
     ↓
Model Development
     ↓
Model Optimization
     ↓
STM32 Deployment
     ↓
On-Device Inference
```

The model and algorithm will be selected based on the actual sensor data and the computational limitations of the STM32 platform.

The Edge AI implementation will be documented only after it has been tested on the target hardware.

---

## Communication

Industrial systems often require reliable communication between different devices or monitoring nodes.

This project therefore includes experiments with:

### CAN

The **SN65HVD230** transceivers will be used to study CAN communication between nodes.

The two CAN modules allow practical experimentation with communication between two devices.

### RS485

The **MAX3485** module will be used to explore RS485-based communication and understand its use in embedded and industrial systems.

The communication section will be expanded after the interfaces are implemented and tested.

---

## Hardware Testing

Testing will be carried out progressively.

The testing process will include:

1. STM32 board setup and basic firmware testing.
2. Individual sensor interfacing.
3. Sensor data verification.
4. OLED display testing.
5. Buzzer and indicator testing.
6. Current and power measurement verification.
7. CAN communication testing.
8. RS485 communication testing.
9. Motor, encoder, relay, and Hall-effect interface testing.
10. Multisensor data acquisition.
11. Edge AI testing on the target hardware.

Actual test observations, measured values, screenshots, and photographs will be added to the repository as the project progresses.

---

## Results

This section will contain the actual results obtained during development.

Planned results include:

- Sensor readings
- Communication test results
- Sampling and processing observations
- Multisensor data
- Graphs and recorded datasets
- Condition classification results
- Edge AI model performance
- On-device inference performance

Results will be added after the corresponding experiments are completed.

---

## Project Status

**Status: Ongoing**

This is the **BEC 753 Project - I** for the 7th semester.

The project is currently under development, with work progressing from individual hardware and peripheral testing toward integrated multisensor monitoring and lightweight Edge AI.

Major milestones will be added to the repository as the corresponding work is completed and tested.

---

## Team

### Aman Shukla
**Project Team Lead**

Responsible for project coordination, embedded system development, hardware integration, firmware development, testing, documentation, and Edge AI-related work.

### Govind Shukla
**Project Team Member**

Contributing to hardware implementation, testing, firmware development, documentation, and other project activities.

Individual contributions may be updated as the project progresses.

---

## Repository Structure

```text
stm32-edge-ai-condition-monitoring/
│
├── README.md
│
├── Firmware/
│
├── Hardware/
│
├── Documentation/
│
└── Results/
```

The repository will grow gradually as actual project work is completed. Additional folders will only be added when they are needed.

---

## Future Scope

Possible future extensions include:

- Improved multisensor data acquisition
- More structured condition datasets
- Additional industrial communication experiments
- Lightweight machine-learning models
- Model optimization for microcontroller deployment
- Real-time condition classification
- Multiple monitoring nodes
- CAN-based sensor networking
- Improved visualization and data logging

The final scope will depend on the results obtained during Project-I and the work planned for the subsequent project stage.

---

## Academic Information

**Course:** BEC 753 – Project - I  
**Semester:** 7th Semester  
**Program:** Bachelor of Technology – Electronics & Communication Engineering  
**Project Type:** Academic Project  
**Status:** Ongoing
