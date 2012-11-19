#include <SoftwareSerial.h>

// software serial #1: TX = digital pin 2, RX = digital pin 3
SoftwareSerial xbee(2, 3);

unsigned long nextreport;

#define REPORTINT 10000UL

#define SAMPLES 20
#define SENSA 0
#define SENSB 1
#define SENSW 2
float sample[3][SAMPLES];
byte  next[3];

void clearsamples(int sens) {
  for (int i = 0; i < SAMPLES; i++) sample[sens][i] = 0;
  next[sens] = 0;
}

float readsens(int sens) {
  float sum = 0;
  for (int i = 0; i < SAMPLES; i++) sum += sample[sens][i];
  return sum / SAMPLES;
}

float lowpass(int sens, float value) {
  sample[sens][next[sens]] = value;
  next[sens] = (next[sens] + 1) % SAMPLES;
  return readsens(sens);
}

// http://hacking.majenko.co.uk/node/57
float readVcc() {
  long result; // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result/1000;
}

int aread(int pin) {
  int sv;
  analogRead(pin);
  delay(50);
  sv = analogRead(pin);
  
  return sv;
}

float readTemp(int sensor) {
  float Vcc = readVcc();
  float tempC;
  tempC = (aread(sensor) * (Vcc / 1023.0) * 100) - 273.15;
  
  return tempC;
}

void setup() {
  Serial.begin(9600);
  xbee.begin(115200);
  xbee.println();
  xbee.println("abort");
  xbee.println();
  clearsamples(SENSA);
  clearsamples(SENSB);
  clearsamples(SENSW);
  delay(1000);
  
  // Wait for all data to be gone
  while (xbee.available() > 0) {
    xbee.read();
  }
  nextreport = millis() + REPORTINT;
}

void loop() {
  float tempa, tempb, weight;
  
  // read the analog in value:
  weight = lowpass(SENSW, aread(A5));
  tempa =  lowpass(SENSA, readTemp(A0));
  tempb =  lowpass(SENSB, readTemp(A1));
  
  if (millis() > nextreport) {
    nextreport = millis() + REPORTINT;
    xbee.print("http://admin01.int.tildaslash.com/fridge/temp.php?tempA=");
    xbee.print(tempa);
    xbee.print("&tempB=");
    xbee.print(tempb);
    xbee.print("&weight=");
    xbee.println(weight);
  }
    
  delay(50);
}
