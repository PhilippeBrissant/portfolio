#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Adafruit_BME280.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "YOUR-SSID"
#define WIFI_PASSWORD "YOUR-WIFI-PASSWOR"

/* 2. Define the Firebase project host name and API Key */
#define FIREBASE_HOST "greenhousev2-786f6.firebaseapp.com"
#define API_KEY "AIzaSjhdOSINJDHoisjd112uMwq4JctA1jZjfQgDCI"

/* 3. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "USER-EMAIL"
#define USER_PASSWORD "USER-PASSWORD"


//Variáveis para requisição NTP
WiFiUDP ntpUDP;
// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionally you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient relogioClient(ntpUDP, "europe.pool.ntp.org", -10800, 60000);

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson json;

//Atuadores
byte pumpPin = 14;
byte lampPin = 12;
byte fanPin = 13;

//Sensores
Adafruit_BME280 sensorTemp;
byte pinoSensorUmidadeSolo = A0;

//Variável auxiliar para contar o tempo decorrido desde o início do funcionamento
long contadorMillis;

//Representação dos estados dos atuadores e leitura dos sensores
typedef struct dados {
  int bomba = 0;
  int luz = 0;
  int ventilador = 0;  

  float temperatura = 0;
  float umidade_ar = 0;
  int umidade_solo = 0;
}Dados;

typedef struct config_fabrica {
  int funcionamento = 1; //1 = Automático, 0 = Manual;
  
  int ligaLuz = 6;
  int desligaLuz = 0;
  
  int ligaBomba = 500;
  int desligaBomba = 300;

  float ligaVentilador = 29;
  float desligaVentilador = 25;
}Config;

//Funções
void setaRelogio(); //Instancia o relógio, acessado via NTP
int pegaHora();
double pegaTimeStamp();
void setaOsPinos(); //Seta os pinos e inicializa os sensores
void conectaWIFI();
void inicializaFirebase();
void enviaDados(); //Envia os dados ao Firebase Realtime Database
Dados *leDados(); //Lê os sensores e os estados dos atuadores e modifica a struct Dados
void realtime(Dados *d); //Envia os dados para o nó 'realtime'
void relatorio(Dados *d); //Envia os dados para o nó 'relatorio'
void funcionaAutomaticoOuManual(); //Executa as tarefas de acordo com a configuração
Config *leConfig(); //Lê o nó config e modifica a struct Config
void respondeComandoRemoto(); //Se estiver configurado para manual, executará os comandos enviados
void rodaAutomatico(Config *c); //Executa as tarefas conforme os parâmetros da struct Config
void ligaEDesligaBomba(Config *c);
void LigaEDesligaLuz(Config *c);
void ligaDesligaVentilador(Config *c);
void resetaConfig(); //Reseta a struct Config para valores "de fábrica"

void setup(){
  Serial.begin(115200);
  setaRelogio();
  setaOsPinos();
  conectaWIFI();
  inicializaFirebase();
}

void loop(){
  enviaDados();
  funcionaAutomaticoOuManual();
}

//Pega o horário via NTP
void setaRelogio(){
  relogioClient.begin();
}

int pegaHora(){
  relogioClient.update();
  return relogioClient.getHours();
}

double pegaTimeStamp(){
  relogioClient.update();
  return relogioClient.getEpochTime();  
}

//Seta os pinos e inicializa os sensores
void setaOsPinos(){
  //Atuadores  
  pinMode(pumpPin, OUTPUT);
  pinMode(lampPin, OUTPUT);
  pinMode(fanPin, OUTPUT);

  //Sensores  
  sensorTemp.begin();
  pinMode(pinoSensorUmidadeSolo, INPUT);
}

void conectaWIFI(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void inicializaFirebase(){  
//  Serial.println("Inicializando Firebase");
  /* Assign the project host and api key (required) */
  config.host = FIREBASE_HOST;
  config.api_key = API_KEY;
  
  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  
  //Initialize the library with the Firebase authen and config.
  Firebase.begin(&config, &auth);

  //Optional, Reconnect WiFi if lost connection.
  Firebase.reconnectWiFi(true);
  
  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  fbdo.setBSSLBufferSize(1024, 1024);
  
  //Set the size of HTTP response buffers in the case where we want to work with large data.
  fbdo.setResponseSize(1024);
  
  //Optional, use classic HTTP GET and POST requests.
  //This option allows get and delete functions (PUT and DELETE HTTP requests) works for
  //device connected behind the Firewall that allows only GET and POST requests.
  Firebase.RTDB.enableClassicRequest(&fbdo, true);
  
  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.RTDB.setReadTimeout(&fbdo, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.RTDB.setwriteSizeLimit(&fbdo, "tiny");
  
  //optional, set the decimal places for float and double data to be stored in database
  Firebase.setFloatDigits(2);
  Firebase.setDoubleDigits(6);
//  Serial.println("Fim Firebase");
}

