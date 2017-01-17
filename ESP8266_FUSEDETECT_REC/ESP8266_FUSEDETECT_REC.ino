/*
    Code for the fuse aggregator. 
      1. Sleep until interrupt on GPIO 1
      2. Wake, then connect to AP
      3. Send TCP packet of phase and GPIO state
      4. go back to sleep
*/

#include<ESP8266WiFi.h>

const char* ssid = "FUSE";
const char* password = "SKUNKWORKS_1!";

WiFiServer server(4000);
IPAddress local_IP(192,168,0,2);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

void setup() 
{
  //init serial & GPIO
  Serial.begin(115200);

  //Set AP mode
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid, password, 1, false) ? "Ready" : "Failed!");
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  server.begin();
}    
  
int stationsConnected = 0;

void loop() 
{
  WiFiClient cli = server.available();
  if(cli) //if a client sending request
  {
    while (cli.connected())
    {
      if (cli.available())
      {
        String line = cli.readStringUntil('\r');
        Serial.println(line);
      }
    }
  }
  
  int currStationsConnected =  WiFi.softAPgetStationNum();
  if(currStationsConnected != stationsConnected)
  {
    Serial.printf("Stations connected: %d\r\n", currStationsConnected);
    stationsConnected = currStationsConnected;
  }
  delay(500);
}


