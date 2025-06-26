#include <WiFiNINA.h>

char buffer[100]; // Buffer to store received data

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

WiFiServer server(80); // Web server object
WiFiClient client; // Client object

float pHValue = 0.0;
float dissolvedOxygenValue = 0.0;
float turbidityValue = 0.0;
float temperatureValue = 0.0;

void setup()
{
  Serial.begin(115200); // Serial monitor for debugging

  connectToWiFi();

  server.begin();
}

void loop()
{
  client = server.available();

  if (client)
  {
    if (client.connected())
    {
      // Process the received data
      while (client.available())
      {
        char c = client.read();
        buffer[strlen(buffer)] = c;

        if (c == '\n')
        {
          // Remove the newline character at the end
          buffer[strlen(buffer) - 1] = '\0';

          // Split the buffer at the ':' character
          char* token;
          token = strtok(buffer, ":");

          while (token != NULL)
          {
            // Assuming the data is in the format "Key: Value", extract the key and value separately
            char* key = token;
            token = strtok(NULL, ":");
            char* value = token;

            // Store the individual values in separate variables
            if (key && value)
            {
              if (strcmp(key, "pH") == 0)
              {
                pHValue = atof(value);
              }
              else if (strcmp(key, "Dissolved Oxygen") == 0)
              {
                dissolvedOxygenValue = atof(value);
              }
              else if (strcmp(key, "Turbidity") == 0)
              {
                turbidityValue = atof(value);
              }
              else if (strcmp(key, "Temperature") == 0)
              {
                temperatureValue = atof(value);
              }
              // Add more conditions for other keys if needed
            }

            token = strtok(NULL, ":"); // Move to the next pair of key-value
          }

          // Reset the buffer for the next request
          memset(buffer, 0, sizeof(buffer));

          // Send the response to the client
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.println("<html>");
          client.println("<body>");
          client.print("Received data:<br>");
          client.print("pH: ");
          client.println(pHValue);
          client.print("Dissolved Oxygen: ");
          client.println(dissolvedOxygenValue);
          client.print("Turbidity: ");
          client.println(turbidityValue);
          client.print("Temperature: ");
          client.println(temperatureValue);
          client.println("</body>");
          client.println("</html>");

          break;
        }
      }

      client.stop();
    }
  }
}

void connectToWiFi()
{
  WiFi.begin((char*)ssid, password); // Convert ssid to non-const char array

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
