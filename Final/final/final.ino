void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("The tracker is up and running !");
}

void loop() {
  // put your main code here, to run repeatedly:
  double lati = 12.97244100;
  double longi = 80.12604500;

  Serial.println(millis());
}
