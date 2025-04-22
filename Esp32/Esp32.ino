#include <WiFi.h>
#include <HTTPClient.h>

// Wi-Fi credentials
const char* ssid = ""; //SSID
const char* password = ""; //PWSD

// Firebase Realtime Database
const String firebaseHost = ""; // firebase Host
const String firebasePath = ""; // firebase Path

// Button setup
const int buttonPin = 4;
int buttonState = HIGH;
int lastButtonState = HIGH;
int toggleState = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long lastReadTime = 0;
unsigned long readInterval = 5000; // every 5 seconds

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(buttonPin, INPUT_PULLUP);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int reading = digitalRead(buttonPin);

  // Debounce
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && buttonState == HIGH) {
      toggleState = !toggleState;

      Serial.println(String(toggleState) + ": " + (toggleState ? "Available" : "Unavailable"));

      updateFirebase(toggleState);  // Send update to Firebase
    }
    buttonState = reading;
  }

  lastButtonState = reading;

  // Periodically read from Firebase
  if (millis() - lastReadTime >= readInterval) {
    readFromFirebase();  // Read and print available_spot
    lastReadTime = millis();
  }
}

void updateFirebase(bool availability) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = firebaseHost + firebasePath;

    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    String jsonData = availability ? "true" : "false";

    int httpResponseCode = http.PUT(jsonData);

    if (httpResponseCode > 0) {
      Serial.print("Firebase update successful: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error updating Firebase: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}

void readFromFirebase() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = firebaseHost + firebasePath;

    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.print("Current value of available_spot: ");
      Serial.println(payload);
    } else {
      Serial.print("Error reading from Firebase: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}
