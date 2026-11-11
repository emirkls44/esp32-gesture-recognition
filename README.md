# Hand Gesture Recognition with an Ultrasonic Sensor

An ESP32 recognizes hand gestures in front of an HC-SR04 ultrasonic sensor. The machine learning model runs on the ESP32 itself, with no internet connection.

## Demo
https://github.com/user-attachments/assets/66f5b6e4-879a-483d-b0d4-b1c4ec40b5fc


## What it does

- Recognizes three classes: idle, push (hand moving toward or away from the sensor) and wave.
- The model was trained in Edge Impulse on data I recorded myself.
- One prediction takes about 2 ms on the ESP32.

## How it works

The ESP32 reads the distance every 60 ms. The last 2 seconds (33 readings) are fed to a small 1D CNN every half second. The model is quantized to int8 so it stays small and fast.

## Hardware

ESP32 and HC-SR04. VCC to VIN, GND to GND, TRIG to GPIO 5, ECHO to GPIO 18. The ECHO pin outputs 5V, so it goes through a voltage divider (1k and 2k) before reaching the ESP32.

## Notes

- Readings are taken at a fixed rate. Without this the sample rate changed with the distance and the model learned the wrong thing.
- A dense network could not tell the gestures apart. Switching to a 1D CNN raised the accuracy a lot.
- Moving toward and moving away looked too similar for this sensor, so they were merged into one push class. Final accuracy is about 88%.

## Files

- `firmware/data_logger`: sends distance readings over serial for data collection
- `firmware/gesture_inference`: runs the model on the ESP32
- `tools/record.py`: saves serial readings into 10 second CSV files
- `data`: the recorded dataset
