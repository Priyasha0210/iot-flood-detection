#include <SPI.h>
#include <LoRa.h>

#define SS    4
#define RST   14
#define DIO0  2

#define trigPin 27
#define echoPin 26

void setup()
{
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  SPI.begin(18, 19, 23, SS);
  LoRa.setPins(SS, RST, DIO0);

  while (!LoRa.begin(433E6))
  {
    Serial.println("LoRa Failed");
    delay(500);
  }

  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Transmitter Ready");
}

void loop()
{
  long duration;
  float distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);
  distance = duration * 0.034 / 2.0;

  if (distance <= 0 || distance > 22)
  {
    delay(300);
    return;
  }

  LoRa.beginPacket();
  LoRa.print(distance, 2);
  LoRa.endPacket();

  Serial.print("Distance Sent: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
}
