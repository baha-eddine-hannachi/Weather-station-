#include <WiFi.h>
#include <DHT11.h>
#include <WebServer.h>

WebServer server(80); 
DHT11 dht(22); // DHT11 sensor on GPIO 22

// HTML page with temperature and humidity display
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { 
            font-family: Arial; 
            text-align: center; 
            margin-top: 50px;
            background-color: #f0f0f0;
        }
        button { 
            padding: 15px 30px; 
            font-size: 20px; 
            margin: 10px; 
            background-color: #4CAF50;
            color: white;
            border: none;
            border-radius: 5px;
            cursor: pointer;
        }
        .container { 
            max-width: 600px; 
            margin: 0 auto;
            background-color: white;
            padding: 30px;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
        }
        .sensor-data {
            display: flex;
            justify-content: space-around;
            margin: 30px 0;
        }
        .sensor-box {
            padding: 20px;
            border-radius: 10px;
            min-width: 150px;
        }
        .temperature {
            background-color: #ffebee;
            color: #c62828;
            border: 2px solid #ef9a9a;
        }
        .humidity {
            background-color: #e8f5e8;
            color: #2e7d32;
            border: 2px solid #a5d6a7;
        }
        .value {
            font-size: 36px;
            font-weight: bold;
            margin: 10px 0;
        }
        .unit {
            font-size: 20px;
            color: #666;
        }
        h2 {
            color: #333;
            margin-bottom: 5px;
        }
        .last-update {
            color: #666;
            font-size: 14px;
            margin-top: 20px;
        }
        .refresh-btn {
            background-color: #2196F3;
        }
    </style>
    <script>
        function updateSensorData() {
            fetch('/sensor')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('temperature').innerHTML = data.temperature;
                    document.getElementById('humidity').innerHTML = data.humidity;
                    document.getElementById('lastUpdate').innerHTML = 'Last updated: ' + new Date().toLocaleTimeString();
                })
                .catch(error => console.error('Error:', error));
        }
        
        function refreshData() {
            updateSensorData();
            alert('Refreshing sensor data...');
        }
        
        // Update data every 5 seconds
        setInterval(updateSensorData, 5000);
        
        // Initial load
        window.onload = updateSensorData;
    </script>
</head>
<body>
    <div class="container">
        <h1> ESP32 Web Server</h1>
        <p>Connected to ESP32 WiFi!</p>
        
        <div class="sensor-data">
            <div class="sensor-box temperature">
                <h2> Temperature</h2>
                <div class="value">
                    <span id="temperature">--</span>
                    <span class="unit">°C</span>
                </div>
            </div>
            
            <div class="sensor-box humidity">
                <h2> Humidity</h2>
                <div class="value">
                    <span id="humidity">--</span>
                    <span class="unit">%</span>
                </div>
            </div>
        </div>
        
        <p id="lastUpdate" class="last-update">Waiting for data...</p>
        
        <button onclick="alert('Hello from ESP32!')"> Click Me</button>
        <button onclick="refreshData()" class="refresh-btn"> Refresh Data</button>
        <button onclick="updateSensorData()"> Update Now</button>
        
        <p>IP: 192.168.4.1</p>
        <p> Network: MyESP32</p>
    </div>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleSensorData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  // Check if readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    String error = "{\"error\":\"Sensor read failed\"}";
    server.send(500, "application/json", error);
    return;
  }
  
  // Create JSON response
  String jsonResponse = "{";
  jsonResponse += "\"temperature\":\"" + String(temperature, 1) + "\",";
  jsonResponse += "\"humidity\":\"" + String(humidity, 1) + "\"";
  jsonResponse += "}";
  
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  
  server.send(200, "application/json", jsonResponse);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("Setting up ESP32 as Access Point...");
  
  // Initialize DHT sensor
  Serial.println("Initializing DHT11 sensor...");
  
  // Set WiFi to Access Point mode
  WiFi.mode(WIFI_AP);
  
  // Create WiFi network
  bool result = WiFi.softAP("MyESP32", "12345678");
  
  if (result) {
    Serial.println("✅ Access Point created successfully!");
    Serial.print("📡 Network Name (SSID): ");
    Serial.println("MyESP32");
    Serial.print("🔑 Password: ");
    Serial.println("12345678");
    Serial.print("📍 IP address: ");
    Serial.println(WiFi.softAPIP());  // Should be 192.168.4.1
    
    // Setup web server routes
    server.on("/", handleRoot);
    server.on("/sensor", handleSensorData); 
    
    server.begin();
    Serial.println("🌐 Web server started!");
    Serial.println("📱 Now look for 'MyESP32' in your phone's WiFi list");
    Serial.println("📊 Access the dashboard at http://192.168.4.1");
  } else {
    Serial.println("❌ Failed to create Access Point!");
  }
}

void loop() {
  server.handleClient();  // Handle web requests
  
  // Optional: Print sensor data to serial every 5 seconds
  static unsigned long lastRead = 0;
  if (millis() - lastRead > 5000) {
    lastRead = millis();
    
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    
    if (!isnan(temp) && !isnan(hum)) {
      Serial.print("[Periodic Read] Temperature: ");
      Serial.print(temp);
      Serial.print("°C, Humidity: ");
      Serial.print(hum);
      Serial.println("%");
    }
  }
  
  delay(2);  // Small delay for stability
}