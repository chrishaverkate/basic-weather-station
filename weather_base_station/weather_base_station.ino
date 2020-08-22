#include <LiquidCrystal.h>
#include <DHT.h>
#include <SPI.h>
#include <NRFLite.h>

const static uint8_t RADIO_ID = 0;
const static uint8_t PIN_RADIO_CE = 9;
const static uint8_t PIN_RADIO_CSN = 10;
const static uint8_t RADIO_CHANNEL = 95;

static const int PIN_LCD_RS = 2;
static const int PIN_LCD_EN = 3;
static const int PIN_LCD_D4 = 4;
static const int PIN_LCD_D5 = 5;
static const int PIN_LCD_D6 = 6;
static const int PIN_LCD_D7 = 7; 

static const int NUM_ROWS = 2;
static const int NUM_COLUMNS = 16;

LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);
NRFLite radio;

struct WeatherPacket {
  float temperature_celsius;
  float humidity; 
  byte id;
};

void setup() {
  Serial.begin(115200);
  
  lcd.begin(NUM_COLUMNS, NUM_ROWS);
 
  if (!radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE250KBPS, RADIO_CHANNEL)) {
      Serial.println("Cannot communicate with radio");
  }
  Serial.println("TemperatureC,TemperatureF,Humidity");
  lcd.clear();
}

void loop() {

  if(radio.hasData()) {
    WeatherPacket packet = {0.0f, 0.0f, 0};
    radio.readData(&packet);

    const float temperature_fahrenheit = (packet.temperature_celsius * 9/5) + 32;
  
    
    Serial.print(packet.temperature_celsius);
    Serial.print(",");
    Serial.print(temperature_fahrenheit);
    Serial.print(",");
    Serial.println(packet.humidity);
  
    lcd.setCursor(0,0);
    lcd.print(packet.temperature_celsius, 1);
    lcd.write(" C : " );
    lcd.print(temperature_fahrenheit, 1);
    lcd.write(" F" );
  
    lcd.setCursor(0,1);
    lcd.write("Humidity: ");
    lcd.print(static_cast<int>(packet.humidity));
    lcd.print("%");
  }
}
