# MAX30102-Signal-Characterization

## Overview

This project is an introductory directed research project focused on sensor interfacing, raw signal acquisition, sampling, noise, basic filtering, and data analysis.

The goal is not to develop a clinical heart-rate monitor. Instead, the goal is to characterize how raw red and infrared readings from a MAX30102 sensor change under different measurement conditions.

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
| Particle Photon 2 | Microcontroller for sensor interfacing and serial output |
| MAX30102 heart-rate sensor | Source of raw red and infrared readings |
| USB serial connection | Data transfer from microcontroller to computer |
| Computer | Data logging, plotting, and analysis |

## Experimental Conditions

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

Limitations
This system is not designed or validated as a clinical heart-rate monitor. The results should be interpreted as an introductory characterization of raw sensor behavior under different measurement conditions, not as a biomedical validation study.
