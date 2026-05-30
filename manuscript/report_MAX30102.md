# Characterization of MAX30102 Raw Red and Infrared Readings under Temperature and Ambient Light Conditions

## Abstract

This project investigated how conditions affect the raw red and infrared readings from a MAX30102 optical sensor. The goal was not to develop or validate a clinical heart-rate or blood-oxygen monitor. 
Instead, the purpose was to observe how raw optical readings change under controlled experimental conditions. Two main variables were tested: finger temperature and ambient light. 
For the temperature experiment, the same middle finger was measured under normal, cool, and warm conditions. 
For the ambient light experiment, a fixed white paper target was used to separate the effect of light intensity from biological variables such as blood flow. 
The results showed that cooling the finger was associated with lower raw red and IR readings compared with the normal condition. The warm condition produced higher red readings but did not increase IR readings in the same way. 
The white paper light-control experiment showed that non-biological reflective targets can also produce stable MAX30102 raw readings, and that strong external light can substantially shift the raw signal baseline.
Overall, the results suggest that MAX30102 raw readings depend strongly on measurement conditions and should not be interpreted as physiological data without careful environmental control and analysis.

## Introduction
The MAX30102 is an optical sensor commonly used for pulse oximetry and heart-rate-related projects. It emits red and infrared light and measures the intensity of reflected or transmitted light. In a finger measurement, 
the raw signal may contain physiological information because blood volume changes during the cardiac cycle. However, the sensor itself does not directly output “blood” or “heart rate.” It outputs raw optical readings.
So because the sensor is optical, its readings can be affected by several non-physiological factors, including contact quality, finger position, pressure, temperature, and light. 
Therefore, a raw reading from the MAX30102 should be treated as an optical signal first, not automatically as a physiological measurement.

This experiment focused on two questions:
1.How does finger temperature affect MAX30102 raw red and infrared readings?
2.How does ambient light affect MAX30102 raw readings when a non-biological reflective target is used?
The temperature experiment was designed to observe how raw readings change under different finger-temperature conditions. 
The ambient light experiment was designed as a control experiment using white paper instead of a finger. Removing the finger helped isolate the effect of light intensity from biological variables.

## Materials
The experiment used the following materials:
* MAX30102 optical sensor module
* Microcontroller running raw red/IR data-logging firmware
* Computer serial monitor
* Google Sheets for data analysis
* Terumo ET-P265WZ electronic thermometer
* White paper
* Smartphone lux meter app with paper diffuser
* Warm water and cold water for temperature conditions
* Flashlight for the bright-light condition

## Firmware and Data Format
The MAX30102 was programmed to output raw red and infrared readings in CSV format. The firmware did not calculate BPM and did not use the LCD display during formal data collection. Each row contained:
```csv
time_ms,red,ir
```
The `time_ms` column recorded the timestamp in milliseconds. The `red` column recorded the raw red-light reading. The `ir` column recorded the raw infrared reading.
This format was selected because it can be directly imported into Google Sheets or Python for later analysis.

## Methods

### Temperature Experiment

The temperature experiment used the same middle finger under three conditions:

| Condition | Temperature Record | Description                                                                        |
| --------- | -----------------: | ---------------------------------------------------------------------------------- |
| Normal    |             33.5°C | Middle finger at normal room-temperature condition                                 |
| Cool      |            <32.0°C | Finger cooled with cold water, dried before measurement; thermometer displayed “L” |
| Warm      |             40.0°C | Finger warmed with warm water, dried before measurement                            |

The Terumo thermometer used in this experiment has a certain range of measurement beginning at 32.0°C. Under "cool"condition, the thermometer will display “L,” indicating that the temperature was below the measurable range. 
Therefore, the cool condition was treated as a qualitative low-temperature condition rather than an exact numerical temperature.

