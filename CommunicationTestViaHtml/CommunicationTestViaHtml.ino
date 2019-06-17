#include <WiFi.h>

const char ssid[]="SSID";
const char passwd[] ="PASSWORD";
const char html_first[]=
"<!DOCTYPE html>\
<html>\
<head>\
<meta http-equiv=”refresh” content=0.1>\
<title>\
test html & WiFi\
</title>\
</head>\
<body>\
<h1>\
test html & WiFi\
</h1>\
<p>\
sound volume:";

const char html_end[]=
"</p>\
<form method='get'>\
<input type='submit' name='led_ON' value='ON' />\
<input type='submit' name='led_OFF' value='OFF' />\
</form>\
</body>\
</html>";

WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(16, OUTPUT);
  connectWiFi();

  server.begin();
}

int snd_vol;
void getSoundVolume(){
  snd_vol = analogRead(32);
}

void loop() {
  // put your main code here, to run repeatedly:
  if ( WiFi.status() == WL_DISCONNECTED ) {
        connectWiFi();
  }
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    String currentLine = "";
    while(client.connected()){
      if(client.available()){
        char c = client.read();
        Serial.write(c);
        if(c == '\n'){
          if (currentLine.length() == 0){
            getSoundVolume();
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            
            client.println(html_first);
            client.println(snd_vol);
            client.println(html_end);
            client.println();
            break;
          }else{
            currentLine = "";
          }
        }else if(c != '\r'){
          currentLine += c;
        }
        if(currentLine.endsWith("GET /?led_ON")){
          digitalWrite(16, HIGH);
        }
        if(currentLine.endsWith("GET /?led_OFF")){
          digitalWrite(16, LOW);
        }

      }
    }
    client.stop();
    Serial.println("client disonnected");
  }  
}

void connectWiFi() {
  WiFi.begin(ssid, passwd);
  Serial.print("WiFi connecting...");
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.print(" connected. ");
  Serial.println(WiFi.localIP());
}
