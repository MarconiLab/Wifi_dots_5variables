//Author: Rodrigo Carbajales MarconiLab

#include <WiFi.h>

 //Dots configuration
#define TOKEN          "Df2PxNVLpOdn2phb7BriUTs0JwkVOvv6su2zBPh2QY1FbPYok5OMHQelKFNY"  //Replace with your TOKEN
#define VARIABLEID1     "5379cddb7625427e2efd958a"                                      //Replace with your variable ID
#define VARIABLEID2    "5386d9c77625426be003b163"              //second variable
#define VARIABLEID3    "538dde6d7625426dbe64dac1"
#define VARIABLEID4    "538dde5d7625426dbe64daaf"
#define VARIABLEID5    "538dde4d7625426dbe64daa6"


char ssid[] = "MarconiLab";     //  your network SSID (name) 
char pass[] = "marconi-lab";  // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
WiFiClient client;
char server[] = "things.ubidots.com";
 
int resetTimer = 0; 

void setup() {
  Serial.begin(9600);
 
  
    // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
   // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:    
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
     }
   
  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();

}

void loop() {

  Serial.println("Trying to send data:" + String(analogRead(0)));
  writeUbidots(String(analogRead(0)), VARIABLEID1 );     //Send data in String format to the Ubidots function
  char buffer[14];
  String in = dtostrf(analogRead(0)/1.5, 7,2,buffer);
  writeUbidots(String(in), VARIABLEID2 );                              //Send data in String format to the Ubidots function
  writeUbidots(String(analogRead(0)*3), VARIABLEID3 );                              //Send data in String format to the Ubidots function
  writeUbidots(String(analogRead(0)*4), VARIABLEID4 );                              //Send data in String format to the Ubidots function
  writeUbidots(String(analogRead(0)*5), VARIABLEID5 );                              //Send data in String format to the Ubidots function


  if(resetTimer == 10) {                                          //Restarts if the connection fails 10 times in a row
    asm volatile ("  jmp 0");
  }

  delay(1000); 
}

void writeUbidots(String data, String VARID) {
  String dataString = "{\"value\":"+ data + "}";                  //Prepares the data in JSON format

  if (client.connect(server, 80)) {                               //If connection is successful, then send this HTTP Request:
    Serial.println("Connecting to " + String(VARID));
    client.print("POST /api/v1.6/variables/");                    //Specify URL, including the VARIABLE ID
    client.print(VARID);
    client.println("/values HTTP/1.1");
    client.println("Host: things.ubidots.com");
    client.print("X-Auth-Token: ");                               //Specify Authentication Token in headers
    client.println(TOKEN);
    client.print("Content-Length: ");
    client.println(dataString.length());
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();                                             //End of HTTP headers

    client.println(dataString);                                   //This is the actual value to send

    client.flush();
    client.stop();
//    return 1;
  }
  else {                                                          // If the connection wasn't possible, then:
    resetTimer += 1;
    Serial.println("Connection failed");
    Serial.println("Device will restart after 10 failed attempts, so far:"+String(resetTimer)+" attempts.");
    Serial.println("Killing sockets and disconnecting...");
    client.flush();
    client.stop();
//    return 0;
  }
}

void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);
  
  // print your MAC address:
  byte mac[6];  
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
 
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);    
  Serial.print("BSSID: ");
  Serial.print(bssid[5],HEX);
  Serial.print(":");
  Serial.print(bssid[4],HEX);
  Serial.print(":");
  Serial.print(bssid[3],HEX);
  Serial.print(":");
  Serial.print(bssid[2],HEX);
  Serial.print(":");
  Serial.print(bssid[1],HEX);
  Serial.print(":");
  Serial.println(bssid[0],HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption,HEX);
  Serial.println();
}
