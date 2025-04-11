void setup() {
  // put your setup code here, to run once:
  pinMode(D1,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(D1,1);
  delay(100);
  digitalWrite(D1,0);
  delay(100);

}
