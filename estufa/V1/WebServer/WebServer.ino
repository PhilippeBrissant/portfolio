#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 177);
EthernetServer server(80);

//seta os pinos do LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

const short luzRelay = 5;
const short ventliadorRelay = 6;
const short bombaRelay = 7;

const short temperatureSensorPin = A0;
const short sensorUmd = A1;

#define DHTTYPE DHT22
DHT sensorTemp(temperatureSensorPin, DHTTYPE);

//seta variaveis auxiliares
long auxDelayLuz = 0;
long tempoLIGLuz = 43200000L;
long tempoDESLIGLuz = 43200000L;

//string que armazena o http request
char request[30];
//indice do request
int index = 0;

void drawOnLCD();
void ligaEDesligaBomba();
void LigaEDesligaLuz();
String ConvertTimeInMillisToString(long millisec);
void treatRequest();
//void SDlogger();

void setup()
{
  //INICIALIZA LCD
  lcd.init();
  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Carregando...");

  //INICIALIZA SERIAL
  Serial.begin(9600);
  while (!Serial)
  {
  }

  //inicializa Ethernet shield/Web Server
  Serial.print("Initializing SERVER...");
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print(Ethernet.localIP());

  lcd.setCursor(0, 0);
  lcd.print("SERVER:                         ");

  lcd.setCursor(0, 1);
  lcd.print(Ethernet.localIP());
  delay(1000);
  lcd.clear();

  //Seta os pinos
  pinMode(sensorUmd, INPUT);
  sensorTemp.begin();

  pinMode(luzRelay, OUTPUT);
  pinMode(ventliadorRelay, OUTPUT);
  pinMode(bombaRelay, OUTPUT);

  //Seta variaveis auxiliares
  bool luzligada = true;
  auxDelayLuz = millis();

  digitalWrite(ventliadorRelay, HIGH);
  digitalWrite(bombaRelay, HIGH);
  if (!luzligada)
    digitalWrite(luzRelay, HIGH);
  else
    digitalWrite(luzRelay, LOW);
  lcd.clear();
}

void loop()
{
  drawOnLCD();
  ligaEDesligaBomba();
  LigaEDesligaLuz();
  ligaDesligaVentilador();
  treatRequest();
}

/*Mostra no LCD  S:25.68 A:59.10
                 L:1 V:1 B:0 T:34C
*/
void drawOnLCD()
{
  if (millis() % 200 > 0)
  {
    lcd.setCursor(0, 0);
    lcd.print("S:");
    lcd.print((float)analogRead(sensorUmd) * 100 / 1024);
    lcd.print(" A:");
    lcd.print(sensorTemp.readHumidity());
    lcd.setCursor(0, 1);
    lcd.print("L:");
    lcd.print(!digitalRead(luzRelay));
    lcd.print(" V:");
    lcd.print(!digitalRead(ventliadorRelay));
    lcd.print(" B:");
    lcd.print(!digitalRead(bombaRelay));
    lcd.print(" T:"); //11
    lcd.print(sensorTemp.readTemperature());
    lcd.print("C");
  }
}

void ligaDesligaVentilador()
{
  float temp = sensorTemp.readTemperature();
  if (temp > 30)
    digitalWrite(ventliadorRelay, LOW); // liga o ventilador
  else if (temp < 25)
    digitalWrite(ventliadorRelay, HIGH); // desliga o ventilador
}

//Baseado no sensor de umidade, liga ou desliga a bomba
void ligaEDesligaBomba()
{

  if (analogRead(sensorUmd) > 700)
  {
    digitalWrite(bombaRelay, LOW);
  }

  if (analogRead(sensorUmd) < 400)
  {
    digitalWrite(bombaRelay, HIGH);
  }
}

//Baseado no tempo, liga ou desliga a luz
void LigaEDesligaLuz()
{
  //Seta variaveis auxiliares
  bool luzDesligada = digitalRead(luzRelay);

  //LIGA a luz
  if ((millis() - auxDelayLuz) >= tempoDESLIGLuz && luzDesligada)
  {
    digitalWrite(luzRelay, LOW);
    auxDelayLuz = millis();
  }

  //DESLIGA a luz
  if ((millis() - auxDelayLuz) >= tempoLIGLuz && !luzDesligada)
  {
    digitalWrite(luzRelay, HIGH);
    auxDelayLuz = millis();
  }
}

//Trata os requests http
void treatRequest()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client)
  {
    boolean currentLineIsBlank = true;
    while (client.connected())
    {
      if (client.available())
      {
        //armazena o request na string
        char c = client.read();
        Serial.print(c);

        if (index < 20)
        {
          Serial.print(c);
          request[index] = c;
          index++;
        }

        //Verifica qual tipo de request e retorna a resposta
        if (c == '\n' && currentLineIsBlank)
        {
          //Seta variaveis auxiliares
          bool bombaLigada = !digitalRead(bombaRelay);
          bool luzLigada = !digitalRead(luzRelay);
          bool ventiladorLigado = !digitalRead(ventliadorRelay);

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/css");
          client.println("Connection: close");
          client.println("Access-Control-Allow-Origin: *");
          client.println();

          if (String(request).indexOf(F("/ajax_inputs")) > -1) //retorna a leitura dos sensores e dispositivos
          {

            //retorna luz ligada ou desligada
            client.print(String(luzLigada) + '|');
            Serial.print(String(luzLigada) + '|');

            //retorna quanto tempo falta p ligar ou desligar
            if (luzLigada)
            {
              client.print(ConvertTimeInMillisToString(tempoLIGLuz - (millis() - auxDelayLuz)) + '|');
              Serial.print(ConvertTimeInMillisToString(tempoLIGLuz - (millis() - auxDelayLuz)) + '|');
            }
            else
            {
              client.print(ConvertTimeInMillisToString(tempoDESLIGLuz - (millis() - auxDelayLuz)) + '|');
              Serial.print(ConvertTimeInMillisToString(tempoDESLIGLuz - (millis() - auxDelayLuz)) + '|');
            }

            //retorna solo umido ou seco
            client.print(String(digitalRead(sensorUmd)) + '|');
            Serial.print(String(digitalRead(sensorUmd)) + '|');

            //retorna bomba lig/deslig
            client.print(String(bombaLigada) + '|');
            Serial.print(String(bombaLigada) + '|');

            // //retorna temperatura
            // client.print(String(analogRead(sensorTemp) * 0.107526882) + '|');
            // Serial.print(String(analogRead(sensorTemp) * 0.107526882) + '|');

            //retorna temperatura
            client.print(String(sensorTemp.readTemperature()) + '|');
            Serial.print(String(sensorTemp.readTemperature()) + '|');

            //retorna ventilador lig/deslig
            client.print(String(ventiladorLigado) + '|');
            Serial.print(String(ventiladorLigado) + '|');
          }

          index = 0;

          break;
        }
        if (c == '\n')
          currentLineIsBlank = true;

        else if (c != '\r')
          currentLineIsBlank = false;
      }
    }
    // give the web browser time to receive the data
    delay(100);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

//Retorna uma string hh:mm:ss, dado um long
String ConvertTimeInMillisToString(long millisec)
{

  String retorno;

  unsigned long aux = millisec / 1000;

  short segundo = aux % 60;
  short minuto = aux / 60 % 60;
  short hora = aux / 3600;

  retorno = String(hora < 10 ? "0" + String(hora) : hora) + ':' + String(minuto < 10 ? "0" + String(minuto) : minuto) + ':' + String(segundo < 10 ? "0" + String(segundo) : segundo) + "   ";

  return retorno;
}
