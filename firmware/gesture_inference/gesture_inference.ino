#include <hand-gesture-ultrasonic_inferencing.h>

const int TRIG_PIN = 5;
const int ECHO_PIN = 18;
const unsigned long SAMPLE_PERIOD_MS = 60;
const float MAX_DISTANCE_CM = 200.0;
const int STRIDE = 8;
const float CONFIDENCE_THRESHOLD = 0.7;

float window[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
int filled = 0;
int sinceLast = 0;

float readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  float cm = duration / 58.0;
  if (duration == 0 || cm > MAX_DISTANCE_CM) cm = MAX_DISTANCE_CM;
  return cm;
}

void classify() {
  signal_t signal;
  numpy::signal_from_buffer(window, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);

  ei_impulse_result_t result;
  if (run_classifier(&signal, &result, false) != EI_IMPULSE_OK) return;

  int best = 0;
  for (int i = 1; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
    if (result.classification[i].value > result.classification[best].value) best = i;
  }

  float confidence = result.classification[best].value;
  if (confidence > CONFIDENCE_THRESHOLD) {
    Serial.printf("%s (%.0f%%)  %d ms\n",
                  result.classification[best].label,
                  confidence * 100,
                  result.timing.classification);
  } else {
    Serial.println("uncertain");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  unsigned long start = millis();

  for (int i = 0; i < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - 1; i++) {
    window[i] = window[i + 1];
  }
  window[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - 1] = readDistance();

  if (filled < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) filled++;
  sinceLast++;

  if (filled == EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE && sinceLast >= STRIDE) {
    sinceLast = 0;
    classify();
  }

  while (millis() - start < SAMPLE_PERIOD_MS) {}
}
