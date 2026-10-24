
#include <WiFi.h> //Wifi Librart
const char* ssid ="Cash Money's S21 Ultra";
const char* password= "nnor242)";

WiFiServer server(80);

String header; /* start: These are aux variables to maintain our state the more we need, the more aux we create*/

String output26State = "off"; 
String output27State = "off";

const int output26 = 26;
const int output27 = 27;
const int TrigPin = 14;
const int EchoPin = 34;
long duratioin;
float distanceCm;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  pinMode(output26,OUTPUT);
  pinMode(output27,OUTPUT);

  digitalWrite(output26,LOW);
  digitalWrite(output27,LOW);

  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  Serial.print("Connecting: ");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(" . ");

  }
Serial.println("");
Serial.println("WIFI Connected");
Serial.println("IP Address: ");
Serial.println(WiFi.localIP());
server.begin();

}

void loop() {
  // put your main code here, to run repeatedly:



  WiFiClient client = server.available(); //Incomming clinents
  if(client){ //This is to not change unless you genuinely know what to do
    Serial.println("New Client");
    String currentLine = ""; //Creates a string to hold incomming data from client
    while(client.connected()){  //loop while the clients connected
      if(client.available()){
        char c = client.read(); //Reads a byte 
        Serial.write(c); //Prints byte to serial
        header += c;
        if (c == '\n'){
          if(currentLine.length() == 0){
            client.println("HTTP/1.1 200 OK");
            client.println("COntent-type: text/html");
            client.println("Connection: close");
            client.println();
            if (header.indexOf("GET /26/on") >= 0) {
 Serial.println("GPIO 26 on");
 output26State = "on";
 digitalWrite(output26, HIGH);
 } else if (header.indexOf("GET /26/off") >= 0) {
 Serial.println("GPIO 26 off");
 output26State = "off";
 digitalWrite(output26, LOW);
 } else if (header.indexOf("GET /27/on") >= 0) {
 Serial.println("GPIO 27 on");
 output27State = "on";
 digitalWrite(output27, HIGH);
 } else if (header.indexOf("GET /27/off") >= 0) {
 Serial.println("GPIO 27 off");
 output27State = "off";
 digitalWrite(output27, LOW);

//Read Ultrasonic Sensor

digitalWrite(TrigPin, LOW);
delayMicroseconds(2);
digitalWrite(TrigPin, HIGH);
delayMicroseconds(10);
digitalWrite(TrigPin,LOW);

float duration = pulseIn(EchoPin,HIGH, 30000);

if(duration ==0){
  distanceCm =0;
}
else {
  distanceCm = duration * 0.034 /2;
}
 
}

  client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display:inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border:none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin:2px; cursor: pointer;}");
            client.println(".button2 {background-color:#555555;}</style></head>");
            
            client.println("<body><h1>ESP32 Control & Sensors</h1>");

            // --- ULTRASONIC DATA DISPLAY ---
            client.println("<h2>Sensor Data</h2>");
            if (distanceCm == 0) {
               client.println("<p>Distance: <strong>Out of Range</strong></p>");
            } else {
               client.println("<p>Distance: <strong>" + String(distanceCm) + " cm</strong></p>");
            }
            client.println("<hr>");

 // Display the HTML web page
 client.println("<!DOCTYPE html><html>");
 client.println("<head><meta name=\"viewport\"content=\"width=device-width, initial-scale=1\">");
 client.println("<link rel=\"icon\" href=\"data:,\">");
 // CSS to style the on/off buttons
 // Feel free to change the background-color and font-size
//attributes to fit your preferences
 client.println("<style>html { font-family: Helvetica; display:inline-block; margin: 0px auto; text-align: center;}");
 client.println(".button { background-color: #4CAF50; border:none; color: white; padding: 16px 40px;");
 client.println("text-decoration: none; font-size: 30px; margin:2px; cursor: pointer;}");
 client.println(".button2 {background-color:#555555;}</style></head>");

 // Web Page Heading
 client.println("<body><h1>ESP32 Web Server</h1>");

 // Display current state, and ON/OFF buttons for GPIO 26
 client.println("<p>GPIO 26 - State " + output26State + "</p>");
 // If the output26State is off, it displays the ON

 if (output26State=="off") {
 client.println("<p><a href=\"/26/on\"><buttonclass=\"button\">ON</button></a></p>");
 } else {
 client.println("<p><a href=\"/26/off\"><button class=\"buttonbutton2\">OFF</button></a></p>");
 }

 // Display current state, and ON/OFF buttons for GPIO 27
 client.println("<p>GPIO 27 - State " + output27State + "</p>");
 // If the output27State is off, it displays the ON

 if (output27State=="off") {
 client.println("<p><a href=\"/27/on\"><buttonclass=\"button\">ON</button></a></p>");
 } else {
 client.println("<p><a href=\"/27/off\"><button class=\"buttonbutton2\">OFF</button></a></p>");
 }
 client.println("</body></html>");

 // The HTTP response ends with another blank line
 client.println();
 // Break out of the
 break;
} else { 
  currentLine = "";}
        } else if (c!= '\r'){
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client Disconnected");
    Serial.println("");
  }
}