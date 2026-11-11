# Hand Gesture Recognition on ESP32

An ESP32 recognizes hand gestures in front of an HC-SR04 ultrasonic sensor. The model runs on the ESP32 itself, no internet needed.

https://github.com/user-attachments/assets/66f5b6e4-879a-483d-b0d4-b1c4ec40b5fc

It detects three gestures: idle, push and wave. I recorded the data myself and trained a small 1D CNN in Edge Impulse. Accuracy is about 88% and one prediction takes 2 ms.

## Wiring

VCC to VIN, GND to GND, TRIG to GPIO 5, ECHO to GPIO 18 through a voltage divider (1k and 2k), because ECHO outputs 5V.

## Files

- `firmware/data_logger`: sends distance readings for data collection
- `firmware/gesture_inference`: runs the model on the ESP32
- `tools/record.py`: saves the readings into CSV files
- `data`: the dataset
