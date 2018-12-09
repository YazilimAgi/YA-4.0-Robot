#include <SoftwareSerial.h>
#include "MotorPins.h"
#include "SignalValues.h"
#include "Macro.h"
#include <Servo.h>

#define motorDelayTimeMilliSeconds 25000
//Bluetooth Bacakları
#define bluetoothTx 2
#define bluetoothRx 3
int stepperCount = 2;
//bool check = false;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
Servo mServo;

void resetArray(int a[]) {
  int i = 0;
  while (a[i] != '\0') {
    a[i] = 0;
    i++;
  }

}
void resetArduino() {
  stepperCount = 2;
  firstArea = "";
  secondArea = "";
  resetArray(alGotur);
  resetArray(coordinate);
  index = 0;

}
void setup() {
  Serial.print("--YAZILIM AĞI YA 4.0-- ");
  Serial.begin(9600);
  setPinModes();
  disableAllMotors();
  enableAllMotors();
  bluetooth.begin(38400);
}

void loop() {
  String readedData = readFromBluetooth();



  if (readedData.length() > 0) {
    Serial.print("DATA:");
    Serial.println(readedData);



    if (readedData == m1GeriBT) {
      Serial.println("Motor 1 Geri Yönde");
      motorStepStart(m1Enable, m1Step, m1Direction, LOW, m1DurBT, stepperCount);
      coordinate[2] += stepperCount;
      showCoordinate();

    }

    else if (readedData == m1IleriBT) {
      Serial.println("Motor 1 İleri Yönde");
      motorStepStart(m1Enable, m1Step, m1Direction, HIGH, m1DurBT, stepperCount);
      coordinate[2] -= stepperCount;
      showCoordinate();

    }

    else if (readedData == m2GeriBT) {
      Serial.println("Motor 2 Geri Yönde");
      motorStepStart(m2Enable, m2Step, m2Direction, LOW, m2DurBT, stepperCount);
      coordinate[0] += stepperCount;
      showCoordinate();

    }

    else if (readedData == m2IleriBT) {
      Serial.println("Motor 2 İleri Yönde");
      motorStepStart(m2Enable, m2Step, m2Direction, HIGH, m2DurBT, stepperCount);
      coordinate[0] -= stepperCount;
      showCoordinate();

    }

    else if (readedData == m3GeriBT) {
      Serial.println("Motor 3 Geri Yönde");
      motorStepStart(m3Enable, m3Step, m3Direction, LOW, m3DurBT, stepperCount);
      coordinate[1] += stepperCount;
      showCoordinate();

    }

    else if (readedData == m3IleriBT) {
      Serial.println("Motor 3 İleri Yönde");
      motorStepStart(m3Enable, m3Step, m3Direction, HIGH, m3DurBT, stepperCount);
      coordinate[1] -= stepperCount;
      showCoordinate();

    }

    else if (readedData == mA) {
      Serial.println("MakroA");
      PointFromPoint(coordinate, myHome);
    }

    else if (readedData == mB) {
      Serial.println("MakroB");
      PointFromPoint(coordinate, startPosition);
    }

    else if (readedData == mC) {
     
    }

    else if (readedData == mD) {

    }


    else if (readedData == mE) {

    }


    else if (readedData[2] == '/') {
      parseString(readedData);
      PointFromPoint(coordinate, myHome);
      takeAndBreak(firstArea.toInt(), secondArea.toInt());
    }



    else if (readedData[1] == '-') {
      parseArea(readedData);
      olaniTopla();
      PointFromPoint(coordinate, myHome);


    }
    else if (readedData == "13") {
      disableAllMotors();
      Serial.println("Disable");
    }

    else if (readedData == "14") {
      enableAllMotors();
      Serial.println("Enable");

    }
    else if (readedData == servoOpen) {
      Serial.println("Servo AÇ");
      mServo.write(95);
    }


    else if (readedData == servoClose) {
      Serial.println("Servo Kapat");
      mServo.write(30);
    }

    else if (readedData == "reset") {
      Serial.println("RESET");
      resetArduino();
    }
    else if (readedData[0] == '$') {
      String deger = "";
      int i = 1;
      while (readedData[i] != '\0') {
        deger += readedData[i];
        i++;

      }

      stepperCount = deger.toInt();
      Serial.println(stepperCount);

    }

    else {
      Serial.print("Farklı bir şey geldi, gelen veri -> ");
      Serial.println(readedData);

    }
  }
}

void parseString(String data) {
  firstArea = "";
  secondArea = "";
  for (int i = 0; i < 5; i++)
  {
    if (i < 2)
      firstArea += data[i];
    else if (i > 2)
      secondArea += data[i];

  }

}
void enableAllMotors() {
  //Tüm mtorları durdur!
  digitalWrite(m1Enable, LOW);
  digitalWrite(m2Enable, LOW);
  digitalWrite(m3Enable, LOW);
}

void disableAllMotors() {
  //Tüm mtorları durdur!
  digitalWrite(m1Enable, HIGH);
  digitalWrite(m2Enable, HIGH);
  digitalWrite(m3Enable, HIGH);
}


void PointFromPoint(int firstPoint[], int secondPoint[]) {
  int temp[3];
  // sağ sol alt
  int rotation[3] = {HIGH, HIGH, HIGH};

  for (int i = 0; i < 3; i++) {
    temp[i] = firstPoint[i] - secondPoint[i];
    if (temp[i] < 0) {
      rotation[i] = LOW;
      temp[i] *= -1;
    }
  }

  startMakroMotors(mEnable, mDir, rotation, mStp, temp);
}



