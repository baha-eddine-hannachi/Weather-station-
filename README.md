
 

ESP32 IoT Web Server
====================
<img width="1918" height="956" alt="image" src="https://github.com/user-attachments/assets/0b884059-5777-4a01-97ff-b3336c82df6a" />


A real-time environmental monitoring web server with DHT11 temperature/humidity sensor, built on ESP32 microcontroller with WiFi Access Point capabilities.

ESP32 Arduino IDE IoT WiFi AP Web Server DHT11

[View Code](#code) [Live Demo](#demo) [Setup Guide](#setup) [Hardware](#hardware)

Project Overview
----------------

This project creates a standalone IoT web server using an ESP32 microcontroller. The device acts as a WiFi Access Point, hosting a responsive web dashboard that displays real-time temperature and humidity data from a DHT11 sensor. Users can connect to the ESP32's WiFi network and access the dashboard from any web browser.

**Key Innovation:** The ESP32 creates its own WiFi network, eliminating the need for an existing WiFi infrastructure. This makes it perfect for remote monitoring or temporary installations.

### Core Features

### Standalone Access Point

Creates its own WiFi network (SSID: `MyESP32`) with password protection. No router needed!

### Real-time Monitoring

Continuously reads temperature (°C) and humidity (%) data from DHT11 sensor with automatic updates.

### Responsive Web Dashboard

Modern, mobile-friendly web interface with live data visualization and interactive controls.

### Auto-Refresh & Manual Controls

Data auto-refreshes every 5 seconds with manual refresh buttons for instant updates.

### Error Handling & Logging

Comprehensive error handling with serial monitoring for debugging and status updates.

### Mobile Compatible

Fully responsive design that works on smartphones, tablets, and desktop computers.

Web Interface Preview
---------------------

The ESP32 serves a modern, responsive web dashboard accessible at `http://192.168.4.1` after connecting to its WiFi network.

192.168.4.1 - ESP32 Dashboard

### 🌡️ ESP32 Web Server

Connected to ESP32 WiFi!

Temperature

24.5°C

Celsius

Humidity

65.2%

Relative

_Last updated: Just now_

Click Me Refresh Data

**IP:** 192.168.4.1

**Network:** MyESP32

Hardware Requirements
---------------------

### ESP32 Development Board

Main microcontroller with built-in WiFi

### DHT11 Sensor

Temperature and humidity sensor

### Breadboard & Jumper Wires

For connecting components

### Micro USB Cable

For programming and power

### Circuit Diagram

**Connection Guide:**

*   **DHT11 VCC** → ESP32 3.3V or 5V pin
*   **DHT11 GND** → ESP32 GND pin
*   **DHT11 DATA** → ESP32 GPIO 22 (can be changed in code)
*   **DHT11 NC** → Not connected

A pull-up resistor (4.7kΩ - 10kΩ) between DATA and VCC is recommended for stable communication.

Setup & Installation
--------------------

### 1\. Install Arduino IDE & ESP32 Board

Install Arduino IDE and add ESP32 board support via Board Manager (URL: `https://espressif.github.io/arduino-esp32/package_esp32_index.json`).

### 2\. Install Required Libraries

Install the following libraries via Arduino Library Manager:

*   `DHT sensor library` by Adafruit
*   `Adafruit Unified Sensor` library
*   ESP32 WiFi library (included with board package)

### 3\. Connect Hardware

Wire the DHT11 sensor to ESP32 as shown in the hardware section. Connect ESP32 to computer via USB.

### 4\. Upload Code

Copy the provided code, select **ESP32 Dev Module** as board, choose correct COM port, and upload.

### 5\. Connect & Access

Open Serial Monitor (115200 baud), look for WiFi network "MyESP32", connect with password "12345678", then open browser to `http://192.168.4.1`.

**Success Indicators:**

*   Serial Monitor shows "✅ Access Point created successfully!"
*   WiFi network "MyESP32" appears in available networks
*   Dashboard loads at `http://192.168.4.1` showing sensor data
*   Serial Monitor displays periodic sensor readings

Source Code
-----------

The complete Arduino sketch for the ESP32 IoT web server:

main.ino - ESP32 Web Server with DHT11

Copy Code

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
            <h1>🌡️ ESP32 Web Server</h1>
            <p>Connected to ESP32 WiFi!</p>
            
            <div class="sensor-data">
                <div class="sensor-box temperature">
                    <h2>🌡️ Temperature</h2>
                    <div class="value">
                        <span id="temperature">--</span>
                        <span class="unit">°C</span>
                    </div>
                </div>
                
                <div class="sensor-box humidity">
                    <h2>💧 Humidity</h2>
                    <div class="value">
                        <span id="humidity">--</span>
                        <span class="unit">%</span>
                    </div>
                </div>
            </div>
            
            <p id="lastUpdate" class="last-update">Waiting for data...</p>
            
            <button onclick="alert('Hello from ESP32!')">👋 Click Me</button>
            <button onclick="refreshData()" class="refresh-btn">🔄 Refresh Data</button>
            <button onclick="updateSensorData()">📊 Update Now</button>
            
            <p>📡 IP: 192.168.4.1</p>
            <p>🌐 Network: MyESP32</p>
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

**Important Notes:**

*   Change `GPIO 22` if your DHT11 is connected to a different pin
*   Modify `SSID` and `password` in `WiFi.softAP()` for security
*   Ensure you have the correct DHT library installed (DHT11, not DHT22)
*   If using DHT22, change `#include <DHT11.h>` to `#include <DHT.h>`

Customization & Extensions
--------------------------

### Easy Modifications

### UI Customization

Modify the HTML/CSS in the `htmlPage` string to change colors, layout, or add new UI elements.

### Additional Sensors

Add more sensors (MQ gas sensors, motion detectors, light sensors) and display their data on the dashboard.

### Station Mode

Modify to connect to existing WiFi instead of creating an Access Point for