For each temperature condition, the finger was placed over the MAX30102 sensor under a covered/no-light setup. The same finger was used to reduce variation from different finger sizes or tissue properties. 
The finger was dried before each measurement to avoid the situation that water may change the optical path.
Each recording lasted approximately 60 seconds or longer. The first 10 seconds of each recording were excluded from the summary statistics because the sensor readings were still stabilizing after finger placement.

### Ambient Light Control Experiment

The ambient light experiment was performed without using a finger. Instead, a fixed white paper target was used as a non-biological reflective surface. This helped isolate the effect of external light intensity from biological variables such as blood flow, temperature, and finger pressure.

Three light conditions were tested:

| Condition           | Approximate Light Intensity | Description                                             |
| ------------------- | --------------------------: | ------------------------------------------------------- |
| Covered / near-dark |                   0.301 lux | White paper target under covered low-light condition    |
| Room light          |                    29.5 lux | White paper target under normal room light              |
| Flashlight          |                    4030 lux | White paper target under strong flashlight illumination |

Light intensity was estimated using a smartphone lux meter app with a paper diffuser. These lux values were treated as approximate because the phone sensor was not laboratory-calibrated.

For the white paper light-control experiment, the first 30 seconds were excluded from analysis because the target placement and optical geometry took longer to stabilize than the finger measurements.

## Data Analysis

Data were analyzed in Google Sheets. For each CSV file, the first part of the recording was excluded to avoid the unstable setup period immediately after placement.

For the temperature experiment, the first 10 seconds were excluded. The Google Sheets filtering method used the `time_ms` column. For example, when the data started at row 5,
the mean red value after excluding the first 10 seconds was calculated as:

```excel
=AVERAGE(FILTER(B5:B,A5:A>=A5+10000))
```

The same filtering method was used for the red and IR columns. The following metrics were calculated:

* mean red
* sample standard deviation of red
* range of red
* mean IR
* sample standard deviation of IR
* range of IR

The range was calculated as:

```text
range = max value - min value
```

Sample standard deviation was calculated using `STDEV.S`.

For the white paper ambient light experiment, the same method was used, but the cutoff was 30 seconds instead of 10 seconds.

## Results

### Temperature Experiment Results

The temperature experiment results after excluding the first 10 seconds are shown below.

| Condition | Temperature Record | Mean Red | STD Red | Range Red |  Mean IR | STD IR | Range IR |
| --------- | -----------------: | -------: | ------: | --------: | -------: | -----: | -------: |
| Normal    |             33.5°C | 122492.6 |   892.4 |      4398 | 142923.9 |  876.9 |     5024 |
| Cool      |      <32.0°C / “L” | 120242.3 |   596.2 |      2773 | 134542.5 |  934.6 |     4127 |
| Warm      |             40.0°C | 125841.0 |  1315.4 |     11668 | 133885.2 | 3013.8 |    17254 |

The cool condition produced lower mean red and IR readings compared with the normal condition. The mean red decreased from 122492.6 to 120242.3, and the mean IR decreased from 142923.9 to 134542.5. 
This suggests that cooling the finger was associated with a lower raw optical signal baseline.

The warm condition showed the highest mean red value, 125841.0, but the mean IR value, 133885.2, did not increase compared with the normal condition. 
The warm condition also had the largest standard deviation and range, especially in the IR channel. This suggests that the warm trial was less stable than the normal and cool trials.

### Ambient Light Control Results

The ambient light experiment used white paper as a non-biological reflective target. The first 30 seconds were excluded from analysis because the readings took longer to stabilize.

| Condition           | Approximate Lux | Mean Red | STD Red | Range Red |  Mean IR | STD IR | Range IR |
| ------------------- | --------------: | -------: | ------: | --------: | -------: | -----: | -------: |
| Covered / near-dark |       0.301 lux |  16370.1 |   118.4 |       477 |  21936.2 |  150.8 |      599 |
| Room light          |        29.5 lux |   5368.2 |     7.5 |        55 |   6761.6 |    6.2 |       37 |
| Flashlight          |        4030 lux | 159825.1 |   180.6 |       859 | 172106.9 |  140.7 |      749 |

