#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int output1 = 5;
const int output2 = 6;
const int buttonFreq = 2;
const int buttonWidth = 3;
const int buttonTrigger = 4;

int frequency = 1; // Hz
int pulseWidth = 5; // milliseconds

bool lastTriggerState = HIGH;

// ✅ Δηλώνουμε πρώτα τη συνάρτηση
void updateDisplay() {
  display.clearDisplay();
  display.setCursor(10, 0);
  display.setTextSize(1);
  display.println("Xray Gen Simulator");
  display.setTextSize(1);
  display.setCursor(0, 25);
  display.print("Freq: ");
  display.print(frequency);
  display.print(" Hz");
  display.setCursor(0, 40);
  display.print("Width: ");
  display.print(pulseWidth);
  display.print(" ms");
  display.display();
}

void setup() {
  pinMode(output1, OUTPUT);
  pinMode(output2, OUTPUT);
  pinMode(buttonFreq, INPUT_PULLUP);
  pinMode(buttonWidth, INPUT_PULLUP);
  pinMode(buttonTrigger, INPUT_PULLUP);

  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println("Xray Gen Simulator");
  display.display();
  delay(1500);
  updateDisplay();
}

void loop() {
  if (digitalRead(buttonFreq) == LOW) {
    delay(200);
    frequency++;
    if (frequency > 10) frequency = 1;
    Serial.print("Frequency: ");
    Serial.println(frequency);
    updateDisplay();
  }

  if (digitalRead(buttonWidth) == LOW) {
    delay(200);
    pulseWidth += 5;
    if (pulseWidth > 1000) pulseWidth = 5;
    Serial.print("Width: ");
    Serial.println(pulseWidth);
    updateDisplay();
  }

  bool currentTriggerState = digitalRead(buttonTrigger);
  if (lastTriggerState == HIGH && currentTriggerState == LOW) {
    int interval = 1000 / frequency;

    digitalWrite(output1, HIGH);
    digitalWrite(output2, HIGH);
    delay(pulseWidth);

    digitalWrite(output1, LOW);
    digitalWrite(output2, LOW);
    delay(pulseWidth);

    digitalWrite(output2, LOW);
    delay(interval - 2 * pulseWidth);

    display.clearDisplay();
    display.setCursor(10, 0);
    display.setTextSize(1);
    display.println("Xray Gen Simulator");
    display.setTextSize(1);
    display.setCursor(0, 18);
    display.println("Triggered!");
    display.setCursor(0, 32);
    display.println("Exposure!");
    display.display();

    int barWidth = 100;
    int barHeight = 10;
    int barX = 14;
    int barY = 50;

    for (int i = 0; i <= barWidth; i += 5) {
      display.fillRect(barX, barY, i, barHeight, SSD1306_WHITE);
      display.display();
      delay(50);
    }

    updateDisplay();
  }
  lastTriggerState = currentTriggerState;
}