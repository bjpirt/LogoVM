#include <LogoVM.h>

LogoVM vm = LogoVM(Serial);

/**
 *  Define the functions we are going to use to control the robot
 **/

boolean forward(int distance){
  Serial.print("FORWARD ");
  Serial.println(distance);
  return true;
}

boolean backward(int distance){
  Serial.print("BACKWARD ");
  Serial.println(distance);
  return true;
}

boolean leftTurn(int angle){
  Serial.println("TURNLEFT");
  return true;
}

boolean rightTurn(int angle){
  Serial.println("TURNRIGHT");
  return true;
}

boolean pause(){
  vm.pause();
  return true;
}

boolean play(){
  vm.start();
  return true;
}

boolean ping(){
  Serial.println("PONG");
  return true;
}

void setup(){
  Serial.begin(115200);           // set up Serial library at 11520 bps
  Serial.println("LOGO test!");
  
  //add the commands
  vm.addUserCmd("FORWARD", INTEGER_TYPE, false, (fp) &forward);
  vm.addUserCmd("BACK", INTEGER_TYPE, false, (fp) &backward);
  vm.addUserCmd("TURNLEFT", INTEGER_TYPE, false, (fp) &leftTurn);
  vm.addUserCmd("TURNRIGHT", INTEGER_TYPE, false, (fp) &rightTurn);
  vm.addUserCmd("PAUSE", NO_TYPE, true, (fp) &pause);
  vm.addUserCmd("PLAY", NO_TYPE, true, (fp) &play);
  vm.addUserCmd("PING", NO_TYPE, true, (fp) &ping);
  vm.addUserCmd("REPEAT", REPEAT_TYPE, false, 0);
  vm.addUserCmd("]", END_REPEAT_TYPE, false, 0);
}

void loop() {
  vm.processInput();
  vm.processNextCmd();
}