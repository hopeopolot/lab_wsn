ESP32 Multi-Sensor Edge Node
1. Introduction

This project involves designing and implementing a multi-sensor edge node using an ESP32 microcontroller. The system collects temperature, humidity, light intensity and motion data using different sensors.

The ESP32 processes the sensor readings and controls an LED and buzzer depending on the conditions detected. The collected data is also stored in a CSV file for further analysis using Python.

This project was carried out as part of the Wireless Sensor Networks laboratory work.

2. Project Objectives

The main objectives of this project are:

To connect different sensors to an ESP32 microcontroller.
To collect temperature, humidity, light and motion readings.
To calibrate the temperature sensor readings.
To apply basic processing to the collected data.
To use sensor readings to control an LED and buzzer.
To analyse the collected data using Python.
3. Hardware Components

The following components were used in the project.

Component	Quantity	Purpose
ESP32 DevKit	1	Main microcontroller
SHT31 Sensor	1	Measures temperature and humidity
LDR	1	Detects light intensity
PIR Sensor	1	Detects motion
LED	1	Provides visual indication
Buzzer	1	Provides sound indication
Resistors	As required	Circuit connections
Breadboard	1	Circuit assembly
Jumper wires	As required	Connecting components
4. Pin Connections

The following GPIO pins were used.

Component	ESP32 Pin
SHT31 SDA	GPIO 21
SHT31 SCL	GPIO 22
PIR Sensor	GPIO 27
LDR	GPIO 34
LED	GPIO 26
Buzzer	GPIO 25
5. How the System Works

The ESP32 receives readings from the connected sensors.

The SHT31 measures temperature and humidity.
The LDR measures the surrounding light level.
The PIR sensor detects movement.
The LED responds to the temperature readings.
The buzzer responds when motion is detected.

The LED operates according to the following temperature conditions:

Temperature Condition	LED Response
Above 30°C	LED stays ON
Below 18°C	LED blinks
Between 18°C and 30°C	LED remains OFF

When motion is detected, the buzzer is activated for approximately two seconds after the last detected movement.

6. Temperature Calibration

Temperature calibration was carried out by comparing the SHT31 readings with a reference thermometer.

The reference temperature used was 28.0°C, while the average raw sensor reading was approximately 30.03°C.

The calibration offset was calculated as:

Calibration offset = Reference temperature − Average raw temperature

Calibration offset = 28.00 − 30.03

Calibration offset = -2.03°C

This offset was used to adjust the raw temperature readings.

7. Data Collection and Analysis

Sensor readings were collected through the ESP32 serial monitor and saved in a CSV file named raw_sensor.csv.

The dataset contains 2,077 observations with the following measurements:

Raw temperature
Calibrated temperature
Humidity
LDR analog values
Motion detection status

Python and Google Colab were used to analyse the collected data.

The analysis includes:

Checking missing and duplicate values.
Calculating mean, minimum, maximum and standard deviation.
Comparing raw and calibrated temperature readings.
Applying a five-sample moving average for analysis.
Examining changes in sensor readings.
Counting motion detection events.
8. Data Visualisation

The following graphs were generated from the collected sensor data:

Temperature against time.
Humidity against time.
Raw temperature against calibrated temperature.
Calibrated temperature against filtered temperature.
LDR readings against time.
PIR motion detection against time.

The graphs help to understand how the sensor readings changed during data collection.

9. Software and Tools

The following software and tools were used:

Arduino IDE – writing and uploading ESP32 code.
Google Colab – Python data analysis.
Python – processing and visualising sensor data.
GitHub – storing and managing project files.
Wokwi – circuit simulation, where applicable.

10. Limitations

Sensor readings may be affected by the surrounding environment.
The LDR readings depend on the amount of light reaching the sensor.
PIR motion detection depends on the sensor's detection range.
The temperature calibration was based on a reference measurement.
The collected dataset does not contain exact timestamps.
11. Conclusion

The project demonstrates how an ESP32 can be used to collect and process data from multiple sensors. It also shows how sensor readings can be used to control simple output devices.

Python was used to analyse and visualise the collected data, while GitHub provides a place to store the project files and document the work.
