#include <QTRSensors.h>

// This example is designed for use with eight QTR-1RC sensors or the eight sensors of a
// QTR-8RC module.  These reflectance sensors should be connected to digital inputs 3 to 10.
// The QTR-8RC's emitter control pin (LEDON) can optionally be connected to digital pin 2,
// or you can leave it disconnected and change the EMITTER_PIN #define below from 2 to
// QTR_NO_EMITTER_PIN.

// The main loop of the example reads the raw sensor values (uncalibrated).
// You can test this by taping a piece of 3/4" black electrical tape to a piece of white
// paper and sliding the sensor across it.  It prints the sensor values to the serial
// monitor as numbers from 0 (maximum reflectance) to 2500 (minimum reflectance).


#define NUM_SENSORS 6  // number of sensors used
#define TIMEOUT 2500   // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN 2  // emitter is controlled by digital pin 2
#define DEBUG 0
bool kontrol = false;
// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[6]){ A0, A1, A2, A3, A4, A5 }, NUM_SENSORS, TIMEOUT, EMITTER_PIN);
int deger = 50;
unsigned int sensorValues[NUM_SENSORS];
int duzcizgi = 0;

void setup() {
  Serial.begin(9600);  // set the data rate in bits per second for serial data transmission

  int i;
  for (i = 0; i < 250; i++) {
    qtrrc.calibrate(QTR_EMITTERS_ON);
    delay(20);
  }

  if (DEBUG) {
    Serial.begin(9600);
    for (int i = 0; i < NUM_SENSORS; i++) {
      Serial.print(qtrrc.calibratedMinimumOn[i]);
      Serial.print(' ');
    }
    Serial.println();

    for (int i = 0; i < NUM_SENSORS; i++) {
      Serial.print(qtrrc.calibratedMaximumOn[i]);
      Serial.print(' ');
    }
    Serial.println();
    Serial.println();
  }
}


void loop() {
  // read raw sensor values
  qtrrc.read(sensorValues);
  int position = qtrrc.readLine(sensorValues, QTR_EMITTERS_ON, 1);
  // print the sensor values as numbers from 0 to 2500, where 0 means maximum reflectance and
  // 2500 means minimum reflectance
  Serial.print(position);
  Serial.print("\t");
  for (unsigned char i = 0; i < NUM_SENSORS; i++) {
    Serial.print(sensorValues[i]);
    Serial.print('\t');  // tab to format the raw data into columns in the Serial monitor
  }
  if (sensorValues[0] < deger && sensorValues[1] < deger && sensorValues[2] < deger && sensorValues[3] < deger && sensorValues[4] < deger && sensorValues[5] < deger) {
    duzcizgi = duzcizgi + 1;
  }

Serial.print(duzcizgi);
Serial.println();

delay(250);
}
