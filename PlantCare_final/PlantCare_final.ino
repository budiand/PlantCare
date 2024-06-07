#include <LCD_I2C.h>
#include <SD.h>
#include <SPI.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include "characters.h"
#include "lcd_print.h"
#include "SD_functions.h"


#define INT0_PIN 2
#define pump_pin 3
#define SD_CS 4
#define speed 150
#define moisture_sensor_vcc_pin 7
#define moisture_sensor_data_pin A0

// temperature
#define ONE_WIRE_BUS 8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature temperature(&oneWire);

// lcd
LCD_I2C lcd(0x3F);

int moisture_value;
float temp_value;
unsigned long moisture_start, moisture_finished, moisture_elapsed_time;
unsigned long SD_START, SD_STOP, SD_TIME;

File file;

void setup() {

  Serial.begin(9600);
  // interrupt
  pinMode(INT0_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(INT0_PIN), INT0_ISR, RISING);

  // temperature
  temperature.begin();

  // pump
  pinMode(pump_pin, OUTPUT);
  analogWrite(pump_pin, 0);

  // soil_moisture
  pinMode(moisture_sensor_vcc_pin, OUTPUT);

  // lcd
  lcd.begin();
  lcd.backlight();
  lcd.createChar(0, circle);
  lcd.createChar(1, plant);
  lcd.createChar(2, heart);
  lcd.clear();

  // sd
  if (!SD.begin(SD_CS)) {
    Serial.println("initialization failed!");
  }
  
  SD_START = millis();
  SD_STOP = millis();
  SD_TIME = SD_STOP - SD_START;

  file = SD.open("test.txt", FILE_WRITE);
}

void loop() {

  // stop from reading humidity data
  if(digitalRead(moisture_sensor_vcc_pin) == HIGH) {
    moisture_start = millis();
    moisture_value = analogRead(moisture_sensor_data_pin);
    digitalWrite(moisture_sensor_vcc_pin, LOW);
  }

  moisture_finished = millis();
  moisture_elapsed_time = moisture_finished - moisture_start;

  // after an hour start the humidity sensor
  if(moisture_elapsed_time >= 3600000) {
    digitalWrite(moisture_sensor_vcc_pin, HIGH);
    moisture_value = analogRead(moisture_sensor_data_pin);
    // delay for stability
    delay(2000);
    moisture_value = analogRead(moisture_sensor_data_pin);
  }

    // get temperature
  temperature.requestTemperatures();
  temp_value = temperature.getTempCByIndex(0);

  // print to LCD display
  print_lcd_message(lcd, moisture_value, temp_value);

  // check humidity and start pump if needed
  if(moisture_value > 800) {
    digitalWrite(INT0_PIN, LOW);
    delay(1);
    digitalWrite(INT0_PIN, HIGH);
    
    // water for 5 seconds
    delay(5000);
    analogWrite(pump_pin, 0);
    delay(3000);

    // write to SD how much time took from last watering
    SD_START = SD_STOP;
    SD_STOP = millis();
    SD_TIME = SD_STOP - SD_START;
    write_time(SD_START, SD_STOP, SD_TIME, file);
  }
}

void INT0_ISR() {
  analogWrite(pump_pin, speed);
}
