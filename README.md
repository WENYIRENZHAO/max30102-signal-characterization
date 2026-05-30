# MAX30102 Signal Characterization and MPU6050 Motion Validation

## Overview

This repository documents two small sensor experiments completed for an introductory directed research project.
The first experiment used an MPU6050 sensor to test static roll-angle measurements. The goal was to see whether the sensor could give stable readings when placed at different physical tilt angles.
The second experiment used a MAX30102 optical sensor to collect raw red and infrared readings. The goal was to understand how raw MAX30102 readings change under different experimental conditions including finger temperature and ambient light.
Overall, this project focuses on basic sensor setup, raw data collection, signal stability, and simple data analysis.

## Completed Experiments

### 1. MPU6050 Roll-Angle Validation

The MPU6050 experiment tested five static tilt setups. For each setup, the physical angle was calculated from the measured height and base length using:

$$\theta = arctan\frac{height}{base}$$

For each physical angle, serial monitor readings were recorded from the MPU6050. The readings were then compared with the calculated physical reference angle.

Main finding:

The MPU6050 readings were fairly stable within each condition, but the error became larger at higher tilt angles. This suggests that the MPU6050 can be useful for basic tilt measurement, but the physical setup and sensor alignment still matter.

### 2. MAX30102 Raw Red/IR Signal Experiment

The MAX30102 experiment collected raw red and infrared sensor readings in CSV format.

The sensor was tested under two types of conditions:

* different finger temperatures
* different ambient light conditions using a white paper target

The MAX30102 was used only as a raw optical sensor. The data should not be interpreted as clinical heart-rate, blood oxygen, or blood-flow measurements.

## Main Finding

The experiments showed that raw sensor readings can change depending on the measurement setup.

For the MAX30102, finger temperature, ambient light, contact condition, paper placement, and optical geometry all affected the raw red and infrared readings. This means that raw MAX30102 values should be analyzed carefully instead of being treated as direct physiological measurements.

## MAX30102 Temperature Experiment

The temperature experiment tested the same middle finger under three temperature conditions:

| Condition |                  Temperature Record | Description                                                |
| --------- | ----------------------------------: | ---------------------------------------------------------- |
| Normal    |                              33.5°C | Middle finger at normal room-temperature condition         |
| Cool      | <32.0°C / thermometer displayed “L” | Finger cooled with cold water and dried before measurement |
| Warm      |                              40.0°C | Finger warmed with warm water and dried before measurement |

The thermometer used in this experiment could measure temperatures starting at 32.0°C. For the cool condition, the thermometer displayed “L,” meaning the finger temperature was below the measurable range. Therefore, the cool condition was treated as a qualitative low-temperature condition instead of an exact temperature measurement.

Each condition was recorded using the MAX30102 raw data logger. The first 10 seconds of each recording were excluded from analysis because the readings were still stabilizing after finger placement.

Summary finding:

Cooling the finger was associated with lower raw red and IR readings compared with the normal condition. The warm condition showed higher red readings, but the IR readings did not increase in the same way. This suggests that temperature may affect raw optical readings, but contact stability, finger position, pressure, and optical geometry also matter.

## MAX30102 Ambient Light Control Experiment

The ambient light experiment tested how light conditions affected raw MAX30102 readings.

To separate light effects from biological variables, a fixed white paper target was used instead of a finger. This helped remove variables such as blood flow, finger temperature, and finger pressure.

Three approximate ambient light conditions were tested:

| Condition           | Approximate Light Intensity | Description                                             |
| ------------------- | --------------------------: | ------------------------------------------------------- |
| Covered / near-dark |                   0.301 lux | White paper target under covered low-light condition    |
| Room light          |                    29.5 lux | White paper target under normal room light              |
| Flashlight          |                    4030 lux | White paper target under strong flashlight illumination |

Light intensity was estimated using a smartphone lux meter app with a paper diffuser. The lux values were treated as approximate because the phone sensor was not a calibrated laboratory light meter.

The first 30 seconds of each white-paper recording were excluded from analysis because the paper placement and optical setup took longer to stabilize.

Summary finding:

The white paper control showed that non-biological objects can also produce stable MAX30102 raw readings. Strong flashlight illumination greatly increased the raw red and IR baseline. However, the absolute reading was not determined by lux alone. Paper distance, angle, placement, and optical geometry also affected the readings.

## Data Format

MAX30102 data were collected in CSV format:

```text
time_ms,red,ir
```

The columns represent:

* `time_ms`: timestamp in milliseconds
* `red`: raw red-light reading from the MAX30102
* `ir`: raw infrared reading from the MAX30102

This format was used because it can be directly imported into Google Sheets, Excel, or other analysis tools.

## Data Analysis

Data were analyzed using Google Sheets.

For the temperature experiment, the first 10 seconds were excluded from each file.

```excel
=AVERAGE(FILTER(B5:B,A5:A>=A5+10000))
```

For the white paper ambient light experiment, the first 30 seconds were excluded. 

```excel
=AVERAGE(FILTER(B7:B,A7:A>=A7+30000))
```

The following summary metrics were calculated for both red and IR readings:

* mean
* sample standard deviation
* range

The range was calculated as:

```text
range = max value - min value
```

Sample standard deviation was calculated using `STDEV.S`.

## Summary Results

### Temperature Experiment

