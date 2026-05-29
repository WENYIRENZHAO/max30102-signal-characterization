# MAX30102-Signal-Characterization

## Overview

This project is an introductory directed research project focused on sensor interfacing, raw signal acquisition, sampling, noise, basic filtering, and data analysis.

The goal is not to develop a clinical heart-rate monitor. Instead, the goal is to characterize how raw red and infrared readings from a MAX30102 sensor change under different measurement conditions.

## Current Progress

The original project focuses on MAX30102 raw red and infrared signal characterization. Since the MAX30102 hardware setup is still being restored, the current completed work focuses on validating the MPU6050 roll-angle measurement as a supporting step for future motion-related analysis.

The current report is available here:

- `manuscript/report.md`

In this preliminary experiment, I tested five static tilt setups, calculated the physical reference angles from measured height and base values, and compared them with MPU6050 `RollRaw` readings from the Arduino serial monitor. The results showed that the readings were stable within each condition, while the error increased at larger tilt angles. This suggests that the MPU6050 can be useful for basic motion or tilt tracking, but the physical setup and alignment still matter.

## Project Goals

- Collect raw red and infrared data from a MAX30102 sensor
- Compare signal behavior under different contact and motion conditions
- Visualize raw sensor readings over time
- Apply a simple moving average filter
- Summarize signal variability using basic metrics
- Discuss noise, motion artifacts, contact quality, and limitations

## Hardware
| Component | Purpose |
|---|---|
| Particle Photon 2 | Microcontroller for MAX30102 raw signal collection |
| Arduino Uno | Microcontroller used for MPU6050 roll-angle validation |
| MAX30102 heart-rate sensor | Source of raw red and infrared readings |
| MPU6050 IMU | Static tilt and motion/orientation measurement |
| USB serial connection | Data transfer from microcontroller to computer |
| Computer | Data logging, plotting, and analysis |


## ## Planned MAX30102 Experimental Conditions

The MAX30102 raw signal will be tested under four controlled measurement conditions:

| Condition | Description | Purpose |
|---|---|---|
| Stable contact | Finger held steadily on the sensor | Baseline signal behavior |
| Light motion | Finger lightly moved during recording | Motion artifact observation |
| Pressure variation | Finger pressure changed slightly during recording | Contact pressure sensitivity |
| Poor contact | Finger partially offset from the sensor | Low-quality measurement condition |

Each condition will be recorded for approximately 60 seconds and repeated twice.

## Data Format

Each CSV file will contain:

```text
time_ms, red, ir
```
## Planned Analysis
The analysis will include:
Raw red and infrared signal plots for each condition
Comparison of signal behavior across measurement conditions
A simple moving average filter applied to raw signal data
Basic signal variability metrics, including standard deviation and signal range
Discussion of noise, motion artifacts, contact quality, and system limitations

## Repository Structure
current:
firmware/
    max30102_data_logger.cpp
    max30102_lcd_bpm.cpp
    mpu6050_roll_validation.ino

data/
    raw_roll_readings.md

figures/
    Flat.png
    Lv1_incline.png
    Lv2_incline.png
    Lv3_incline.png
    Lv4_incline.jpg
    serial_0.png
    serial_1.png
    serial_2.png
    serial_3.png
    serial_4.png

manuscript/
    report.md
Expecting:
```text
directed-research-max30102-characterization/
firmware
    max30102_data_logger.cpp

data/
C1_stable_trial1.csv
C1_stable_trial2.csv
C2_motion_trial1.csv
C2_motion_trial2.csv
C3_pressure_trial1.csv
C3_pressure_trial2.csv
C4_poor_contact_trial1.csv
C4_poor_contact_trial2.csv

analysis/
analyze_max30102.py

igures/
fig1_stable_raw_signal.png
fig2_motion_raw_signal.png
fig3_pressure_raw_signal.png
fig4_poor_contact_raw_signal.png
fig5_raw_vs_filtered.png
fig6_signal_variability_summary.png

manuscript/
revised_manuscript.pdf

notes/
experiment_log.md
```

## Limitations
This system is not designed or validated as a clinical heart-rate monitor. The results should be interpreted as an introductory characterization of raw sensor behavior under different measurement conditions, not as a biomedical validation study.
