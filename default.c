#define speakerPin D3
#define NOTE_Med_DO  523
#define NOTE_Med_SO  784
int i;

#define TRIG_PIN D6
#define ECHO_PIN  D7
const unsigned int MAX_DIST = 23200; // 最大探測距离
const unsigned int MIN_WATER_LEVEL = 100; // 最低水位阈值 (cm)

#define moisture D7
#define pumpPin D8

#define targetMoisture 60 // 目標濕度
#define minMoisture 40 // 最小濕度
#define maxMoisture 90 // 最大濕度

// 澆水量設定
#define dryWaterAmount 100 // 乾燥時澆水量
#define wetWaterAmount 50 // 潮濕時澆水量

void setup() {
  pinMode(speakerPin, OUTPUT);
  pinMode(BT1, INPUT); // 用于触发手动报警 (可选)
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);

  pinMode(moisture, INPUT);
  pinMode(pumpPin, OUTPUT);
}

void loop() {
  // 檢查水位是否低於閾值
  unsigned long t1, t2;
  unsigned long pulse_width;
  float cm;
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  while (digitalRead(ECHO_PIN) == 0);
  t2 = micros();
  while (digitalRead(ECHO_PIN) == 1);
  t1 = micros();
  pulse_width = t2 - t1;
  cm = pulse_width / 58.0;
  if (cm > MAX_DIST || cm < MIN_WATER_LEVEL) {
    playAlarm();
  } else {
    Serial.print("水位: ");
    Serial.print(cm);
    Serial.println("cm");
  }

  // 檢查土壤濕度是否低於目標濕度
  int moistureValue = digitalRead(moisture);
  Serial.println(moistureValue);
  if (moistureValue < targetMoisture) {
    digitalWrite(pumpPin, HIGH);
    Serial.println("Watering...");
    delay(waterAmount);
    digitalWrite(pumpPin, LOW);
  } else {
    Serial.println("Fine");
  }

  delay(5000);
}

void playAlarm() {
  for (i = 0; i < 5; i++) {
    tone(speakerPin, NOTE_Med_DO);
    delay(1000);
    noTone(speakerPin);
    tone(speakerPin, NOTE_Med_SO);
    delay(1000);
    noTone(speakerPin);
  }
  Serial.println("水位低，請加水！");
}
