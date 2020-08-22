
#include <DHT.h>
#include <SPI.h>
#include <NRFLite.h>
#include <LowPower.h>

const static uint8_t RADIO_ID = 1;
const static uint8_t DESTINATION_RADIO_ID = 0;
const static uint8_t PIN_RADIO_CE = 10;
const static uint8_t PIN_RADIO_CSN = 9;
const static uint8_t RADIO_CHANNEL = 95;

static const int PIN_SENSOR_DATA = 2;
static const int PIN_SENSOR_POWER = 3;
static const int PIN_SENSOR_GND = 4;

DHT dht(PIN_SENSOR_DATA, DHT11);
NRFLite radio;

struct WeatherPacket {
  float temperature;
  float humidity; 
  byte id;
};

void setup() {
  Serial.begin(115200);

  // for easier wiring, I'm using a couple of I/O pins 
  // to provide power and ground for the temperature sensor.
  pinMode(PIN_SENSOR_POWER, OUTPUT);
  pinMode(PIN_SENSOR_GND, OUTPUT);

  digitalWrite(PIN_SENSOR_POWER, HIGH);
  digitalWrite(PIN_SENSOR_GND, LOW);

  dht.begin();

  const bool result_radio_init = radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE250KBPS, RADIO_CHANNEL);
  
  if (!result_radio_init) {
      Serial.println("Cannot communicate with radio");
  }
}

void loop() {
  const float humidity = dht.readHumidity();
  const float temperature = dht.readTemperature();

  WeatherPacket packet { temperature, humidity, RADIO_ID };
  const bool result_send = radio.send(DESTINATION_RADIO_ID, &packet, sizeof(packet));

  if(result_send) {
    Serial.println("data sent");
  } else {
    Serial.println("send failed");
  }

  radio.powerDown();
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  delay(2000);
}
