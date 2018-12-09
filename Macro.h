
#include"MotorPins.h"

String firstArea, secondArea;

/*------------------------------------*/
// MOTOR 1 ALT mEbable = 12 olan
// MOTOR 2 SAĞ mEbable = 6 olan
// MOTOR 3 SOL mEbable = 9 olan

/* MAKRO ARRAY'LER İÇİN DİKKAT EDİLMESİ GEREKEN ARRAY SIRALAMASI => [0]SAĞ MOTOR [1]SOL MOTOR [2]ALT MOTOR OLARAK SIRALANMIŞTIR.  */

int mEnable[3] = {m2Enable, m3Enable, m1Enable};
int mDir[3] = {m2Direction, m3Direction, m1Direction};
int mStp[3] = {m2Step, m3Step, m1Step};
//int mRot[3] = {LOW, HIGH, LOW};
//int mRot2[3] = {HIGH, LOW, HIGH};


int delayTimeMotor = 350;
int alGotur[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int coordinate[3] = {0, 0, 0};        //[0]SAĞ [1]SOL [2]ALT
int myHome[3] = {104, -161, 0};         //[0]SAĞ [1]SOL [2]ALT
int startPosition[3] = {0, 0, 0};     //[0]SAĞ [1]SOL [2]ALT

int index = 0;

//AREA 0=> item box koordinatları diğerleri soldan sağa olan item area koordinatları
int area[9][3] = {{ -31, -126, -102}, { -136, -101, -74}, { -28, -29, -51}, { -128, -96, -24}, { -35, -45, 2}, { -110, -86, 29}, { -32, -33, 54}, { -116, -89, 78}, { -23, -36, 101}} ;
