#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <WiFiClient.h>

//const char* ssid = "Vodafone-14E2";
//const char* password = "Summer@23";

const char* ssid = "yourwifi";
const char* password = "yourpass";


#define IO_HOST  "io.adafruit.com"
#define IO_PORT  yourport              

#define IO_USERNAME  "Dhruv010"
#define IO_KEY       "yourkey"

WiFiClient wifiClient;
Adafruit_MQTT_Client mqttClient(&wifiClient, IO_HOST, IO_PORT, IO_USERNAME, IO_KEY);

Adafruit_MQTT_Publish pHp = Adafruit_MQTT_Publish(&mqttClient, IO_USERNAME "/feeds/ph");
Adafruit_MQTT_Publish dOp = Adafruit_MQTT_Publish(&mqttClient, IO_USERNAME "/feeds/dissolved-oxygen");
Adafruit_MQTT_Publish turbp = Adafruit_MQTT_Publish(&mqttClient, IO_USERNAME "/feeds/turbidity");
Adafruit_MQTT_Publish tempp = Adafruit_MQTT_Publish(&mqttClient, IO_USERNAME "/feeds/temperature");

float pH, dO, turb, temp;

void connectToWiFi()
{
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void connectToMQTT()
{
  while (!mqttClient.connected())
  {
    Serial.print("Connecting to MQTT... ");

    int8_t ret = mqttClient.connect();

    if (ret == 0)
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(ret);
      Serial.println(", try again in 5 seconds...");
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  connectToWiFi();
  connectToMQTT();
}

void publishSensorData()
{
  // Publish sensor data to MQTT topics
  char message[50]; // Buffer to store the sensor data as a string
   //String message; 
   //message = String(pH, 2);
  //Serial.print("pH: ");
  //Serial.println(message);
  //pHp.publish(message.c_str());
  
  // Convert sensor data to string format
  dtostrf(pH, 4, 2, message);
  //Serial.print(message);
  mqttClient.publish("Dhruv010/feeds/ph", message);
  //pHp.publish(message);
  delay(5000);

  //snprintf(message, sizeof(message), "%.2f", pH);
  //Serial.print("pH: ");
  //Serial.println(message);
  //pHp.publish(message);
  
  dtostrf(dO, 5, 2, message);
  mqttClient.publish("Dhruv010/feeds/dissolved-oxygen", message);
  //dOp.publish(message);
  delay(5000);
  
  dtostrf(turb, 5, 2, message);
  mqttClient.publish("Dhruv010/feeds/turbidity", message);
  //turbp.publish(message);
  delay(5000);
  
  dtostrf(temp, 5, 2, message);
  mqttClient.publish("Dhruv010/feeds/temperature", message);
  //tempp.publish(message);
  delay(5000); 
}

void parseSerialData(String data)
{
  //Serial.print("Received data: ");
  //Serial.println(data);

  int pHIndex = data.indexOf("pH:");
//Serial.println("pH label found"); // Add this line
if (pHIndex != -1) {
  int newlineIndex = data.indexOf("\n", pHIndex);
  //Serial.print(newlineIndex);
  //Serial.println("Newline character found"); // Add this line
  if (newlineIndex == -1) {
    String pHString = data.substring(pHIndex + 3, newlineIndex);
    pH = pHString.toFloat();
    //Serial.print("pH Index: ");
    //Serial.println(pHIndex);
    //Serial.print("New line Index: ");
    //Serial.println(newlineIndex);
    //Serial.print("pH String: ");
    //Serial.println(pHString);
    Serial.print("pH: ");
    Serial.println(pH);
    // ...
  }
  // ...
}

 int doIndex = data.indexOf("Dissolved Oxygen:");
  if (doIndex != -1) {
    int newlineIndex = data.indexOf("\n", doIndex);
    if (newlineIndex == -1) {
      String doString = data.substring(doIndex + 17, newlineIndex);
      dO = doString.toFloat();
      Serial.print("Dissolved Oxygen: ");
      Serial.println(dO);
    }
  }

  int turbidityIndex = data.indexOf("Turbidity:");
  if (turbidityIndex != -1) {
    int newlineIndex = data.indexOf(",", turbidityIndex);
    if (newlineIndex == -1) {
      String turbString = data.substring(turbidityIndex + 10, newlineIndex);
      turb = turbString.toFloat();
      Serial.print("Turbidity: ");
      Serial.println(turb);
    }
  }


  int temperatureIndex = data.indexOf("Temperature:");
  if (temperatureIndex != -1) {
    int newlineIndex = data.indexOf(",", temperatureIndex);
    if (newlineIndex == -1) {
      String tempString = data.substring(temperatureIndex + 12, newlineIndex);
      temp = tempString.toFloat();
      Serial.print("Temperature: ");
      Serial.println(temp);
    }
  }
 }

void processSerialData()
{
  static char buffer[100]; // Buffer to store received data
  static uint8_t bufferIndex = 0;

  while (Serial.available())
  {
    char c = Serial.read();
    //Serial.print(c);

    if (c == '\n')
    {
      parseSerialData(String(buffer));
      bufferIndex = 0; // Reset the buffer index for the next message
    }
    else
    {
      buffer[bufferIndex] = c;
      bufferIndex++;
      // Check if the buffer is full and handle accordingly
      if (bufferIndex >= sizeof(buffer))
      {
        bufferIndex = sizeof(buffer) - 1; // Truncate the buffer to prevent overflow
      }
    }
  }
}

void loop()
{
  if (!mqttClient.connected())
  {
    connectToMQTT();
  }

  mqttClient.processPackets(1000); // Process MQTT packets to maintain the connection

  processSerialData(); // Call the processSerialData() function first
  //delay(5000);
  //Serial.println(pH);
  // Publish the sensor data to the MQTT topics
  publishSensorData();
}
