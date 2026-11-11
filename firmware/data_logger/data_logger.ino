const int TRIG_PIN = 5;
const int ECHO_PIN = 18;
const unsigned long SAMPLE_PERIOD_MS = 60;
const float MAX_DISTANCE_CM = 200.0;

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

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  unsigned long start = millis();
  Serial.println(readDistance());
  while (millis() - start < SAMPLE_PERIOD_MS) {}
}
