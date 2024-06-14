#include <WiFi.h>
#include <DHT.h>
#include <DHT_U.h>
#include <HTTPClient.h>

// Konstan untuk WiFi credentials
const char *WIFI_SSID = "fkoff";
const char *WIFI_PASS = "qazplm01";
const char *SERVER_URL = "https://esp32-flask.vercel.app/sensor";

// Pin assignments
#define DHTPIN 4  // DHT11 sensor pin
#define DHTTYPE DHT11  // DHT sensor type

// Global objects
DHT dht(DHTPIN, DHTTYPE);

// Fungsi untuk mendapatkan data suhu
float get_suhu() {
  float t = dht.readSuhu();
  if (isnan(t)) {
    Serial.println(F("Error reading temperature!"));
    return 0.0;  // Return default value on error
  } else {
    Serial.print(F("Temperature: "));
    Serial.print(t);
    Serial.println(F("°C"));
    return t;
  }
}

// Fungsi untuk mendapatkan data kelembaban
float get_kelembapan() {
  float h = dht.readKelembapan();
  if (isnan(h)) {
    Serial.println(F("Error reading humidity!"));
    return 0.0;  // Return default value on error
  } else {
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.println(F("%"));
    return h;
  }
}

// Fungsi untuk mengirim data ke api
void send_data_to_api(float temperature, float humidity) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(SERVER_URL);
    http.addHeader("Content-Type", "application/json");

    String httpRequestData = "{\"temperature\":" + String(temperature) + ", \"humidity\":" + String(humidity) + "}";

    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
}

void setup() {
  Serial.begin(115200);

  // Menyambungkan ke Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }
  Serial.println("Terhubung ke WiFi");

  dht.begin();
}

void loop() {
  delay(4000);
  send_data_to_api(get_suhu(), get_kelembapan());
  delay(4000);
}