The room-light white paper condition produced very low and highly stable readings. The standard deviations were small, and the ranges were narrow. T
his suggests that a fixed non-biological reflective target can produce a stable optical baseline.
The flashlight condition produced much higher mean red and IR readings than the covered and room-light conditions. The mean red increased to 159825.1, and the mean IR increased to 172106.9. 
This shows that strong external light can substantially shift the MAX30102 raw optical baseline when a non-biological reflective target is used.
The covered condition produced higher readings than the room-light condition, even though the measured lux was lower. This indicates that the absolute raw reading was not determined by lux alone.
Physical placement, distance, angle, reflection geometry, and how the paper covered the sensor also affected the result.

## Discussion

The temperature experiment suggests that finger temperature may be associated with changes in MAX30102 raw red and infrared readings. The cool condition produced lower red and IR means than the normal condition. 
This may be consistent with changes in finger tissue condition, local circulation, or optical properties after cooling. However, this experiment cannot prove that temperature alone caused the change, 
because finger placement and contact pressure may also have influenced the readings.
The warm condition was more complicated. It produced a higher mean red reading but not a higher mean IR reading. It also had a larger standard deviation and range, especially in the IR channel.
This suggests that the warm trial may have included more contact instability or baseline drift. Therefore, the warm result should be interpreted cautiously.
The ambient light control experiment shows that MAX30102 raw readings can be produced even without a finger. This is important because it separates raw optical readings from physiological interpretation. 
A white paper target does not have blood flow or a pulse, but it still produced measurable red and IR values. Therefore, raw MAX30102 values are not automatically physiological signals.

The flashlight condition strongly increased the raw red and IR baseline compared with the room-light and covered white paper conditions. This supports the idea that external light can act as an optical interference source. 

However, the covered condition had higher readings than the room-light condition, even though its measured lux was lower. This shows that light intensity alone is not the only factor.
Optical geometry, target distance, material placement, and reflection path may also matter.

Put alll in together, these results suggest that MAX30102 measurements are sensitive to experimental conditions. Temperature and ambient light can affect raw readings. 
Therefore, future MAX30102 experiments should carefully control these variables or record them as limitations.


## Limitations

This experiment has several limitations.

First, the temperature experiment was exploratory. The cool condition was recorded as below 32.0°C rather than as an exact temperature because the thermometer displayed “L.” 
Therefore, the cool condition should be interpreted as a qualitative low-temperature condition.

Second, the thermometer measured surface or near-surface finger temperature rather than internal tissue temperature. The temperature values should therefore be treated as approximate.

Third, contact pressure and finger placement were not measured quantitatively. Small differences in finger position or pressure may have affected the raw red and IR readings.

Fourth, the smartphone lux meter app was not a calibrated laboratory instrument. The lux values were useful for approximate comparison but should not be treated as high-precision measurements.

Fifth, the white paper ambient light experiment isolated optical effects but did not represent a real physiological finger measurement. It was useful as a control experiment but cannot directly predict how a finger measurement would behave under every lighting condition.

Finally, only one trial per condition was analyzed here. Additional trials would improve reliability and make the results more statistically robust.

## Conclusion

This project characterized MAX30102 raw red and infrared readings under temperature and ambient light conditions. 
The temperature experiment showed that cooling the finger was associated with lower raw red and IR readings compared with the normal condition. 
The warm condition changed the raw readings as well, but the effect was not identical for red and IR channels and showed greater variability.
The ambient light control experiment showed that a non-biological white paper target can produce stable MAX30102 raw readings. 
It also showed that strong external light can substantially increase the raw red and IR baseline. These results confirm that MAX30102 raw readings are optical signals that depend on experimental conditions.
Overall, the raw red and IR readings from MAX30102  must be interpreted in the context of measurement conditions, including temperature, light exposure, contact quality, and potentially optical geometry.
