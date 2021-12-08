/*
Inclusão da lib SoftwareSerial usada para comunicação 
com o módulo bluetooth
*/
#include <SoftwareSerial.h>

// o modulo hc06 (e 05) funcionam nesse baud_rate
#define BAUD_RATE 38400 

int motorDir1 = 4; //D2 Ligado ao pino 2 do L293D
int motorDir2 = 0; //D3 Ligado ao pino 7 do L293D

int motorEsq1 = 16; //D0 Ligado ao pino 10 do L293D
int motorEsq2 = 5; //D1 Ligado ao pino 15 do L293D

/* inicializaa variável da lib, setando os pinos RX e TX, 
 D7(13) e D8(15), respectivamente
 */
SoftwareSerial swSer(13, 15, false, 256);

void viraDirreita(){
  if(Serial.available())
    Serial.println("girando direita");
    
  digitalWrite(motorDir1, LOW);
  digitalWrite(motorDir2, LOW); 
  
  digitalWrite(motorEsq1, HIGH );
  digitalWrite(motorEsq2, LOW); 
}

void viraEsquerda(){
  if(Serial.available())
    Serial.println("girando esquerda");
    
  digitalWrite(motorDir1, LOW);
  digitalWrite(motorDir2, HIGH);
  
  digitalWrite(motorEsq1, LOW);
  digitalWrite(motorEsq2, LOW); 
  
}

void andaFrente(){  
  if(Serial.available())
    Serial.println("Andanro p frente");
    
  digitalWrite(motorDir1, LOW);
  digitalWrite(motorDir2, HIGH);
  
  digitalWrite(motorEsq1, HIGH);
  digitalWrite(motorEsq2, LOW);  
}

void daRe(){
  if(Serial.available())
    Serial.println("dando re");
    
  digitalWrite(motorDir1, HIGH);
  digitalWrite(motorDir2, LOW);
  
  digitalWrite(motorEsq1, LOW);
  digitalWrite(motorEsq2, HIGH); 
}

void para()
{
  if(Serial.available())
    Serial.println("MOTOR PARADO.................................");
    
  digitalWrite(motorDir1, LOW);
  digitalWrite(motorDir2, LOW);
  
  digitalWrite(motorEsq1, LOW);
  digitalWrite(motorEsq2, LOW);  
}



void setup()
{
  Serial.begin(BAUD_RATE);
  swSer.begin(BAUD_RATE);
  
  if(Serial.available())
    Serial.print("Setando inputs e outputs\n");
    
  pinMode(motorDir1, OUTPUT);
  pinMode(motorDir2, OUTPUT);
  pinMode(motorEsq1, OUTPUT);
  pinMode(motorEsq2, OUTPUT);
}

void loop()
{  
  if(swSer.available()){
    char byteRecebido = (char) swSer.read();
    if(Serial.available())
      Serial.println(byteRecebido);
    
    if(byteRecebido == 'W'){
      andaFrente();
    }
    else if(byteRecebido == 'S'){
      daRe();
    }
    else if(byteRecebido == 'D'){
      viraDireita();
    }
    else if(byteRecebido == 'A'){
      viraEsquerda();
    }
    else{
      para();
    }
  }
}
