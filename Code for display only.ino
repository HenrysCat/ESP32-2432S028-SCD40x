#include <Wire.h>
#include <SparkFun_SCD4x_Arduino_Library.h>
#include <TFT_eSPI.h> // Include the graphics library

TFT_eSPI tft = TFT_eSPI(); // Create a TFT_eSPI object

#define DEVICE "SCD40"

// Custom I2C pins
#define SDA_PIN 27
#define SCL_PIN 22

SCD4x mySensor;

void setup() {
  Serial.begin(115200);
  Serial.println(F("SCD4x Example"));

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
    // Get CO2 reading and add 20
    float co2_reading = mySensor.getCO2() + 20;
    float temperature = mySensor.getTemperature();
    float humidity = mySensor.getHumidity();

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

    // Display CO2 reading at the top center
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // Set text color to white with a black background
    tft.setTextSize(3);
    tft.setCursor(64, 50); // Adjust coordinates as needed
    tft.printf("CO2: %.0fppm", co2_reading);

    // Display temperature and humidity below CO2 reading
    tft.setTextSize(3);
    tft.setCursor(46, 100); // Adjust coordinates as needed
    tft.printf("Temp: %.2f C", temperature);

    tft.setCursor(18, 150); // Adjust coordinates as needed
    tft.printf("Humidity: %.2f%%", humidity);
  } else {
    Serial.print(F("."));
  }

  delay(60000); // Wait for 60 seconds between measurements
}
