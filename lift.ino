#include "SevSeg.h"
#include <SoftPWM.h>
#include <StackArray.h>
#include <QueueArray.h>

StackArray <int> stack;
SevSeg sevseg;
QueueArray <int> queue;

int gndflr = 2;
int floor1 = 3;
int floor2 = 4;
int drclose = 6;
int motorup = 7;
int motordown = 8;
int sensegnd = 9;
int sensefl1 = 10;
int sensefl2 = 11;
//int ledup = 11;
//int leddown = 12;
int lastfloor, nextfloor, currentfloor, flr1, flr2, flr0, upnextflr, drrclose, foo,i;

void setup()
{
  pinMode(gndflr, INPUT);
  pinMode(floor1, INPUT);
  pinMode(floor2, INPUT);
  pinMode(sensegnd, INPUT);
  pinMode(sensefl1, INPUT);
  pinMode(sensefl2, INPUT);
  pinMode(drclose, INPUT);
  pinMode(motorup, OUTPUT);
  pinMode(motordown, OUTPUT);
  //pinMode(ledup , OUTPUT);
  //pinMode(leddown, OUTPUT);
  Serial.begin(9600);
  flr0 = 0;
  flr1 = 1;
  flr2 = 2;
  digitalWrite(motorup, LOW);
  digitalWrite(motordown, LOW);
  //digitalWrite(ledup, LOW);
  //digitalWrite(leddown, LOW);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop()
{

  Serial.print("upnextfloor- ");
  Serial.println(upnextflr);

  //nextfloor = stack.pop();
  //nextfloor = currentfloor;

  digitalWrite(motorup, LOW);
  digitalWrite(motordown, LOW);
  sensefloor();
  readbuttons();
    if (queue.isEmpty ())
  {
    return;
  }
  //nextfloor = upnextflr;
  else
  {
  nextfloor = queue.dequeue();
  }
  elevate();
  //delay(1000);
}

void sensefloor()
{
  if (digitalRead(sensegnd) == LOW)
  {
    Serial.println("current floor - ground ");
    currentfloor = flr0;
    Serial.print("current floor- ");
    Serial.println(currentfloor);
    print0();
  }
  else if (digitalRead(sensefl1) == LOW)
  {
    Serial.println("current floor - first ");
    currentfloor = flr1;
    Serial.print("current floor- ");
    Serial.println(currentfloor);
    print1();
  }
  else if (digitalRead(sensefl2) == LOW)
  {
    Serial.println("current floor - second ");
    currentfloor = flr2;
    Serial.print("current floor- ");
    Serial.println(currentfloor);
    print2();
  }
  else
  {
    Serial.println("moving");
  }
}

void readbuttons()
{
  if (digitalRead(gndflr) == LOW)
  {
    Serial.println("button pressed - ground ");
    //stack.push (flr0);
    queue.enqueue (flr0);
    //nextfloor = flr0;
    Serial.print("nextfloor- ");
    Serial.println(nextfloor);
    
  }
  else if (digitalRead(floor1) == LOW)
  {
    Serial.println("button pressed - first ");
    //stack.push (flr1);
    queue.enqueue (flr1);
    //nextfloor = flr1;
    Serial.print("nextfloor- ");
    Serial.println(nextfloor);
  }
  else if (digitalRead(floor2) == LOW)
  {
    Serial.println("button pressed - second ");
    //stack.push (flr2);
    queue.enqueue (flr2);
    //nextfloor = flr2;
    Serial.print("nextfloor- ");
    Serial.println(nextfloor);
  }
  else
  {
    Serial.println("No button pressed");
    //nextfloor = upnextflr;
    Serial.print("nextfloor- ");
    Serial.println(nextfloor);
  }
}

void upnext()
{
  if (digitalRead(gndflr) == LOW)
  {
    Serial.print("upnextfloor- ");
    //upnextflr = flr0;
    queue.enqueue (flr0);
    //stack.push (flr0);
    Serial.println(upnextflr);
  }
  else if (digitalRead(floor1) == LOW)
  {
    Serial.print("upnextfloor- ");
    //upnextflr = flr1;
    queue.enqueue (flr1);
    //stack.push (flr1);
    Serial.println(upnextflr);
  }
  else if (digitalRead(floor2) == LOW)
  {
    Serial.print("upnextfloor- ");
    //upnextflr = flr2;
    queue.enqueue (flr2);
    //stack.push (flr2);
    Serial.println(upnextflr);
  }
  else
  {
    //upnextflr = currentfloor;
  }
  /*if(currentfloor < upnextflr < nextfloor)
    {
    foo = nextfloor;
    nextfloor = upnextflr;
    upnextflr = foo;
    }
    else if(currentfloor > upnextflr > nextfloor)
    {
    foo = nextfloor;
    nextfloor = upnextflr;
    upnextflr = foo;
    } */
  Serial.print("upnextfloor- ");
  Serial.println(upnextflr);
  Serial.print("nextfloor- ");
  Serial.println(nextfloor);
}
void elevate()
{
  sensefloor();
  drrclose = digitalRead(drclose);
  if (drrclose == HIGH)
  {
    //delay(1000);
    Serial.println("closed");
    //nextfloor = stack.pop();
    if (currentfloor != nextfloor)
    {
      if (currentfloor < nextfloor)
      {
        motorhigh();
        //digitalWrite(motorup, LOW);
        //digitalWrite(motordown, LOW);
      }
      else if (currentfloor > nextfloor)
      {
        motorlow();
        //digitalWrite(motordown, LOW);
        //digitalWrite(motorup, LOW);
      }
      else
      {
        return;
      }
    }
  }
  else
  {
    Serial.println("open");
  }
}

void motorhigh()
{
  sensefloor();
  drrclose = digitalRead(drclose);
  //delay(1000);
  do
  {
    //readbuttons();

    sensefloor();
    drrclose = digitalRead(drclose);
    digitalWrite(motorup, HIGH);
    digitalWrite(motordown, LOW);
    Serial.println("UP");
    sensefloor();
    upnext();
   // digitalWrite(ledup, HIGH);
    //readbuttons();
  }
  while (currentfloor < nextfloor && drrclose == HIGH);
  digitalWrite(motorup, LOW);
  digitalWrite(motordown, LOW);
  //digitalWrite(ledup, LOW);
  int i = 0;
  while( i<=30)
  {
    i++;
    delay(100);
    upnext();
  }
  //delay(3000);
}

void motorlow()
{
  sensefloor();
  drrclose = digitalRead(drclose);
  //delay(1000);
  do
  {
    //readbuttons();

    sensefloor();
    drrclose = digitalRead(drclose);
    digitalWrite(motordown, HIGH);
    digitalWrite(motorup, LOW);
    Serial.println("DOWN");
    //digitalWrite(leddown, HIGH);
    sensefloor();
    upnext();
    //readbuttons();
  }
  while (currentfloor > nextfloor && drrclose == HIGH);
  digitalWrite(motorup, LOW);
  digitalWrite(motordown, LOW);
  //digitalWrite(leddown, LOW);
  int i = 0;
  while(i<=30)
  {
    i++;
    delay(100);
    upnext();
  }
  //delay(3000);
}

void resetfloor()
{
  while (currentfloor != flr0)
  {
    digitalWrite(motordown, HIGH);
    sensefloor();
  }
}

void print2()
{
  digitalWrite(A0, 0);
  digitalWrite(A1, 0);
  digitalWrite(A2, 1);
  digitalWrite(A3, 0);
  digitalWrite(A4, 0);
  digitalWrite(A5, 1);
  digitalWrite(13, 0);
}

void print1()
{
  digitalWrite(A0, 1);
  digitalWrite(A1, 0);
  digitalWrite(A2, 0);
  digitalWrite(A3, 1);
  digitalWrite(A4, 1);
  digitalWrite(A5, 1);
  digitalWrite(13, 1);
}

void print0()
{
  digitalWrite(A0, 0);
  digitalWrite(A1, 0);
  digitalWrite(A2, 0);
  digitalWrite(A3, 0);
  digitalWrite(A4, 0);
  digitalWrite(A5, 0);
  digitalWrite(13, 1);
}

