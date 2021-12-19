#include <DHT.h>

const unsigned char lightRelayPin = 5;
const unsigned char fanRelayPin = 6;
const unsigned char pumpRelayPin = 7;

const unsigned char temperatureSensorPin = A0;
const unsigned char soilMoistureSensorPin = A1;

#define DHTTYPE DHT22
DHT temperatureSensor(temperatureSensorPin, DHTTYPE);

long auxLlightTime = 0;
long turnOnLightTime = 64800000L;  //18 hours
long turnOffLightTime = 21600000L; //6 hours

void controlActuators();
void controlLight();
void controlFan();
void controlPump();
void answerRaspi();
String convertTimeInMillisToString(long millisec);

void setup()
{
  Serial.begin(9600);

  auxLlightTime = millis();

  pinMode(soilMoistureSensorPin, INPUT);
  temperatureSensor.begin();

  pinMode(lightRelayPin, OUTPUT);
  pinMode(fanRelayPin, OUTPUT);
  pinMode(pumpRelayPin, OUTPUT);

  //turning actuators off
  digitalWrite(fanRelayPin, HIGH);
  digitalWrite(pumpRelayPin, HIGH);
  digitalWrite(lightRelayPin, HIGH);
}

void loop()
{
  controlActuators();
  answerRaspi();
}

void controlActuators()
{
  controlLight();
  controlFan();
  controlPump();
}

void controlLight()
{
  bool isLightTurnedOff = digitalRead(lightRelayPin);

  if ((millis() - auxLlightTime) >= turnOffLightTime && isLightTurnedOff)
  {
    digitalWrite(lightRelayPin, LOW);
    auxLlightTime = millis();
  }

  if ((millis() - auxLlightTime) >= turnOnLightTime && !isLightTurnedOff)
  {
    digitalWrite(lightRelayPin, HIGH);
    auxLlightTime = millis();
  }
}

void controlFan()
{
  float temp = temperatureSensor.readTemperature();

  if (temp > 30)
    digitalWrite(fanRelayPin, LOW); //turn fan on
  else if (temp < 25)
    digitalWrite(fanRelayPin, HIGH); //turn fan off
}

void controlPump()
{
  if (analogRead(soilMoistureSensorPin) > 450)
    digitalWrite(pumpRelayPin, LOW); //turn pump on

  if (analogRead(soilMoistureSensorPin) < 300)
    digitalWrite(pumpRelayPin, HIGH); //turn pump on
}

void answerRaspi()
{
  if (Serial.available())
  {
    switch (Serial.read())
    {
    case 87: //87 = "W"
      bool isPumpTurnedOn = !digitalRead(pumpRelayPin);
      bool isLightTurnedOn = !digitalRead(lightRelayPin);
      bool isFanTurnedOn = !digitalRead(fanRelayPin);
      short soilMoisture = analogRead(soilMoistureSensorPin);
      float temperature = temperatureSensor.readTemperature();
      float humidity = temperatureSensor.readHumidity();

      Serial.print(String(isLightTurnedOn) + '|');
      if (isLightTurnedOn)
        Serial.print(convertTimeInMillisToString(turnOnLightTime - (millis() - auxLlightTime)) + '|');
      else
        Serial.print(convertTimeInMillisToString(turnOffLightTime - (millis() - auxLlightTime)) + '|');
      Serial.print(String(soilMoisture) + '|');
      Serial.print(String(isPumpTurnedOn) + '|');
      Serial.print(String(temperature) + '|');
      Serial.print(String(isFanTurnedOn) + '|');
      Serial.print(String(humidity) + '|');

      Serial.print("\r\n");
      break;

    default:;
    }
  }
}

String convertTimeInMillisToString(long millisec)
{
  String timeString;

  unsigned long aux = millisec / 1000;

  short seconds = aux % 60;
  short minutes = aux / 60 % 60;
  short hours = aux / 3600;

  timeString = String(hours) + ':' + String(minutes) + ':' + String(seconds) + "   ";
  return timeString;
}
