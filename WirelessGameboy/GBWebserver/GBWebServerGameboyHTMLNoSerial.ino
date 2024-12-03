/*
    This sketch demonstrates how to set up a simple HTTP-like server.
    The server will set a GPIO pin depending on the request
      http://server_ip/gpio/0 will set the GPIO2 low,
      http://server_ip/gpio/1 will set the GPIO2 high
    server_ip is the IP address of the ESP8266 module, will be
    printed to Serial when the module is connected.
*/

#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "gameboy wifi"
#define STAPSK  ""
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

enum Buttons {
  Start = 15,
  Select = 0,
  Up = 2,
  DRight = 14,
  Down = 13,
  Left = 4,
  A = 16,
  B = 5,
  DLeft = 12,
  Right = 2
};

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // prepare LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);

  // Connect to WiFi network
  WiFi.softAP(ssid);

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
  if (req.indexOf(F("/button/a")) != -1) {
    toggleButton(A);
  } else if (req.indexOf(F("/button/b")) != -1) {
    toggleButton(B);
  } else if (req.indexOf(F("/button/start")) != -1) {
    toggleButton(Start);
  } else if (req.indexOf(F("/button/select")) != -1) {
    toggleButton(Select);
  } else if (req.indexOf(F("/button/left")) != -1) {
    toggleButton(Left);
  } else if (req.indexOf(F("/button/right")) != -1) {
    toggleButton(Right);
  } else if (req.indexOf(F("/dpad/north")) != -1) {
    toggleButton(Up);
  } else if (req.indexOf(F("/dpad/east")) != -1) {
    toggleButton(DRight);
  } else if (req.indexOf(F("/dpad/south")) != -1) {
    toggleButton(Down);
  } else if (req.indexOf(F("/dpad/west")) != -1) {
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
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF-8\"> <title>Title</title></head><body> <textarea style=\"display:none;\" id=\"testing\" rows=\"200\" cols=\"200\"></textarea> <script>const callButton=(button)=>{fetch(\"http://192.168.4.1/button/\"+button,{method: \"GET\"});}; const callDpad=(direction)=>{fetch(\"http://192.168.4.1/dpad/\"+direction,{method: \"GET\"});}; var area=document.querySelector(\"#testing\"); area.addEventListener(\'input\', (e)=>{let value=e.target.value; switch(value){case \"w\": callDpad(\"north\"); break; case \"d\": callDpad(\"east\"); break; case \"s\": callDpad(\"south\"); break; case \"a\": callDpad(\"west\"); break; case \"e\": callButton(\"a\"); break; case \"r\": callButton(\"b\"); break; case \"p\": callButton(\"start\"); break; case \"o\": callButton(\"select\"); break;}e.target.value=\"\";}); </script> <div class=\"gameboy\"> <div class=\"top\"> <div class=\"onoff\"> <span class=\"arrow-left\"></span><span class=\"onoff-label\">on/off</span><span class=\"arrow-right\"></span> </div></div><div class=\"components\"> <div class=\"screen\"> <div class=\"battery\"></div><canvas class=\"display\" id=\"display\"></canvas> </div><div class=\"controls\"> <div class=\"logo\"> <div class=\"logo-nintendo\"></div><div class=\"logo-gameboy\"></div></div><div class=\"inputs\"> <div class=\"dpad\"> <div class=\"left\" onmousedown=\"callButton(\'left\')\"></div><div class=\"up\" onmousedown=\"callDpad(\'north\')\"></div><div class=\"right\" onmousedown=\"callDpad(\'east\')\"></div><div class=\"down\" onmousedown=\"callDpad(\'south\')\"></div></div><div class=\"buttons\"> <div class=\"button-b\" onmousedown=\"callButton(\'b\')\"></div><div class=\"button-a\" onmousedown=\"callButton(\'a\')\"></div></div><div class=\"selections\"> <div class=\"select\" onmousedown=\"callButton(\'select\')\"></div><div class=\"start\" onmousedown=\"callButton(\'select\')\"></div></div></div><div class=\"speakers\"> <div class=\"grill\"></div><div class=\"grill\"></div><div class=\"grill\"></div><div class=\"grill\"></div><div class=\"grill\"></div><div class=\"grill\"></div></div></div></div></div><style>html,body{background-color: #3c3c3c; width: 100%; height: 100%; font-family: arial; box-sizing: border-box; padding: 0; margin: 0;}.gameboy{margin-left: auto; margin-right:auto; margin-top: 25px; width: 367px; height: 630px; background-color: rgb(210,221,205); border-radius: 0 20px 40px 0; box-shadow: inset -2px -1px 5px 2px rgba(0,0,0,0.25);}.gameboy:after{content: \" \"; top: 25px; position: absolute; display: block; width: 363px; height: 625px; border-radius: 0 20px 40px 0; box-shadow: inset 0px 0px 2px 2px rgba(255, 255, 255, 0.25); user-select: none; pointer-events: none;}.top{height: 28px; border-bottom: 3px solid rgb(188, 202, 179);}.onoff{width: 56px; height: 15px; position: absolute; display: flex; background-color: #b8c4ae; margin-left: 40px; margin-top: -28px; border-radius: 24px; color: #d2ddcd; padding-left: 12px; padding-top:3px; box-shadow: inset -1px 1px 10px 1px rgba(0, 0, 0, 0.1);}.onoff .arrow-left:before{content: \" \"; position: absolute; border-right: 0; margin-left: -7px; margin-top: 1px; border-top: 5px solid transparent; border-bottom: 5px solid transparent; border-right: 5px solid #d2ddcd;}.onoff .arrow-right:before{content: \" \"; position: absolute; border-right: 0; margin-left: 2px; margin-top: 1px; border-top: 5px solid transparent; border-bottom: 5px solid transparent; border-left: 5px solid #d2ddcd;}.onoff-label{font-size: 8pt; font-weight: bold; text-transform: uppercase;}.components{border-top: 1px solid rgba(255,255,255,0.25);}.top:before{content: \" \"; height: 30px; position: relative; left: 30px; display: inline-block; top: 0px; border-left: 1px solid rgba(255, 255, 255, 0.25); border-right: 3px solid #bccab3;}.top:after{content: \" \"; height: 28px; position: relative; display: inline-block; left: 324px; border-left: 1px solid rgba(255,255,255,0.25); border-right: 3px solid rgb(188, 202, 179);}.screen{background-color: #585769; width: 317px; height: 247px; margin-left: auto; margin-right: 28px; margin-top: 15px; border-radius: 10px 10px 50px 10px; border: 1px solid rgba(0,0,0,0.25); display: flex; flex-direction: row; box-shadow: inset -1px 1px 10px 1px rgba(0, 0, 0, 0.2);}.screen:before,.screen:after{content: \" \"; border-top: 3px solid rgb(68, 47, 62); border-bottom: 3px solid rgb(65, 55, 79); position: absolute; height: 2px; margin-top: 13px;}.screen:before{margin-left: 15px; width: 83px;}.screen:after{margin-left: 265px; width: 35px;}.battery{width: 12px; height: 12px; background-color: #6f0202; border-radius: 12px; margin-top: 91px; margin-left: 15px; margin-right: 27px; box-shadow: inset 1px 1px 0px 1px rgba(0, 0, 0, 0.25);}.battery:after{content: \"BATTERY\"; color: white; width: 0; height:0; font-size: 6pt; position: relative; display: block; margin-left: -6px; margin-top: 17px;}.display{width: 210px; height: 195px; background-color: black; margin-top: 27px; border-left: 1px solid rgba(255,255,255,0.4); border-top: 1px solid rgba(255,255,255,0.4);}.display:before{content: \"DOT MATRIX WITH STEREO SOUND\"; position: absolute; color: white; font-size: 6.5pt; margin-top: -16px; margin-left: 50px;}.controls{display: flex; flex-direction: column;}.logo{color: rgb(70, 71, 99); display: flex; line-height: 22pt; margin-top: 5px; margin-left: 20px;}.logo-nintendo:before{content: \"Nintendo\"; font-weight: bold; margin-top: 2px; position: relative; display: inline-block;}.logo-gameboy:before{content: \"GAME BOY\"; font-style: italic; font-weight: bold; font-size: 17pt;}.inputs{display: flex; justify-content: space-between; margin-top: 34px; flex-wrap: wrap;}.dpad{width: 128px; height: 128px; background-color: rgb(184, 196, 174); border-radius: 100px; margin-left: 10px; box-shadow: inset -2px -1px 5px 2px rgba(0,0,0,0.025);}.dpad:before{content: \" \"; display: block; position: absolute; width: 36px; height: 36px; background-color: #353531; margin-top: 46px; margin-left: 46px;}.dpad:after{content: \" \"; display: block; position: absolute; width: 24px; height: 24px; border-radius: 24px; background-color: black; margin-top: 54px; margin-left: 51px;}.dpad .left,.dpad .right,.dpad .up,.dpad .down{background-color: #353531; position: absolute;}.dpad .left,.dpad .right{width: 33px; height: 36px;}.dpad .up,.dpad .down{width: 36px; height: 33px;}.dpad .left:active,.dpad .right:active,.dpad .up:active,.dpad .down:active{background-color: black;}.dpad .up{margin-top: 13px; margin-left: 46px; border-radius: 5px 5px 0 0;}.dpad .down{margin-left: 46px; margin-top: 81px; border-radius: 0 0 5px 5px; border-bottom: 3px solid black;}.dpad .right{border-radius: 0 5px 5px 0; margin-top: 48px; margin-left: 81px; border-bottom: 3px solid black;}.dpad .left{border-radius: 5px 0 0 5px; margin-top: 48px; margin-left: 13px; border-bottom: 3px solid black;}.dpad .down:before,.dpad .left:before,.dpad .up:before,.dpad .right:before{content: \" \"; width: 0; height: 0; position: relative;}.dpad .up:before{border-left: 5px solid transparent; border-right: 5px solid transparent; border-bottom: 5px solid #d2ddcd; margin-left: 12px; top: -28px;}.dpad .right:before{position: absolute; border-right: 0; border-top: 5px solid transparent; border-bottom: 5px solid transparent; border-left: 5px solid #d2ddcd; top: 12px; right: -10px;}.dpad .left:before{position: absolute; border-right: 0; border-top: 5px solid transparent; border-bottom: 5px solid transparent; border-right: 5px solid #d2ddcd; top: 12px; right: 38px;}.dpad .down:before{position: absolute; border-right: 0; border-left: 5px solid transparent; border-right: 5px solid transparent; border-top: 5px solid #d2ddcd; top: 40px; right: 12px;}.dpad .right:after,.dpad .up:after,.dpad .left:after,.dpad .down:after{content: \"| |\"; color: rgba(0,0,0, 0.5); display: block; font-size: 15pt;}.dpad .right:after{margin-left: 7px; margin-top: 5px;}.dpad .left:after{margin-left: 10px; margin-top: 5px;}.dpad .down:after{transform: rotate(90deg); margin-left: 5px; margin-top: 15px;}.dpad .up:after{transform: rotate(90deg); margin-left: 5px; margin-top: -5px;}.buttons{width: 115px; height: 65px; background-color: rgb(184, 196, 174); border-radius: 60px; margin-left: 80px; margin-right: 5px; transform: rotate(-30deg); margin-top: 29px; display: flex; justify-content: space-around; box-shadow: inset -2px -1px 5px 2px rgba(0,0,0,0.05);}.button-a,.button-b{width: 46px; height: 46px; border-radius: 40px; margin-top: 9px; border: 3px solid rgba(42, 0, 3,0.25); box-sizing: border-box; background-color: rgb(99, 0, 44); box-shadow: 1px 1px 0px 1px rgba(0, 0, 0, 0.25);}.button-a:before,.button-b:before{content: \" \"; width: 42px; height: 42px; border-top: 1px solid rgba(255,255,255,0.45); border-right: 1px solid rgba(255,255,255,0.45); border-radius: 40px; display: block; margin-left: -1px; margin-top: -2px;}.button-a:active:before,.button-b:active:before{border-color: transparent;}.button-a:after{content: \"A\";}.button-b:after{content: \"B\";}.button-a:after,.button-b:after{position: relative; display: inline-block; margin-top: 14px; margin-left: 14px; font-size: 9pt; font-weight: bold;}.button-a:active,.button-b:active{background-color: #400921;}.selections{display: flex; justify-content: center; width: 100%; margin-top: 10px;}.start,.select{z-index: 1; position: relative; height: 15px; width: 48px; border-radius: 15px; background-color: rgb(82, 85, 100); transform: rotate(-25deg); margin: 5px; box-sizing: border-box; box-shadow: inset -2px -1px 5px 2px rgba(0,0,0,0.25);}.start:active,.select:active{background-color: #3d404c;}.select:before,.start:before{z-index: 0; position: absolute; content: \" \"; margin-top:-2px; margin-left: -2px; height: 17px; width: 50px; border-radius: 15px; border: 1px solid rgba(0,0,0,0.2); box-shadow: inset -1px -1px 2px 1px rgba(0,0,0,0.25);}.select:after,.start:after{position: relative; top: 16px; font-size: 9pt; font-weight: bold; color: rgb(15, 19, 30);}.select:after{content: \"SELECT\";}.start:after{content: \"START\";}.speakers{display: flex; justify-content: flex-end; margin-top: 60px; transform: rotate(-30deg);}.speakers .grill{width: 10px; height: 70px; border-radius: 20px; background-color: rgb(176, 189, 171); border: 1px solid rgba(0,0,0,0.35); border-top: 0; border-right: 0; border-bottom-color: rgb(239,239,239); border-left-color: rgb(239,239,239); margin: 3px; box-shadow: inset -1px -1px 2px 1px rgba(0,0,0,0.25);}.speakers .grill:before{display: block; margin-left: 1px; margin-top: 2px; border-radius: 15px; content: \" \"; width: 1px; height: 50px; border-left: 1px solid black;}</style></body></html>"));

  // The client will actually be *flushed* then disconnected
  // when the function returns and 'client' object is destroyed (out-of-scope)
  // flush = ensure written data are received by the other side
  Serial.println(F("Disconnecting from client"));
}
