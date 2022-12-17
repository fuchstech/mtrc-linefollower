//Çizgi izleyen robot
//www.robimek.com

#include <QTRSensors.h>
/*
#define SolMotorileri 11
#define SolMotorGeri 8
#define SagMotorileri 9 
#define SagMotorGeri  10
*/
#define SolMotorileri 9
#define SolMotorGeri 10
#define SagMotorileri 11
#define SagMotorGeri 8

#define ena 6
#define enkb 5

#define KP 0.022

#define KD 4
#define M1_minumum_hiz 60
#define M2_minumum_hiz 60
#define M1_maksimum_hiz 180
#define M2_maksimum_hiz 180
#define MIDDLE_SENSOR 4, 5
#define NUM_SENSORS 8
#define TIMEOUT 2500
#define EMITTER_PIN 2
#define DEBUG 0
int lastError = 0;
int last_proportional = 0;
int integral = 0;
QTRSensorsRC qtrrc((unsigned char[8]){ A7, A6, A5, A4, A3, A2, A1, A0 }, NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];

void setup() {
  pinMode(SolMotorileri, OUTPUT);
  pinMode(SolMotorGeri, OUTPUT);
  pinMode(SagMotorileri, OUTPUT);
  pinMode(SagMotorGeri, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(enkb, OUTPUT);
  delay(1000);
  manual_calibration();
  set_motors(0, 0);
}

void loop() {
  unsigned int sensors[8];
  int position = qtrrc.readLine(sensors, 1, 0);
  int error = position - 3500;

  int motorSpeed = KP * error + KD * (error - lastError);
  lastError = error;

  int leftMotorSpeed = M1_minumum_hiz + motorSpeed;
  int rightMotorSpeed = M2_minumum_hiz - motorSpeed;

  set_motors(leftMotorSpeed, rightMotorSpeed);
}

void set_motors(int motor1speed, int motor2speed) {
  if (motor1speed > M1_maksimum_hiz) motor1speed = M1_maksimum_hiz;  //MAKSİMUM MOTOR 1 HIZ LİMİTİ
  if (motor2speed > M2_maksimum_hiz) motor2speed = M2_maksimum_hiz;  // MAKSİMUM MOTOR 2 HIZ LİMİTİ
  if (motor1speed < 0) motor1speed = 0;                              // MİNIMUMMOTOER 1 HIZ LİMİTİ
  if (motor2speed < 0) motor2speed = 0;                              // MİNİMUM MOTOR 2 HIZ LİMİTİ
  analogWrite(ena, motor1speed);
  analogWrite(enkb, motor2speed);
  digitalWrite(SolMotorileri, HIGH);
  digitalWrite(SagMotorileri, HIGH);
  digitalWrite(SolMotorGeri, LOW);
  digitalWrite(SagMotorGeri, LOW);
}

void manual_calibration() {

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