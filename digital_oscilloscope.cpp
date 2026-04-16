#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SIGNAL_PIN 34
#define BUTTON_UP 25
#define BUTTON_DOWN 26
#define BUTTON_RUNSTOP 27

int timeDivision = 2;
int voltageDivision = 1;
bool runOscilloscope = true;

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_RUNSTOP, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("Digital Oscilloscope");
  display.display();
  delay(2000);
}

void loop() {

  if (digitalRead(BUTTON_UP) == LOW) {
    voltageDivision++;
    if (voltageDivision > 5) voltageDivision = 5;
    delay(200);
  }

  if (digitalRead(BUTTON_DOWN) == LOW) {
    voltageDivision--;
    if (voltageDivision < 1) voltageDivision = 1;
    delay(200);
  }

  if (digitalRead(BUTTON_RUNSTOP) == LOW) {
    runOscilloscope = !runOscilloscope;
    delay(300);
  }

  if (runOscilloscope) {
    display.clearDisplay();

    for (int x = 0; x < 128; x++) {
      int sensorValue = analogRead(SIGNAL_PIN);
      float voltage = sensorValue * (3.3 / 4095.0);

      int y = map(sensorValue, 0, 4095, 63, 0);
      display.drawPixel(x, y, SSD1306_WHITE);

      delay(timeDivision);
    }

    display.setCursor(0, 0);
    display.print("V:");
    display.print(voltageDivision);
    display.print("X");

    display.setCursor(70, 0);
    display.print("T:");
    display.print(timeDivision);

    display.setCursor(0, 56);
    display.print("RUN");

    display.display();
  }
  else {
    display.clearDisplay();
    display.setCursor(30, 20);
    display.setTextSize(2);
    display.println("STOP");
    display.display();
  }
}
