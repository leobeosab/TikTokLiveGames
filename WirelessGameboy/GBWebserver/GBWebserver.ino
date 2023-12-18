/*
    This sketch demonstrates how to set up a simple HTTP-like server.
    The server will set a GPIO pin depending on the request
      http://server_ip/gpio/0 will set the GPIO2 low,
      http://server_ip/gpio/1 will set the GPIO2 high
    server_ip is the IP address of the ESP8266 module, will be
    printed to Serial when the module is connected.
*/

#include <WiFi.h>

#define LED_BUILTIN 2

const boolean AP_MODE = false;


const char* ap_ssid = "gameboy wifi";
const char* ap_password = "";

const char* ssid = "test";
const char* password = "testtest";

boolean led_lit = false;

enum Buttons {
  Select = 18,
  Right = 4,
  Left = 19,
  B = 23,
  Up = 21,
  DLeft = 17,
  Down = 16,
  DRight = 5,
  Start = 2,
  A = 22
};

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(9600);

  // prepare LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, led_lit);

  // Connect to WiFi network
  if (AP_MODE) {
    WiFi.softAP(ssid);
  } else {
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      led_lit = !led_lit;
      digitalWrite(LED_BUILTIN, led_lit);
      delay(1000);
      Serial.println(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }


  // Start the server
  server.begin();
  Serial.println(F("Server started"));

  // Print the IP address
  Serial.println(WiFi.localIP());



  // Setup Gameboy Pins
  //Start
  pinMode(Start, OUTPUT);

  //Select
  pinMode(Select, OUTPUT);

  //Up
  pinMode(Up, OUTPUT);
  //Righ
  pinMode(Right, OUTPUT);
  //Left
  pinMode(Left, OUTPUT);
  //Down
  pinMode(Down, OUTPUT);
  //A
  pinMode(A, OUTPUT);
  //B
  pinMode(B, OUTPUT);
  //Left
  pinMode(DLeft, OUTPUT);
  //Right
  pinMode(DRight, OUTPUT);

}

void toggleButton(Buttons b) {
  digitalWrite(b, HIGH);
  delay(75);
  digitalWrite(b, LOW);
}

void loop() {
  delay(1); // If this isn't here it will almost never load

  const String word = Serial.readStringUntil('\n');
  if (word.indexOf(F("/button/a")) != -1) {
    toggleButton(A);
  } else if (word.indexOf(F("/button/b")) != -1) {
    toggleButton(B);
  } else if (word.indexOf(F("/button/start")) != -1) {
    toggleButton(Start);
  } else if (word.indexOf(F("/button/select")) != -1) {
    toggleButton(Select);
  } else if (word.indexOf(F("/button/left")) != -1) {
    toggleButton(Left);
  } else if (word.indexOf(F("/button/right")) != -1) {
    toggleButton(Right);
  } else if (word.indexOf(F("/dpad/north")) != -1) {
    toggleButton(Up);
  } else if (word.indexOf(F("/dpad/east")) != -1) {
    toggleButton(DRight);
  } else if (word.indexOf(F("/dpad/south")) != -1) {
    toggleButton(Down);
  } else if (word.indexOf(F("/dpad/west")) != -1) {
    toggleButton(DLeft);
  } else {
    Serial.println(F("invalid request"));
  }


  return;
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println(F("new client"));

  client.setTimeout(5000); // default is 1000

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(F("request: "));
  Serial.println(req);

  // Match the request
  int val;
  if (word.indexOf(F("/button/a")) != -1) {
    toggleButton(A);
  } else if (word.indexOf(F("/button/b")) != -1) {
    toggleButton(B);
  } else if (word.indexOf(F("/button/start")) != -1) {
    toggleButton(Start);
  } else if (word.indexOf(F("/button/select")) != -1) {
    toggleButton(Select);
  } else if (word.indexOf(F("/button/left")) != -1) {
    toggleButton(Left);
  } else if (word.indexOf(F("/button/right")) != -1) {
    toggleButton(Right);
  } else if (word.indexOf(F("/dpad/north")) != -1) {
    toggleButton(Up);
  } else if (word.indexOf(F("/dpad/east")) != -1) {
    toggleButton(DRight);
  } else if (word.indexOf(F("/dpad/south")) != -1) {
    toggleButton(Down);
  } else if (word.indexOf(F("/dpad/west")) != -1) {
    toggleButton(DLeft);
  } else {
    Serial.println(F("invalid request"));
  }

  // read/ignore the rest of the request
  // do not client.flush(): it is for output only, see below
  while (client.available()) {
    // byte by byte is not very efficient
    client.read();
  }

  // This should probably only return the full website on "/"
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html lang=\"en\">cool</body></html>"));

  // The client will actually be *flushed* then disconnected
  // when the function returns and 'client' object is destroyed (out-of-scope)
  // flush = ensure written data are received by the other side
  Serial.println(F("Disconnecting from client"));
}
