#include <TimerOne.h>

#define MEASURE_PERIOD 80  
#define numStepsMotor 400  
#define TENSOPIN A0

int timeCount;  // счетчик времени
long  sumU; // переменные для суммирования кодов АЦП
long  averageU; // сумма кодов АЦП (среднее значение * 80)
int currentStep;  // текущее положение двигателя
int setStep;  // заданное положение двигателя

int stepPin = 7;
int dirPin = 8;

void motorStep(int k){
  if (k > 0)      digitalWrite(dirPin, LOW);
  else if(k < 0)  digitalWrite(dirPin, HIGH);
  
  int e = abs(k);
  
  for(int i=0;i<4*e;i++)
  {
      digitalWrite(stepPin, HIGH);
      delay(1);
      digitalWrite(stepPin, LOW);
      delay(1);
  }
}

void setup() {
Serial.begin(9600);
Timer1.initialize(250);  // инициализация таймера 1, период 250 мкс
Timer1.attachInterrupt(timerInterrupt, 250);  // обработчик прерываний
}

void loop() {

setStep = averageU * (numStepsMotor - 1) / 1023 / MEASURE_PERIOD;

int stepsToDo;

stepsToDo = currentStep - setStep;

if( abs(stepsToDo) >= (numStepsMotor / 2) ) {

      if((stepsToDo) > 0) stepsToDo -= numStepsMotor;
      else                stepsToDo += numStepsMotor;
}


motorStep(stepsToDo);  


currentStep = setStep; 

Serial.println(stepsToDo);
//delay(200);
}

void  timerInterrupt() {
  //myMotor.control(); // управвление двигателем

  sumU += analogRead(A0);  // суммирование кодов АЦП
  timeCount++;  // +1 счетчик выборок усреднения

  // проверка числа выборок усреднения
  if ( timeCount >= MEASURE_PERIOD ) {
    timeCount= 0;
    averageU= sumU; // перегрузка среднего значения 
    sumU= 0;
    }

}
