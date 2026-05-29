# MAX30102-Signal-Characterization

## Overview

This repository contains an introductory directed research project focused on sensor interfacing, raw signal acquisition, sampling, noise, basic filtering, and data analysis.

The original goal of the project is to characterize how raw red and infrared readings from a MAX30102 sensor change under different measurement conditions. The goal is not to develop a clinical heart-rate monitor. Instead, the project focuses on understanding raw sensor behavior, signal stability, contact quality, and motion-related artifacts.

## Current Progress

The original project focuses on MAX30102 raw red and infrared signal characterization. Since the MAX30102 hardware setup is still being restored, the current completed work focuses on validating MPU6050 roll-angle measurements as a supporting step for future motion-related analysis.

The current progress report is stored in:

* `manuscript/report.md`

In this preliminary experiment, I tested five static tilt setups. The physical reference angles were calculated from measured height and base values, and the sensor readings were compared with MPU6050 `RollRaw` output from the Arduino serial monitor.

The raw roll readings are stored in:

* `data/raw_roll_readings.md`

Setup photos, the MPU6050 circuit diagram, and serial monitor screenshots are stored in:

* `figures/`

The results showed that the MPU6050 readings were stable within each condition, while the error increased at larger tilt angles. This suggests that the MPU6050 can be useful for basic motion or tilt tracking, but the physical setup and sensor alignment still matter.

This MPU6050 validation is treated as a preliminary motion-measurement step, not as a replacement for the broader MAX30102 signal-stability project.

## Project Goals

The broader project goals are:

* Collect raw red and infrared data from a MAX30102 sensor
* Compare raw signal behavior under different contact and motion conditions
* Visualize raw sensor readings over time
* Apply a simple moving average filter
* Summarize signal variability using basic metrics
* Discuss noise, motion artifacts, contact quality, and limitations
* Use motion/orientation information to support future interpretation of sensor stability

## Hardware

| Component                  | Purpose                                                    |
| -------------------------- | ---------------------------------------------------------- |
| Arduino Uno                | Microcontroller used for MPU6050 roll-angle validation     |
| MPU6050 IMU                | Static tilt and orientation measurement                    |
| Particle Photon 2          | Planned microcontroller for MAX30102 raw signal collection |
| MAX30102 heart-rate sensor | Planned source of raw red and infrared PPG readings        |
| USB serial connection      | Data transfer from microcontroller to computer             |
| Computer                   | Data logging, plotting, and analysis                       |

## Completed MPU6050 Validation Experiment

The current completed experiment tested whether the MPU6050 could provide stable and reasonable static roll-angle measurements.

Five tilt setups were tested. For each setup, the physical angle was calculated using measured height and base values:

```text
theta = arctan(height / base)
```

For each physical angle, 10 consecutive `RollRaw` values were recorded from the Arduino serial monitor. The mean was used to estimate the measured angle, and the standard deviation was used to describe measurement stability.

The current results are summarized in:

* `manuscript/report.md`

The raw readings are stored in:

* `data/raw_roll_readings.md`

## Planned MAX30102 Experimental Conditions

After the MAX30102 hardware setup is restored, the raw red and infrared signal will be tested under controlled measurement conditions.

| Condition          | Description                                       | Purpose                           |
| ------------------ | ------------------------------------------------- | --------------------------------- |
| Stable contact     | Finger held steadily on the sensor                | Baseline signal behavior          |
| Light motion       | Finger lightly moved during recording             | Motion artifact observation       |
| Pressure variation | Finger pressure changed slightly during recording | Contact pressure sensitivity      |
| Poor contact       | Finger partially offset from the sensor           | Low-quality measurement condition |

Each condition is planned to be recorded for approximately 60 seconds and repeated twice.

## Planned MAX30102 Data Format

Each CSV file will contain:

```text
time_ms,red,ir
```

## Planned MAX30102 Analysis

The planned MAX30102 analysis will include:

* Raw red and infrared signal plots for each condition
* Comparison of signal behavior across measurement conditions
* A simple moving average filter applied to raw signal data
* Basic signal variability metrics, including mean, standard deviation, and signal range
* Discussion of noise, motion artifacts, contact quality, and system limitations

## Repository Structure

Current repository structure:

## Repository Structure

Current repository structure:

### `data/`

* `raw_roll_readings.md`

### `figures/`

* `Circuit1_MPU.png`
* `Flat.png`
* `Lv1_incline.png`
* `Lv2_incline.png`
* `Lv3_incline.png`
* `Lv4_incline.jpg`
* `serial_0.png`
* `serial_1.png`
* `serial_2.png`
* `serial_3.png`
* `serial_4.png`

### `firmware/`

* `max30102_data_logger.cpp`
* `max30102_lcd_bpm.cpp`
* `mpu6050_roll_validation.ino`

### `manuscript/`

* `report.md`

### `notes/`

* experiment notes and project logs

### Root files

* `.gitignore`
* `README.md`

## Planned Future MAX30102 Files

### `data/`

* `C1_stable_trial1.csv`
* `C1_stable_trial2.csv`
* `C2_motion_trial1.csv`
* `C2_motion_trial2.csv`
* `C3_pressure_trial1.csv`
* `C3_pressure_trial2.csv`
* `C4_poor_contact_trial1.csv`
* `C4_poor_contact_trial2.csv`

### `analysis/`

* `analyze_max30102.py`

### `figures/`

* `fig1_stable_raw_signal.png`
* `fig2_motion_raw_signal.png`
* `fig3_pressure_raw_signal.png`
* `fig4_poor_contact_raw_signal.png`
* `fig5_raw_vs_filtered.png`
* `fig6_signal_variability_summary.png`

### `manuscript/`

* `revised_manuscript.pdf`

### `notes/`

* `experiment_log.md`


## Limitations

This system is not designed or validated as a clinical heart-rate monitor. The results should be interpreted as an introductory characterization of raw sensor behavior under different measurement conditions, not as a biomedical validation study.

The current MPU6050 experiment is also preliminary. It tested static tilt measurements rather than dynamic motion, and the physical angles were constructed manually from height and base measurements. Therefore, small systematic errors may come from sensor alignment, physical setup limitations, or geometric measurement uncertainty.

## Next Steps

The next stage is to restore the MAX30102 hardware setup and continue with raw red and infrared PPG signal collection. Once the MAX30102 setup is available again, the project can return to the planned signal-quality experiment under stable contact, light motion, pressure variation, and poor contact conditions.

The MPU6050 validation may support this next stage by providing a way to measure motion or tilt during PPG signal collection.