//Envia os dados ao Firebase Realtime Database
void enviaDados(){
//  Serial.println("Enviando Dados");
  Dados *d = leDados();

  realtime(d);
  relatorio(d);

  free(d);
}

//Lê os sensores e os estados dos atuadores e modifica a struct Dados
Dados *leDados(){
  Dados *d = (Dados *) malloc(sizeof(Dados));
  d->bomba = digitalRead(pumpPin);
  d->luz = digitalRead(lampPin);
  d->ventilador = digitalRead(fanPin);

  d->temperatura = sensorTemp.readTemperature();
  d->umidade_ar = sensorTemp.readHumidity();
  d->umidade_solo = analogRead(pinoSensorUmidadeSolo);

  return d;
}

//Envia os dados para o nó 'realtime'
void realtime(Dados *d){  
//  Serial.println("Enviando Realtime...");

  //Implementar com setjson
  json.clear()
    .add("bomba", d->bomba)
    .add("luz", d->luz)
    .add("ventilador", d->ventilador)
    .add("temperatura", d->temperatura)
    .add("umidade_ar", d->umidade_ar)
    .add("umidade_solo", d->umidade_solo);  

  if ( Firebase.RTDB.setJSON(&fbdo, "realtime", &json) )
    Serial.println("Realtime enviado!");
}

//Envia os dados para o nó 'relatorio'
void relatorio(Dados *d){  
//  Serial.println("Enviando Relatorio...");
  
  json.clear()
    .add("timestamp", pegaTimeStamp())
    .add("valor", d->bomba);
  Firebase.RTDB.pushJSON(&fbdo, "relatorio/bomba", &json);
  
  json.clear()
    .add("timestamp", pegaTimeStamp())
    .add("valor", d->luz);
  Firebase.RTDB.pushJSON(&fbdo, "relatorio/luz", &json);
  
  json.clear()
    .add("timestamp", pegaTimeStamp())
    .add("valor", d->ventilador);
  Firebase.RTDB.pushJSON(&fbdo, "relatorio/ventilador", &json);
  
  json.clear()
    .add("timestamp", pegaTimeStamp())
    .add("valor", d->temperatura);
  Firebase.RTDB.pushJSON(&fbdo, "relatorio/temperatura", &json);
  
  json.clear()
    .add("timestamp", pegaTimeStamp())
    .add("valor", d->umidade_ar);
  Firebase.RTDB.pushJSON(&fbdo, "relatorio/umidade_ar", &json);
  
  json.clear()
    .add("timestamp", pegaTimeStamp())
    .add("valor", d->umidade_solo);
    
  if(Firebase.RTDB.pushJSON(&fbdo, "relatorio/umidade_solo", &json))
    Serial.println("Relatório enviado!");
}

void funcionaAutomaticoOuManual(){
  Config *c = leConfig();
  if(c->funcionamento == 1){
    Serial.println("roda Automatico");
    rodaAutomatico(c);
  }
  else if(c->funcionamento == 0){
    Serial.println("responde Comando Remoto");
    respondeComandoRemoto();
  }

  free(c);
//  Serial.println("-------------Fim--------------");
}

