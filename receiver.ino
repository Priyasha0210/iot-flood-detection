#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ThingSpeak.h>

#define SS    5
#define RST   14
#define DIO0  4

#define RED_LED    26
#define GREEN_LED  25
#define BUZZER     27

// Replace these placeholders locally. Never publish real credentials.
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

#define BOT_TOKEN "YOUR_TELEGRAM_BOT_TOKEN"
#define CHAT_ID   "YOUR_TELEGRAM_CHAT_ID"

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

WiFiClient clientTS;

unsigned long channelID = 0;  // Replace locally with your ThingSpeak channel ID.
const char* writeAPI = "YOUR_THINGSPEAK_WRITE_API_KEY";

float distance = 0;
bool moderateSent = false;
bool highSent = false;
bool criticalSent = false;

void setup()
{
  Serial.begin(115200);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BUZZER, LOW);

  SPI.begin(18, 19, 23, SS);
  LoRa.setPins(SS, RST, DIO0);

  while (!LoRa.begin(433E6))
  {
    Serial.println("LoRa Failed");
    delay(500);
  }

  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Receiver Ready");

  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nWiFi Connected");

  client.setInsecure();
  ThingSpeak.begin(clientTS);

  bot.sendMessage(CHAT_ID, "Smart Flood Monitoring System Active", "");
}

void loop()
{
  int packetSize = LoRa.parsePacket();

  if (packetSize)
  {
    String received = "";

    while (LoRa.available())
      received += (char)LoRa.read();

    received.trim();

    if (received.length() == 0)
      return;

    distance = received.toFloat();

    if (distance <= 0 || distance > 400)
      return;

    Serial.print("Distance: ");
    Serial.println(distance);

    if (distance > 12)
    {
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BUZZER, LOW);

      delay(150);
      digitalWrite(GREEN_LED, LOW);
      delay(150);

      ThingSpeak.setField(2, 0);

      moderateSent = false;
      highSent = false;
      criticalSent = false;
    }
    else if (distance > 8 && distance <= 12)
    {
      digitalWrite(GREEN_LED, HIGH);
      delay(120);
      digitalWrite(GREEN_LED, LOW);
      delay(120);

      digitalWrite(BUZZER, HIGH);
      delay(150);
      digitalWrite(BUZZER, LOW);

      ThingSpeak.setField(2, 1);

      if (!moderateSent)
      {
        bot.sendMessage(CHAT_ID, "Moderate Water Rise Detected", "");
        moderateSent = true;
      }
    }
    else if (distance > 4 && distance <= 8)
    {
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);
      digitalWrite(BUZZER, HIGH);
      delay(250);

      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, HIGH);
      digitalWrite(BUZZER, LOW);
      delay(250);

      digitalWrite(BUZZER, HIGH);
      delay(400);
      digitalWrite(BUZZER, LOW);

      ThingSpeak.setField(2, 2);

      if (!highSent)
      {
        bot.sendMessage(CHAT_ID, "HIGH WATER LEVEL", "");
        highSent = true;
      }
    }
    else
    {
      for (int i = 0; i < 6; i++)
      {
        digitalWrite(RED_LED, HIGH);
        digitalWrite(BUZZER, HIGH);
        delay(120);

        digitalWrite(RED_LED, LOW);
        delay(40);
      }

      digitalWrite(BUZZER, HIGH);
      delay(1200);
      digitalWrite(BUZZER, LOW);

      ThingSpeak.setField(2, 3);

      if (!criticalSent)
      {
        bot.sendMessage(
          CHAT_ID,
          "CRITICAL FLOOD ALERT\nWater Rising Rapidly!\nImmediate Action Required!",
          ""
        );
        criticalSent = true;
      }
    }

    ThingSpeak.setField(1, distance);
    ThingSpeak.writeFields(channelID, writeAPI);
  }

  delay(10);
}
