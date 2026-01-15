#define echopin 2
#define trigpin 4
long duration,distance;
void setup()
{
  Serial.begin(115200);
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
}
void loop()
{
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  duration= pulseIn(echopin, HIGH);
  distance= duration/58.2;
  String disp= String(distance);

  Serial.print("Distance: ");
  Serial.print(disp);
  Serial.println(" cm");
  delay(10);
}