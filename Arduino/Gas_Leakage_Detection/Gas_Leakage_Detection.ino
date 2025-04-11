#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "Shravani";
const char* password = "tmkoc1234";

//Your Domain name with URL path or IP address with path
String serverName = "http://ezyeve.com/project24/rfidshopping/gasleakage.php";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;


#include <Servo.h>
Servo servo;
int myservo = D1;

int buzzer = D2;

int pin = A0;

float sensorValue;  //variable to store sensor value

void setup()
{
  Serial.begin(9600); // sets the serial port to 9600
  servo.attach(myservo);
  servo.write(0);
  delay(1000);

  pinMode(buzzer, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop()
{
  sensorValue = analogRead(pin); // read analog input pin 0

  Serial.println("");
  
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);

  // Send an HTTP POST request depending on timerDelay
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      Serial.println("");

      String serverPath = serverName + "?value=" + String(sensorValue);

      Serial.print(serverPath);
      Serial.println("");
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.print("Payload: ");
        Serial.print(payload);

        if (sensorValue<=99)
        {
          Serial.println("");
          Serial.println("Gas is not detected");
          Serial.println("Door is Closed");
          delay(1000);
          digitalWrite(buzzer,LOW);
          //Serial.println("Degree 270");
          servo.write(180);
        }

        else if (100<sensorValue)
        {
          Serial.println("");
          Serial.println("Gas is detected");
          Serial.println("Door is Open");
          digitalWrite(buzzer,HIGH);
          //Serial.println("Degree 270");
          servo.write(270);

          // delay(2000);
          // digitalWrite(buzzer,LOW);
        }

        // else if (470<=sensorValue<=520)
        // {
        //   Serial.println("");
        //   Serial.println("Smoke is detected");
        //   digitalWrite(buzzer,HIGH);
        //   delay(2000);
        //   digitalWrite(buzzer,LOW);
        // }
      }
      else {
        Serial.println("");
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
  }
}