//Lê o nó config e modifica a struct Config
Config *leConfig(){  
//  Serial.println("Lendo config...\n");
  Config *c = (Config *) malloc(sizeof(Config));
  
  Firebase.RTDB.getInt(&fbdo, "configuracao/funcionamento", &c->funcionamento);
  Firebase.RTDB.getInt(&fbdo, "configuracao/luz/liga", &c->ligaLuz);
  Firebase.RTDB.getInt(&fbdo, "configuracao/luz/desliga", &c->desligaLuz);
  Firebase.RTDB.getInt(&fbdo, "configuracao/bomba/liga", &c->ligaBomba);
  Firebase.RTDB.getInt(&fbdo, "configuracao/bomba/desliga", &c->desligaBomba);
  Firebase.RTDB.getFloat(&fbdo, "configuracao/ventilador/liga", &c->ligaVentilador);
  Firebase.RTDB.getFloat(&fbdo, "configuracao/ventilador/desliga", &c->desligaVentilador);
  
  Serial.println("Config atualizada!");
  
  return c;
}

//Executa as tarefas conforme os parâmetros da struct Config
void rodaAutomatico(Config *c){
  ligaEDesligaBomba(c);
  LigaEDesligaLuz(c);
  ligaDesligaVentilador(c);
}

void ligaEDesligaBomba(Config *c){
  if (analogRead(pinoSensorUmidadeSolo) > c->ligaBomba){
    digitalWrite(pumpPin, LOW);
  }
  
  if (analogRead(pinoSensorUmidadeSolo) < c->desligaBomba){
    digitalWrite(pumpPin, HIGH);
  }
}

void LigaEDesligaLuz(Config *c){
  int hora = pegaHora();
  if(hora >= c->ligaLuz && hora <= c->desligaLuz)
    digitalWrite(lampPin, LOW);
  else  
    digitalWrite(lampPin, HIGH);
}

void ligaDesligaVentilador(Config *c){  
  float temp = sensorTemp.readTemperature();
  float umi = sensorTemp.readHumidity();

  if(temp >= c->ligaVentilador)
    digitalWrite(fanPin, LOW); // liga o ventilador
  else if (temp < c->desligaVentilador)
    digitalWrite(fanPin, HIGH); // desliga o ventilador
}

//Se estiver configurado para manual, executará os comandos enviados
void respondeComandoRemoto(){
//  Serial.println("Lendo Comandos...\n");
  
  if ( Firebase.RTDB.getString(&fbdo, "comando/mandou") ){
    if( fbdo.dataType() == "string" ){
      String cmd = fbdo.stringData();
      Serial.print("Comando: ");
      Serial.println(cmd);
      
      if (cmd == "L")
          digitalWrite(lampPin, LOW);
      if (cmd == "l")
          digitalWrite(lampPin, HIGH);
      if (cmd == "R")
          digitalWrite(pumpPin, LOW);
      if (cmd == "r")
          digitalWrite(pumpPin, HIGH);
      if (cmd == "V")
          digitalWrite(fanPin, LOW);
      if (cmd == "v")
          digitalWrite(fanPin, HIGH);
//      if (cmd == "E")
//        digitalWrite(luzRelaym LOW);
//      if (cmd == "e")
//        digitalWrite(luzRelaym HIGH);

      if( Firebase.RTDB.setInt(&fbdo, "comando/mandou", 0))
        Serial.println("Comandos lidos!");
    }
  }
}

//Reseta a struct Config para valores "de fábrica"
void resetaConfig(){
  Config *c = (Config*) malloc(sizeof(Config));
  
  json.clear()
    .add("funcionamento", c->funcionamento)
    .add("luz/liga", c->ligaLuz)
    .add("luz/desliga", c->desligaLuz)
    .add("ventilador/liga", c->ligaVentilador)
    .add("ventilador/desliga", c->desligaVentilador)
    .add("bomba/liga", c->ligaBomba)
    .add("bomba/desliga", c->desligaBomba);

  if ( Firebase.RTDB.setJSON(&fbdo, "configuracao", &json) )
    Serial.println("Config Resetada!");
}
