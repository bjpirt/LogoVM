#include <LogoVM.h>

LogoVM vm = LogoVM(Serial);

/**
 *  Define the functions we are going to use to control the robot
 **/

boolean forward(FnContext *context){
  Serial.print("FORWARD ");
  Serial.println(context->arg);
  return true;
}

boolean backward(FnContext *context){
  Serial.print("BACKWARD ");
  Serial.println(context->arg);
  return true;
}

boolean leftTurn(FnContext *context){
  Serial.println("TURNLEFT");
  return true;
}

boolean rightTurn(FnContext *context){
  Serial.println("TURNRIGHT");
  return true;
}

boolean pause(FnContext *context){
  vm.pause();
  return true;
}

boolean play(FnContext *context){
  vm.start();
  return true;
}

boolean ping(FnContext *context){
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
