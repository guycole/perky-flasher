//
// Title:switcher.cpp
//
// Description:
//   Kill/restart flasher, invoked by mechanical switch
//
//   g++ -Wall switcher.cpp -o switcher
//
// Development Environment:
//   Angstrom v2012.12
//   gcc version 4.7.3 20130205 (prerelease) (Linaro GCC 4.7-2013.02-01) 
//
// Author:
//   G.S. Cole (guycole at gmail dot com)
//
// Maintenance History:
//   $Log$
//
//static char rcsid[] = "$Id$";

#include <unistd.h>
#include <stdio.h>
#include <string.h>

//using namespace std;

enum run_mode { sequence, fault };

class Configuration {
public:
  void setRunMode(run_mode next_mode);
  run_mode getRunMode(){return current_mode;}
  int getSwitchGpio(){return switch_gpio;}
private:
  int switch_gpio;
  run_mode current_mode;
};

void Configuration::setRunMode(run_mode next_mode) {
  current_mode = next_mode;
}
  
int main(int argc, char *argv[]) {
  printf("fix me\n");
}
