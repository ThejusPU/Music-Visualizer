#define MicSamples (1024*2)
#define ANALOG_MIC A10
#define LED_PIN 10

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
//  long signalAvg = 0, signalMax = 0, signalMin = 1024, t0 = millis();
//    for (int i = 0; i < MicSamples; i++)
//    {
//        int k = analogRead(ANALOG_MIC);
//        signalMin = min(signalMin, k);
//        signalMax = max(signalMax, k);
//        signalAvg += k;
//    }
//    signalAvg /= MicSamples;
// 
    // print
//    Serial.print("Time: " + String(millis() - t0));
//    Serial.print(" Min: " + String(signalMin));
//    Serial.print(" Max: " + String(signalMax));
//    Serial.print(" Avg: " + String(signalAvg));
//    Serial.print(" Span: " + String(signalMax - signalMin));
//    Serial.print(", " + String(signalMax - signalAvg));
//    Serial.print(", " + String(signalAvg - signalMin));
//    Serial.println("");
  int k = analogRead(ANALOG_MIC);
  Serial.println(k);
  if (k == 32) {
    digitalWrite(LED_PIN, LOW);
  }
  else {
    digitalWrite(LED_PIN, HIGH);
  }
}
