
#include <WaziDev.h>
#include <xlpp.h>
#include <DHT.h>

int sensor_pin = A0;
int buzzer_pin = 4;
int LED_pin = 2;
int DHTPIN = 3;
int DHTTYPE = 11;

//int sensor_pin, buzzer_pin, LED_pin, DHTPIN, DHTTYPE = A0, 4, 2, 5, 11;
DHT dht(DHTPIN, DHTTYPE);
unsigned char loRaWANKey[16] = { 0x25, 0x15, 0x8D, 0x3B, 0xBC, 0x31, 0xE6, 0xAF, 0x67, 0x0D, 0x19, 0x5B, 0x5A, 0xED, 0x55, 0x25 };
unsigned char devAddr[4] = { 0xBB, 0x01, 0x1D, 0x87 };

WaziDev wazidev;

void setup() {
  Serial.begin(9600);
  pinMode(sensor_pin, INPUT);
  pinMode(buzzer_pin, OUTPUT);
  pinMode(LED_pin, OUTPUT);
  dht.begin();
  wazidev.setupLoRaWAN(devAddr, loRaWANKey);
}

XLPP xlpp(120);

void loop(void) {
  xlpp.reset();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("Temperature = ");
  Serial.println(temperature);

  Serial.print("Humidity = ");
  Serial.println(humidity);

  // put your main code here, to run repeatedly:
  int sensor_value = analogRead(sensor_pin);
  Serial.print("Gas = ");
  Serial.println(sensor_value);
  delay(500);
  if (sensor_value >= 500) {
    digitalWrite(LED_pin, HIGH);
    delay(500);
    digitalWrite(LED_pin, LOW);
    delay(500);
    digitalWrite(buzzer_pin, HIGH);
    delay(1000);
    digitalWrite(buzzer_pin, LOW);
    delay(1000);
    Serial.println("GAS DETECTED!");
  } else {
    Serial.println("GAS AT NORMAL LEVELS...");
  }

  Serial.println("");

  xlpp.addTemperature(1, 20.3);  // °C
  xlpp.addVoltage(2, 12.5);      // V
  xlpp.addString(3, "Hello :D");

  serialPrintf("LoRaWAN send ... ");
  int e = wazidev.sendLoRaWAN(xlpp.getBuffer(), xlpp.getSize());
  if (e == 0)
    serialPrintf("OK\n");
  else
    serialPrintf("Err %d\n", e);
  delay(60000);
}
