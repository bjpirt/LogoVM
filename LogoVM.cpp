#include "Arduino.h"
#include "LogoVM.h"

LogoVM::LogoVM(Stream &s) {
  _s = &s;
  fn_counter = 0;
  input_buffer_pos = 0;
  cmd_write_pos = 0;
  cmd_read_pos = 0;
  running = false;
  subroutine_pos = -1;
  nest_level = 0;
}

void LogoVM::start(){
  running = true;
}

void LogoVM::pause(){
  running = true;
}

// This gets called each loop and is responsible for kicking off any commands
void LogoVM::processInput(){
  int arg;
  if (_s->available() > 0){
    char incomingByte = _s->read();
    if(incomingByte == '\r' || incomingByte == '\n'){
      // we have a message in the buffer
      input_buffer[input_buffer_pos] = '\0';
      storeCmd(input_buffer);
      //reset the buffer because we've processed this line
      input_buffer_pos = 0;
    }else{
      // add the character to the buffer
      input_buffer[input_buffer_pos] = incomingByte;
      input_buffer_pos++;
    }
  }
}

// This allows you to register a callback funtion
void LogoVM::addUserCmd(char* cmd, byte type, boolean meta, fp fn){
  if (fn_counter == COMMAND_COUNT) {
    _s->println("Too many commands defined");
    return;
  }
  user_cmds[fn_counter].cmd = cmd;
  user_cmds[fn_counter].type = type;
  user_cmds[fn_counter].meta = meta;
  user_cmds[fn_counter].fn = fn;
  fn_counter++;
}

// This is a private method which matches the command to our list of commands
char LogoVM::extractCmd(char *buffer){
  for(byte cmd = 0; cmd<COMMAND_COUNT; cmd++){
    for(byte j = 0; j < input_buffer_pos; j++){
      //compare each letter until we hit a space or the end of the string to find a match
      if(buffer[j] == user_cmds[cmd].cmd[j]){
        if((buffer[j + 1] == ' ' || buffer[j + 1] == '\0') && user_cmds[cmd].cmd[j + 1] == '\0'){
          //This is a command match, so return the index of the user command
          return cmd;
        }
      }else{
        break;
      }
    }
  }
  return -1;
}

// This extracts an int from the arguments
int LogoVM::extractIntArg(char *buffer){
  //extract everthing after the space to the end of the line and convert to an integer
  char arg[INPUT_BUFFER_LENGTH];
  char arg_pos = -1;
  for (byte i = 0; i < INPUT_BUFFER_LENGTH; i++){
    if(arg_pos == -1){
      if (buffer[i] == ' '){
        arg_pos++;
      }
    }else{
      if(buffer[i] != '\0' && buffer[i] != ' '){
        arg[arg_pos] = buffer[i];
        arg_pos++;
      }else{
        break;
      }
    }
  }
  arg[arg_pos] = '\0';
  return atoi(arg);
}

void LogoVM::storeCmd(char *buffer){
  char cmd = extractCmd(input_buffer);
  if(cmd < 0){
    _s->println("Syntax error: unknown command");
    return;
  }
  if(user_cmds[cmd].meta){
    ((void (*)(int))user_cmds[cmd].fn)(0);
  }else{
    if (user_cmds[cmd].type == REPEAT_TYPE) {
      // Increment the nesting level counter
      nest_level++;
    } else if (user_cmds[cmd].type == END_REPEAT_TYPE) {
      // Decrement the nesting level counter
      if (nest_level == 0){
        _s->println("Syntax error: closing unopened subroutine");
        return;
      }
      nest_level--;
    }
    cmd_stack[cmd_write_pos].cmd = cmd;
    if(user_cmds[cmd].type == INTEGER_TYPE || user_cmds[cmd].type == REPEAT_TYPE){
      cmd_stack[cmd_write_pos].arg = extractIntArg(buffer);
    }
    cmd_write_pos++;
  }
}


void LogoVM::processNextCmd(){
  boolean moveon = true;
  if (!running) { return; }
  if (cmd_read_pos >= cmd_write_pos) { return; }
  if (cmd_write_pos == 0) { return; }

  if (user_cmds[cmd_stack[cmd_read_pos].cmd].type == REPEAT_TYPE) {
    // if it's a repeat, enter another level in the stack
    subroutine_pos++;
    if(subroutine_pos >= SUBROUTINE_STACK_DEPTH){
      _s->println("Program error: stack too deep");
    }else{
      subroutine_stack[subroutine_pos].pos = cmd_read_pos;
      subroutine_stack[subroutine_pos].loops_remaining = cmd_stack[cmd_read_pos].arg - 1;
    }
  } else if (user_cmds[cmd_stack[cmd_read_pos].cmd].type == END_REPEAT_TYPE) {
    // it's the end of a repeat
    if(subroutine_stack[subroutine_pos].loops_remaining > 0){
      // there are loops remaining so do another one
      subroutine_stack[subroutine_pos].loops_remaining--;
      cmd_read_pos = subroutine_stack[subroutine_pos].pos;
    }else{
      // we've done all of the loops so continue
      subroutine_pos--;
    }
  } else {
    // run the command
    moveon = user_cmds[cmd_stack[cmd_read_pos].cmd].fn(new FnContext(_first_call, cmd_stack[cmd_read_pos].arg));
    _first_call = false;
  }
  if(moveon) {
    cmd_read_pos++;
    _first_call = true;
  };
}

