#include <Arduino.h>
#include <Servo.h>
#include <ESP8266WiFi.h>

WiFiServer server(80); //Set a web server on port 80
Servo myServo; //Servo nesnesi

//WiFi credentials
const char* ssid = "";
const char* password = "";

//Will store the HTTP header
String header;

void setup() {

  // Serial setup
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }


  //WiFi setup
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi..");
    delay(500);
  }
  Serial.println(WiFi.localIP());
  Serial.println("Connection has established.");


  //Server setup
  server.begin();
  Serial.println("Server has begun.");


  //Servo setup (Red: 5V, Brown: GND, Orange: PWM)
  myServo.attach(15);


  //Setup done
  Serial.println("Setup completed.");

}

void loop() {
  //Loop start
  //Servo
  //myServo.write(1);

  //for(int posDegrees = 0; posDegrees <= 180; posDegrees++) {
  //      myServo.write(posDegrees);
  //      Serial.println(posDegrees);
  //      delay(20);
  //  }

  //  for(int posDegrees = 180; posDegrees >= 0; posDegrees--) {
  //      myServo.write(posDegrees);
  //      Serial.println(posDegrees);
  //      delay(20);
  //  }

  //for (int i = 0; i < 180; i++)
  //{
  //  myServo.write(i);
  //  delay(50);
  //}
  

  WiFiClient client = server.available(); //Listen for incoming clients
  int temp = NULL;

  if (client)
  {
    Serial.println("New client.");
    String currentLine = ""; //Will hold incoming data from client
    while (client.connected()) //Loop while client is connected
    {
      if (client.available()) //If there are bytes to read from the client,
      {
        char c = client.read(); //Read a byte
        Serial.write(c);
        header += c;
        if (c == '\n') { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /temp") >= 0)
            {
              Serial.println("Temp isteği geldi");
              int location = header.lastIndexOf("GET"); //Get the location of "GET"
              char temp1 = header.charAt(location+10); //First number of temp
              char temp2 = header.charAt(location+11); //Second number of temp
              if (isDigit(temp1) && isDigit(temp2))
              {
                String tempStr = ""; //Will store the temp
                tempStr += temp1;
                tempStr += temp2;
                temp = tempStr.toInt();
                Serial.println(temp);
                client.println(temp);
              } else
              {
                Serial.println("NaN");
                client.println("NaN");
              }           
              
            }
            
            // The HTTP response ends with another blank line
            client.println();
            //Clear header
            header = "";
            // Break out of the while loop
            break;

          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }

        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
      
    }
    
  }
  
  if (temp != NULL)
  {
    myServo.write(temp);
  }
  
  
  //Loop end
  //Serial.println("Loop bitti");
  //delay(1000);
}