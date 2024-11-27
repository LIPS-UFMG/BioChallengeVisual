#include <Stepper.h>

//definitions for each command to be recieved via serial port
#define COMMAND_1 '1'
#define COMMAND_3 '3'
#define COMMAND_4 '4'
#define COMMAND_6 '6'
#define COMMAND_7 '7'
#define COMMAND_9 '9'
#define COMMAND_8 '8'
#define COMMAND_2 '2'

#define COMMAND_STOP ' '

//enter the steps per rev for your motors here
int stepsInRev = 200;

//this sets the value for the for loops and therefore sets the amount of steps in each call
int num_of_steps = 1;
// setup pins for each driver motor1 ~ IN1, IN2, IN3, IN4; motor2 ~ IN1, IN2, IN3, IN4
Stepper motorbottom(stepsInRev, 13, 11, 12, 10);
Stepper motormid(stepsInRev, 9, 7, 8, 6);
Stepper motortop(stepsInRev, 2, 4, 3, 5);
// variable to store the last call to the serial port
char lastCall = ' ';


void bottomleft(int steps){
  motorbottom.step(1);
}
void bottomright(int steps){
  motorbottom.step(-1);
}
void midleft(int steps){
  motormid.step(1);
}
void midright(int steps){
  motormid.step(-1);
}
void topleft(int steps){
  motortop.step(1);
}
void topright(int steps){
  motortop.step(-1);
}
void bothleft(int steps){
  motortop.step(1);
  motormid.step(1);
}
void bothright(int steps){
  motortop.step(-1);
  motormid.step(-1);
}

void allStop() {
  Serial.println("stop");
  // Definir os pinos individualmente como LOW para desligar os motores
  for (int pin = 0; pin <= 7; pin++) {
    digitalWrite(pin, LOW);  // Pinos 0 a 7 (Stepper 1)
  }
  for (int pin = 8; pin <= 13; pin++) {
    digitalWrite(pin, LOW);  // Pinos 8 a 13 (Stepper 2)
  }
}

void setup() {
  Serial.begin(9600);//start the bluetooth serial port - send and recieve at 9600 baud
  // set the speed at 60 rpm:
  motortop.setSpeed(60);
  motormid.setSpeed(60);
  motorbottom.setSpeed(60);
}

void loop() {
//check to see if there is serial communication and if so read the data
if(Serial.available()) {
char data = (char)Serial.read();
// switch to set the char via serial to a command
switch(data) {
  case COMMAND_1:
    bottomleft(num_of_steps);
    break; 
  case COMMAND_3:
    bottomright(num_of_steps);
    break;
  case COMMAND_4:
    midleft(num_of_steps);
    break;
  case COMMAND_6:
    midright(num_of_steps);
    break;
  case COMMAND_7:
    topleft(num_of_steps);
    break;
  case COMMAND_9:
    topright(num_of_steps);
    break;
  case COMMAND_8:
    bothleft(num_of_steps);
    break;
  case COMMAND_2:
    bothright(num_of_steps);
    break;
  case COMMAND_STOP:
    allStop();
    break;
}
// set the 'lastCall' variable to the last call from the serial
lastCall = data;
}
else{
char data = lastCall;
switch(data) {
  case COMMAND_1:
    bottomleft(num_of_steps);
    break; 
  case COMMAND_3:
    bottomright(num_of_steps);
    break;
  case COMMAND_4:
    midleft(num_of_steps);
    break;
  case COMMAND_6:
    midright(num_of_steps);
    break;
  case COMMAND_7:
    topleft(num_of_steps);
    break;
  case COMMAND_9:
    topright(num_of_steps);
    break;
  case COMMAND_8:
    bothleft(num_of_steps);
    break;
  case COMMAND_2:
    bothright(num_of_steps);
    break;
  case COMMAND_STOP:
    allStop();
    break;
}
lastCall = data;
}
}
