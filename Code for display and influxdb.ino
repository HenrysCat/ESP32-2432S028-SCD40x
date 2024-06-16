#include <WiFi.h>
#include <Wire.h>
#include <SparkFun_SCD4x_Arduino_Library.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <TFT_eSPI.h> // Include the graphics library

TFT_eSPI tft = TFT_eSPI(); // Create a TFT_eSPI object

#define DEVICE "SCD40"
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "password"
#define INFLUXDB_URL "http://influxdb-server-ip:8086"
#define INFLUXDB_TOKEN "ifluxdb-token"
#define INFLUXDB_ORG "org-name"
#define INFLUXDB_BUCKET "bucket-name"
#define TZ_INFO "GMT+0BST-1,M3.5.0/01:00:00,M10.5.0/02:00:00"

// Custom I2C pins
#define SDA_PIN 27
#define SCL_PIN 22

// InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Data point
Point sensor_db("SCD4x");

SCD4x mySensor;

void setup() {
  Serial.begin(115200);
  Serial.println(F("SCD4x Example"));

  // Setup WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Wait for WiFi connection
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  // Initialize the SCD4x sensor
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!mySensor.begin()) {
    Serial.println(F("Sensor not detected. Please check wiring. Continuing without sensor..."));
  }

  // Initialize the TFT display
  tft.init();
  tft.setRotation(1); // Set the rotation if needed
  tft.fillScreen(TFT_BLACK); // Clear the screen to black
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // Set text color to white with a black background
}

void loop() {
  if (mySensor.readMeasurement()) {
    // Clear fields for reusing the point. Tags will remain untouched
    sensor_db.clearFields();

    // Get CO2 reading and add 20
    float co2_reading = mySensor.getCO2() + 20;
    float temperature = mySensor.getTemperature();
    float humidity = mySensor.getHumidity();

    // Store measured values into point
    sensor_db.addField("co2_ppm", co2_reading);
    sensor_db.addField("temperature_c", temperature);
    sensor_db.addField("humidity_rh", humidity);

    // Print measurements for debugging
    Serial.print("CO2: ");
    Serial.print(co2_reading);
    Serial.print(" ppm, ");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, ");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    // Clear the screen for new readings
    tft.fillScreen(TFT_BLACK);

    // Display CO2 reading in large font at the top center
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // Set text color to white with a black background
    tft.setTextSize(3);
    tft.setCursor(64, 50); // Adjust coordinates as needed
    tft.printf("CO2: %.0fppm", co2_reading);

    // Display temperature and humidity in smaller font below CO2 reading
    tft.setTextSize(3);
    tft.setCursor(46, 100); // Adjust coordinates as needed
    tft.printf("Temp: %.2f C", temperature);

    tft.setCursor(18, 150); // Adjust coordinates as needed
    tft.printf("Humidity: %.2f%%", humidity);

    // Check WiFi connection and reconnect if needed
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi connection lost");
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }

    // Write point
    if (!client.writePoint(sensor_db)) {
      Serial.print("InfluxDB write failed: ");
      Serial.println(client.getLastErrorMessage());
    } else {
      Serial.println("Data sent to InfluxDB");
    }
  } else {
    Serial.print(F("."));
  }

  delay(10000); // Wait for 10 seconds between measurements
}
