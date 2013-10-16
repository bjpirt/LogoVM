arduino-LogoVM
==============

A simple configurable LOGO based virtual machine for Arduino.

It is very configurable and can be used to create your own set of commands. Currently it supports functions with an integer argument and a loop construct.

If you use the example sketch it comes with and type in the following over the serial console:

```LOGO

FORWARD 100
REPEAT 3 [
FORWARD 10
REPEAT 2 [
BACKWARD 10
]
]
PLAY

```

You should get the following printed out over the console as it performs the repeat loops:

```LOGO

FORWARD 100
FORWARD 10
BACKWARD 10
BACKWARD 10
FORWARD 10
BACKWARD 10
BACKWARD 10
FORWARD 10
BACKWARD 10
BACKWARD 10

```

The commands PLAY and PAUSE are commands which control the operation of the program. When the Arduino starts execution is paused. Once you type PLAY then the commands will be executed from the beginning and then as you type them in.