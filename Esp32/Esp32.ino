#include <WiFi.h>

const char* ssid = ""; //SSID
const char* password = ""; //PWSD

// Button setup
const int buttonPin = 4;
int buttonState = HIGH;
int lastButtonState = HIGH;
int toggleState = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Setup button pin
  pinMode(buttonPin, INPUT_PULLUP);

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int reading = digitalRead(buttonPin);

  // Debounce logic
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && buttonState == HIGH) {
      toggleState = !toggleState;
     // Serial.println(toggleState);
      Serial.println(String(toggleState) + ": " + (toggleState ? "Available" : "Unavailable"));

    }
    buttonState = reading;
  }

  lastButtonState = reading;
}
