//imports
#include <Wire.h> //for I2C, needed for shields and most breakouts
#include <SparkFun_MMA8452Q.h>
#include <SPI.h>  //required by other libraries for cross communication
#include <WiFiNINA.h>
//#include <WiFi.h>
#include <SD.h>


//variables go here
char ssid[] = "LancerNet"; //edit to actual network name
char pass[] = "thePassword"; //edit to actual password (for WPA or as key for WEP)
int keyIndex = 0; //like password but for WEP instead

IPAddress server(74,125,232,128);  //fill in with number ip of website and comment out line below
//char server[] = "www.google.com"  //using DNS is memory expensive... so try using the above
//WiFiSSLClient client;
WiFiClient client;

MMA8452Q accel;

File root;

unsigned int numFailed = 0; //number of data entries created while no internet

// tri-color LED info
// http://wiring.org.co/learning/basics/rgbled.html


//helper functions
void printAccels() {
  Serial.print("x: ");
  Serial.print(accel.x, 3);
  Serial.print("\t y: ");
  Serial.print(accel.y, 3);
  Serial.print("\t z: ");
  Serial.println(accel.z, 3);
}

void reconnect() {
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 443)) { //port 80 instead for non SSL connection
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


//startup goes here
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; //do nothing till serial is avalible
  }
  Serial.println("Initializing...");

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true); // don't continue
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  //do first connection
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWiFiStatus();

  reconnect();

  /*
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("SD init failed!");
    while (1);
    //add turning on a LED here
  }
  root = SD.open("/");
  */

  accel.init(SCALE_8G, ODR_800);
  //fastest readings of 800hz of values between -8g and 8g

  pinMode(A0,INPUT);
  /*  the small 3 pin semi circle shaped thing is temperature sensor,
  connect middle pin to A0, flat side of sensor is "front",
  left is 5v, right is ground (GND) */  

  Serial.println("Finished startup!");
}


//running goes here
void loop() {
  //step 1: test connection status, reconnect if needed
  //i don't trust existing "status" variable right now, so made my own
  int WS = WiFi.status();
  if (WS != WL_CONNECTED) {
    ++numFailed;
    if (WS == WL_CONNECTION_LOST || WS == WL_DISCONNECTED) {
      Serial.println("Lost connection to webpage/server!");
      reconnect();
    }
  }

  //step 2: grab data
  if (accel.available()) {
    accel.read();
    // accel reads x, y, and z as signed 12 bit ints
    printAccels(); //replace with string building for JSON
  }
  float temp = analogRead(A0);
  Serial.print("Temperature is ... ");
  Serial.println(temp, 3); //3 decimal places

  //step 3: outputing
  if (WS == WL_CONNECTED) {
    //upload to database
  } else {
    //not (re)connected, use SD
  }
}
