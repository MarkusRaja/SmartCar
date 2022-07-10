#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <NewPing.h>
int stat = 0;
int stepone = 0;
int steptwo = 0;
int step3 = 0;
int step4 = 0;
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<center>
<h1>Mobil mobilan</h1><br>
<a href="forWard" target="myIframe">Forward</a><br>
<p>
<a href="turnLeft" target="myIframe">Turn Left</a> 
<a href="sTop" target="myIframe">Stop Car</a> 
<a href="turnRight" target="myIframe">Turn Right</a></p><br><br>
<a href="skemaSatu" target="myIframe">Scheme one</a><br>
<a href="skemaDua" target="myIframe">Scheme Two</a><br>
<a href="sensorTiga" target="myIframe">Sensor 360</a><br>
Mobil dalam keadaan :<iframe name="myIframe" width="100" height="25"\ frameBorder="0"><br>

</center>

</body>
</html>
)=====";
//---------------------------------------------------------------

#define RIGHT 5
#define LEFT 4
#define TRIGGER_PIN  D5  //Pin Trigger HC-SR04 pada NodeMCU
#define TRIGGER_PIN1  D6
#define ECHO_PIN     12  //Pin Echo HC-SR04 pada NodeMCU
#define MAX_DISTANCE 250 //Maksimum Pembacaan Jarak (cm)
NewPing sonar(TRIGGER_PIN1, ECHO_PIN, MAX_DISTANCE);
const char* ssid = "pro updated"; 
const char* password = "markustian2345"; 

ESP8266WebServer server(80);
int chkstat(){
  return stat;
}
void forward(){
  digitalWrite(LEFT,LOW);
  digitalWrite(RIGHT,LOW);
}
void left(){
  digitalWrite(LEFT,HIGH);
 digitalWrite(RIGHT,LOW);
  
}
void right(){
  digitalWrite(LEFT,LOW);
  digitalWrite(RIGHT,HIGH);
}
void stopcar(){
  digitalWrite(LEFT,HIGH); //LED off
 digitalWrite(RIGHT,HIGH); //LED off
}
void handleRoot() {
 Serial.println("You called root page");
 String s = MAIN_page;
 server.send(200, "text/html", s);
}

void handleFORWARD() { 
 stat = 1;
 
 
 Serial.println("Forward");
 
}
void skemaSATU() { 
 stepone = 350;
 steptwo = 350;
 step3 = 350;
 step4 = 250;
 stat = 2;
 
 Serial.println("Skema satu");
 
}
void skemaDUA() { 
 stepone = 150;
 steptwo = 400;
 step3 = 120;
 step4 = 180;
 stat = 3;
 
 Serial.println("Skema dua");
 
}
void handleLEFT() { 
 Serial.println("LEFT");
 left();
 delay(250);
 digitalWrite(RIGHT,HIGH);
 server.send(200, "text/html", "LEFT");
}
void sensorTiga() { 
//  stat = 4;
left();
delay(245);
stopcar();
 Serial.println("sensor");
 server.send(200, "text/html", "sensor 360");
}
void handleRIGHT() { 
 Serial.println("RIGHT");
 right();
 delay(250);
 digitalWrite(LEFT,HIGH);
 server.send(200, "text/html", "RIGHT");
}

void handleSTOP() { 
  stat = 0;
 Serial.println("LED off page");
 stopcar();
 server.send(200, "text/html", "STOP");
}

void setup(void){
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);      
  Serial.println("");

  pinMode(LEFT,OUTPUT); 
  pinMode(RIGHT,OUTPUT); 
  pinMode(TRIGGER_PIN, INPUT);
  stopcar();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); 
  server.on("/", handleRoot);    
  server.on("/forWard", handleFORWARD);
  server.on("/sTop", handleSTOP);
  server.on("/turnLeft", handleLEFT);
  server.on("/turnRight", handleRIGHT);
  server.on("/skemaSatu", skemaSATU);
  server.on("/skemaDua", skemaDUA);
  server.on("/sensorTiga", sensorTiga);
  server.begin();
  Serial.println("HTTP server started");
}
void loop(void){
  
  server.handleClient();
  if (stat == 1){
    int jarak = sonar.ping_cm();
    if(jarak > 40){
    forward();
    server.send(200, "text/html", "FORWARD");\
    jarak = sonar.ping_cm();
    
  }
  else if(jarak <= 40){
    stopcar();
    jarak = sonar.ping_cm();
  }
  }
  //skema pertama
  else if (stat == 2){
    server.send(200, "text/html", "Scheme One Running");
    int jarak = sonar.ping_cm();
    if(jarak > 20){
    if (stepone > 0){
      forward();
    
    jarak = sonar.ping_cm();
    stepone = stepone- 1;
    delay(1);
    }
    else if (steptwo > 0){
      left();
    jarak = sonar.ping_cm();
    steptwo = steptwo - 1;
    delay(1);
    }
    else if (step3 > 0){
      forward();
    jarak = sonar.ping_cm();
    step3 = step3 - 1;
    delay(1);
    }
    else if (step4 > 0){
      right();
    jarak = sonar.ping_cm();
    step4 = step4 - 1;
    delay(1);
    }
    else if (step4 == 0){
      server.send(200, "text/html", "Scheme One Done");
      stat = 0;
     stopcar();
    }
    
  }
  else if(jarak <= 20){
    right();
    jarak = sonar.ping_cm();
  }
  }
  //skema kedua
  else if (stat == 3){
    server.send(200, "text/html", "Scheme Two Running");
    int jarak = sonar.ping_cm();
    if(jarak > 20){
    if (stepone > 0){
      left();
    jarak = sonar.ping_cm();
    stepone = stepone- 1;
    delay(1);
    }
    else if (steptwo > 0){
      forward();
    jarak = sonar.ping_cm();
    steptwo = steptwo - 1;
    delay(1);
    }
    else if (step3 > 0){
     right();
    jarak = sonar.ping_cm();
    step3 = step3 - 1;
    delay(1);
    }
    else if (step4 > 0){
     left();
    jarak = sonar.ping_cm();
    step4 = step4 - 1;
    delay(1);
    }
    else if (step4 == 0){
      server.send(200, "text/html", "Scheme Two Done");
      stat = 0;
     stopcar();
    }
    
  }
  else if(jarak <= 20){
    right();
    jarak = sonar.ping_cm();
  }
  }
  else if (stat == 4){
    
    if (digitalRead(TRIGGER_PIN) == HIGH){
      forward();
      }
     else if (digitalRead(TRIGGER_PIN) == LOW){
      stopcar();
      
      }
    }
}