| Condition | Temperature Record | Mean Red | STD Red | Range Red |  Mean IR | STD IR | Range IR |
| --------- | -----------------: | -------: | ------: | --------: | -------: | -----: | -------: |
| Normal    |             33.5°C | 122492.6 |   892.4 |      4398 | 142923.9 |  876.9 |     5024 |
| Cool      |      <32.0°C / “L” | 120242.3 |   596.2 |      2773 | 134542.5 |  934.6 |     4127 |
| Warm      |             40.0°C | 125841.0 |  1315.4 |     11668 | 133885.2 | 3013.8 |    17254 |

### White Paper Ambient Light Control Experiment

| Condition           | Approximate Lux | Mean Red | STD Red | Range Red |  Mean IR | STD IR | Range IR |
| ------------------- | --------------: | -------: | ------: | --------: | -------: | -----: | -------: |
| Covered / near-dark |       0.301 lux |  16370.1 |   118.4 |       477 |  21936.2 |  150.8 |      599 |
| Room light          |        29.5 lux |   5368.2 |     7.5 |        55 |   6761.6 |    6.2 |       37 |
| Flashlight          |        4030 lux | 159825.1 |   180.6 |       859 | 172106.9 |  140.7 |      749 |

## Hardware

| Component                      | Purpose                                                               |
| ------------------------------ | --------------------------------------------------------------------- |
| Arduino Uno                    | Microcontroller used for MPU6050 roll-angle validation                |
| MPU6050 IMU                    | Static tilt and orientation measurement                               |
| Particle Photon 2              | Microcontroller used for MAX30102 raw red/IR data collection          |
| MAX30102 optical sensor module | Source of raw red and infrared readings                               |
| USB serial connection          | Data transfer from microcontroller to computer                        |
| Computer                       | Data logging, spreadsheet analysis, and report writing                |
| Terumo thermometer             | Approximate finger temperature measurement                            |
| Smartphone lux meter app       | Approximate ambient light measurement                                 |
| White paper                    | Non-biological reflective target for ambient light control experiment |

## Firmware

The repository includes firmware for both sensor experiments.

### MPU6050

* `mpu6050_roll_validation.ino`

This file was used for static roll-angle validation.

### MAX30102

* `max30102_data_logger.cpp`

This file was used for formal raw red/IR data collection. It outputs clean CSV data in the format:

```text
time_ms,red,ir
```

* `max30102_lcd_bpm.cpp`

This file may be kept as an older debug or reference version. It displays or estimates BPM-related values, but it was not used as the main source of formal raw red/IR experimental data.

## Repository Structure

### `data/`

Raw experimental data files.

Includes:

* MPU6050 roll-angle readings
* MAX30102 temperature experiment CSV files
* MAX30102 white paper ambient light CSV files

Example files:

* `raw_roll_readings.md`
* `T1_normal_33p5C_no_light_trial1.csv`
* `T2_cool_below32C_no_light_trial1.csv`
* `T3_warm_40p0C_no_light_trial1.csv`
* `L1_whitepaper_covered_0p301lux_trial1.csv`
* `L2_whitepaper_room_29p5lux_trial1.csv`
* `L3_whitepaper_flashlight_4030lux_trial1.csv`

### `figures/`

Experimental setup images, circuit diagrams, serial monitor screenshots, thermometer photos, and condition photos.

Example files:

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

Microcontroller code for sensor data collection.

Includes:

* `mpu6050_roll_validation.ino`
* `max30102_data_logger.cpp`
* `max30102_lcd_bpm.cpp`

### `manuscript/`

Written report files.

Includes:

* `report.md`

### `notes/`

Experiment logs, condition descriptions, and analysis notes.

Includes:

* `experiment_log.md`

### `analysis/`

Optional analysis scripts or analysis documentation.

Includes:

* `analyze_max30102.py`
* optional notes describing Google Sheets formulas or future Python analysis

## Limitations

This project is preliminary and exploratory.

The MAX30102 results should not be interpreted as clinical heart-rate, blood oxygen, or blood-flow measurements. The sensor was used only to record raw optical red and IR readings.

The temperature experiment used approximate surface finger temperature measurements. The cool condition was below the thermometer’s measurable range and was therefore treated qualitatively.

The ambient light values were measured using a smartphone lux meter app with a paper diffuser, not a calibrated laboratory light meter.

Finger pressure, exact placement, paper distance, sensor angle, and optical geometry were not perfectly controlled.

The white paper ambient light experiment isolated optical effects but did not represent a real physiological finger measurement. It was useful as a control experiment, but it cannot directly predict how a finger measurement would behave under every lighting condition.

The MPU6050 experiment was also preliminary. It tested static tilt measurements rather than dynamic motion, and the physical angles were constructed manually from height and base measurements. Therefore, small systematic errors may come from sensor alignment, physical setup limitations, or geometric measurement uncertainty.

Only one trial per condition was analyzed for the current MAX30102 experiments. Additional repeated trials would improve reliability.

## Next Steps

Future work could include:

* repeating each condition for multiple trials
* plotting red and IR readings over time
* applying moving average filtering
* testing stable contact, light motion, pressure variation, and poor contact
* comparing finger-contact light experiments with white paper control experiments
* using MPU6050 motion/orientation data to support interpretation of MAX30102 signal stability
* developing Python-based analysis scripts to automate mean, standard deviation, range, and plotting
* improving the physical setup to better control sensor distance, contact pressure, and optical geometry

## Conclusion

This repository documents two related sensor experiments.

The MPU6050 experiment showed that static roll-angle readings can be stable, but the result depends on alignment and physical setup.

The MAX30102 experiments showed that raw red and infrared readings are affected by finger temperature, ambient light, and optical geometry.

Overall, the project shows that raw sensor data should be interpreted carefully. For the MAX30102, raw red and IR readings are optical signals, not automatic physiological measurements.
