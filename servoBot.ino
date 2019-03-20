#include <Servo.h>

Servo myservo;  // create servo object to control a servo

byte action = 0;
byte actionDetach = 3;
byte pos = 0; // 0 to 90
byte ByteReceived;

byte minPos = 0;
byte maxPos = 50;
byte servoWaitDelay = 10;

int servoPin = 3;
int buttonPin = 4;
int ledPin = 13;

void setup() {  
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  Serial.begin(9600);
  Serial.println("Serial listening");
  
  Serial.println("Initiating Servo");
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  myservo.write(minPos);
  delay(500);
  action = actionDetach; 
}
int ultimaAcaoBotao = 0;
int buttonState = 0;
int ultimoButtonState = LOW;
void loop() {
  bool showPos = false;
  if (Serial.available() > 0){
    int b = Serial.read();
    if(b == '0' || b == 0) action = 0;
    if(b == '1' || b == 1) action = 1;
    if(b == '2' || b == 2) action = 2;
    if(b == '4' || b == 4) action = 4;
    if(b == 'p' || b == 'P') action = 'p';
  }
  if(action == 1 || action == 2 || action == 4){
    if(!myservo.attached()) myservo.attach(servoPin);
  }
  if(action == 0) myservo.detach();
    
  if(action == 1 || action == 4){      
      if(++pos >= maxPos) {
        pos = maxPos;        
        if(action == 4) action = 5;
        else action = actionDetach;
      }
      //showPos = true;
      myservo.write(pos);
      delay(servoWaitDelay);
  }
  if(action == 2 || action == 5){
      if(pos > 0) pos--; // evita dar a volta
      if(pos <= minPos+1) {
        pos = minPos;
        if(action == 5) action = 4; 
        else action = actionDetach;
      }
      //showPos = true;
      myservo.write(pos);
      delay(servoWaitDelay);
  }
  if(action == actionDetach){
      delay(servoWaitDelay*3);
      action = 0;
  }
  if(action == 'p'){ showPos = true;action=0;}
  if(showPos){    
    Serial.print("Servo: ");
    Serial.println(pos);
  }

  // Aciona com botão
  buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH && ultimoButtonState == LOW){
    // Apertou e não entra de novo até soltar
    if(ultimaAcaoBotao == 0) {
      action = 1;
      ultimaAcaoBotao = 1;
    }else{
      action = 2;
      ultimaAcaoBotao = 0;
    }
  }
  digitalWrite(ledPin, buttonState);
  ultimoButtonState = buttonState;
}
