# STM32-Based Lightweight Edge AI for Multisensor Industrial Condition Monitoring

**BEC 753 – Project - I | 7th Semester Project**

**Project Team**
- **Aman Shukla** — Project Team Lead
- **Govind Shukla** — Project Team Member

## Overview

This project focuses on developing a multisensor embedded monitoring system around the **STM32 NUCLEO-F446RE**. The system combines temperature, motion/vibration, and electrical measurements with local display, alerts, and planned CAN/RS485 communication.

The project is being developed incrementally, starting with individual sensor and peripheral bring-up before moving toward multisensor data processing and lightweight Edge AI.

## Current Hardware Bring-Up

Current hardware integration includes:
- **DS18B20** temperature sensor
- **INA219** current/voltage/power monitor
- **MPU6050** accelerometer and gyroscope
- **SSD1306** I2C OLED
- Buzzer and status indicators
- STM32 NUCLEO-F446RE

The MPU6050 was also checked independently with an Arduino UNO after an earlier STM32 test showed no valid sensor response. The Arduino test confirmed the sensor responding at I2C address **0x68** and returning accelerometer, gyroscope, and temperature data. This provides a known-good reference for the next STM32 integration retest.

## Current Firmware Work

Firmware is being developed incrementally using Embedded C. Current work includes STM32 peripheral setup, sensor interfacing, I2C communication, display output, and hardware debugging.

The repository documents tested hardware and firmware progressively rather than presenting untested features as completed.

## Hardware

| Component | Purpose |
|---|---|
| STM32 NUCLEO-F446RE | Main microcontroller |
| DS18B20 | Temperature measurement |
| MPU6050 | Motion and vibration data |
| INA219 | Current, voltage, and power measurement |
| SSD1306 OLED | Local display |
| SN65HVD230 ×2 | CAN experiments |
| MAX3485 | RS485 interface |
| A3144 Hall sensor | Rotational/position sensing |
| N20 6V 100RPM motor with encoder | Motor/encoder experiments |
| L298N | Motor control |
| Relay module | Switching |
| Buzzer | Audible indication |
| LM2596 | Regulated power supply |

## MPU6050 Interface

The tested sensor responds at the standard I2C address **0x68**.

Detailed bring-up notes are documented in `firmware/MPU6050.md`.

## Planned Edge AI Workflow

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

Edge AI deployment remains a development target and will be documented after validation on the target hardware.

## Communication

Planned communication experiments include **CAN** using SN65HVD230 transceivers and **RS485** using MAX3485.

## Project Status

**Status: Ongoing**

Current work is focused on hardware bring-up, sensor verification, firmware development, and integration. Multisensor condition analysis and Edge AI are subsequent stages.

## Repository Structure

```text
stm32-edge-ai-condition-monitoring/
├── README.md
├── firmware/
│   └── MPU6050.md
└── images/
```

## Academic Information

**Course:** BEC 753 – Project - I  
**Semester:** 7th Semester  
**Program:** Bachelor of Technology – Electronics Engineering  
**Project Type:** Academic Project  
**Status:** Ongoing