void startMakroMotors(int mToggle[], int mDirection[], int rotation[], int mStepPin[], int mStepCount[]) {
  index = 0;
  while (mToggle[index] != '\0') {
    digitalWrite(mToggle[index], LOW);//enable et
    digitalWrite(mDirection[index], rotation[index]);
    index++;
  }
  index = 0;
  while (index < mStepCount[0] || index < mStepCount[1] || index < mStepCount[2]) {
    for (int i = 0; i < 3; i++) {
      if (index < mStepCount[i]) {
        digitalWrite(mStepPin[i], HIGH);
        delayMicroseconds(50000);
        digitalWrite(mStepPin[i], LOW);
        delayMicroseconds(50000);
        if (rotation[i] == LOW) {
          coordinate[i]++;
        }
        if (rotation[i] == HIGH) {
          coordinate[i]--;
        }

      }

    }
    delay(15);
    index++;
  }
  showCoordinate();
}
void motorCalistir(int mToggle, int mStep, int mDirection, int rotation, String mDurBT) {

  Serial.print("mToggle=");
  Serial.println(mToggle);
  digitalWrite(mToggle, LOW);//enable et
  digitalWrite(mDirection, rotation);//yönünü söyle
  //---------DÖNGÜ--------------------
  //bir steplik kod (4 satır)

  while (true) {
    digitalWrite(mStep, HIGH);
    delayMicroseconds(motorDelayTimeMilliSeconds);
    //delay(5);
    digitalWrite(mStep, LOW);
    delayMicroseconds(motorDelayTimeMilliSeconds);
    //delay(5);

    String readedData = readFromBluetooth();
    if (readedData == mDurBT) {
      Serial.println(readedData);

      break;
    }
  }

  //----------------------------------
  //digitalWrite(mToggle, HIGH);//disable et
}

void motorStepStart(int mToggle, int mStep, int mDirection, int rotation, String mDurBT, int stepCount) {

  Serial.print("mToggle=");
  Serial.println(mToggle);

  digitalWrite(mToggle, LOW);//enable et
  digitalWrite(mDirection, rotation);//yönünü söyle
  //---------DÖNGÜ--------------------
  //bir steplik kod (4 satır)

  for (int i = 0; i < stepCount; i++) {
    digitalWrite(mStep, HIGH);
    delayMicroseconds(motorDelayTimeMilliSeconds);
    digitalWrite(mStep, LOW);
    delayMicroseconds(motorDelayTimeMilliSeconds);

  }
  //----------------------------------
  //digitalWrite(mToggle, HIGH);//disable et
}

void showCoordinate() {
  Serial.print("SAG Count: ");
  Serial.print(coordinate[0]);
  Serial.print(" SOL Count: ");
  Serial.print(coordinate[1]);
  Serial.print(" ALT Count: ");
  Serial.println(coordinate[2]);

}

String readFromBluetooth() {
  String readString = "";
  while (bluetooth.available()) {
    //delay(3);
    char c = bluetooth.read();
    readString += c;
  }
  readString.trim();
  if (readString.length() > 0) {
    Serial.println(readString);
  }
  return readString;
}


void setPinModes() {
  mServo.attach(13);
  pinMode(m1Enable, OUTPUT);
  pinMode(m1Step, OUTPUT);
  pinMode(m1Direction, OUTPUT);
  pinMode(m2Enable, OUTPUT);
  pinMode(m2Step, OUTPUT);
  pinMode(m2Direction, OUTPUT);
  pinMode(m3Enable, OUTPUT);
  pinMode(m3Step, OUTPUT);
  pinMode(m3Direction, OUTPUT);
}

void olaniTopla() {
  Serial.println("ALANI TOPLA");
  for (int i = 0; i < 8; i ++) {
    if (alGotur[i] == 1)
      takeAndBreak(i + 1, 0);
  }
}

void parseArea(String str) {
  Serial.println("PARSER");
  int x = 0;
  for (int i = 0; i < 16; i += 2, x++)  {
    if (str[i] == '1')
      alGotur[x] = 1;
    else
      alGotur[x] = 0;
  }
  for (int i = 0; i < 8; i ++) {
    if (alGotur[i] == 1)
      Serial.print(i);
    Serial.print(" ");
  }
  Serial.println(" ");
}

void takeAndBreak(int taken, int breaken) {
  int temp[3] = {coordinate[0], coordinate[1], area[taken][2]};
  PointFromPoint(coordinate, temp);
  delay(delayTimeMotor);
  mServo.write(180);//servo aç
  PointFromPoint(coordinate, area[taken]);
  delay(delayTimeMotor);
  mServo.write(25);//servo KAPA
  delay(delayTimeMotor);
  int temp4[3] = {myHome[0], myHome[1], coordinate[2]};
  PointFromPoint(coordinate, temp4);
  delay(delayTimeMotor);
  // PointFromPoint(coordinate, myHome);
  // delay(500);
  int temp2[3] = {coordinate[0], coordinate[1], area[breaken][2]};
  PointFromPoint(coordinate, temp2);
  delay(delayTimeMotor);
  PointFromPoint(coordinate, area[breaken]);
  delay(delayTimeMotor);
  mServo.write(180);//servo aç
  delay(delayTimeMotor);
  int temp3[3] = {myHome[0], myHome[1], coordinate[2]};
  PointFromPoint(coordinate, temp3);
  delay(delayTimeMotor);
  //PointFromPoint(coordinate, myHome);
}
