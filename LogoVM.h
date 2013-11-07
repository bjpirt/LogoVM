#ifndef LogoVM_h
#define LogoVM_h

#include "Arduino.h"

// COMMAND_COUNT specifies how many user commands we are going to be able to hold - you can reduce this to save memory
#define COMMAND_COUNT  12
#define INPUT_BUFFER_LENGTH 20
#define CMD_STACK_SIZE 80
#define SUBROUTINE_STACK_DEPTH 10
// Define the different types of results
#define INTEGER_TYPE    1
#define FLOAT_TYPE      2
#define STRING_TYPE     3
#define NO_TYPE         4
#define REPEAT_TYPE     5
#define END_REPEAT_TYPE 6

struct FnContext {
  boolean first_call;
  int arg;
  FnContext(boolean f=false, int a=NULL):first_call(f),arg(a){}
};

typedef boolean (* fp) (FnContext *context);

struct UserCmd {
  const char *cmd;
  byte type;
  boolean meta;
  fp fn;
};

struct Command {
  char cmd;
  int  arg;
};

struct RepeatStack {
  byte pos;
  byte loops_remaining;
};

class LogoVM {
  public:
    LogoVM(Stream &s);
    void addUserCmd(char* cmd, byte type, boolean meta, fp fn);
    void processInput();
    void processNextCmd();
    void pause();
    void start();
  private:
    Stream* _s;
    char extractCmd(char *buffer);
    int  extractIntArg(char *buffer);
    void storeCmd(char *buffer);
    byte fn_counter;
    char input_buffer[INPUT_BUFFER_LENGTH];
    byte input_buffer_pos;
    byte cmd_write_pos;
    byte cmd_read_pos;
    boolean running;
    byte subroutine_pos;
    byte nest_level;
    boolean _first_call;
    UserCmd user_cmds[COMMAND_COUNT];
    Command cmd_stack[CMD_STACK_SIZE];
    RepeatStack subroutine_stack[SUBROUTINE_STACK_DEPTH];
};

#endif
