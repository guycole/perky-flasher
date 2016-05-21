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
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

//using namespace std;

enum run_mode { sequence, fault };

class Configuration {
public:
  void setRunMode(run_mode next_mode);
  run_mode getRunMode(){return current_mode;}
  int getSwitchGpio(){return switch_gpio;}
  int getPollDuration(){return poll_duration;}
private:
  int poll_duration = 500000;
  int switch_gpio = 7;
  run_mode current_mode = fault;
};

void Configuration::setRunMode(run_mode next_mode) {
  current_mode = next_mode;
  printf("run mode\n");
}

class PushSwitch {
public:
  PushSwitch(int gpio);
  bool pollSwitch();
  void switchActive(bool flag);
private:
  char gpio_string[5];
};

PushSwitch::PushSwitch(int gpio) {
  sprintf(gpio_string, "%d", gpio);

  FILE *export_file = fopen("/sys/class/gpio/export", "w");
  fwrite(gpio_string, strlen(gpio_string), 1, export_file);
  fclose(export_file);
}

bool PushSwitch::pollSwitch() {
  char buffer[64];
  char file_name[64];

  sprintf(file_name, "/sys/class/gpio/gpio%s/value", gpio_string);
  strcpy(buffer, "\n");

  FILE *value_file = fopen(file_name, "r");
  fread(buffer, sizeof(buffer), 1, value_file);
  fclose(value_file);

  if ((strlen(buffer) > 0) && (atoi(buffer) > 0)) {
      return true;
  }

  return false;
}

int main(int argc, char *argv[]) {
  Configuration configuration;

  PushSwitch push_switch = PushSwitch(configuration.getSwitchGpio());

  bool last_result = false;

  pid_t pid = 0;

  char *command_sequence[3] = {"/home/root/flasher", "sequence", NULL};
  char *command_fault[3] = {"/home/root/flasher", "fault", NULL};
  
  //  for (int ii = 0; ii < 100; ii++) {
  while (1) {
    usleep(configuration.getPollDuration());
    bool result = push_switch.pollSwitch();

    if (result != last_result) {
      last_result = result;

      if (result) {
	//switch mode on first true

	if (pid > 0) {
	  kill(pid, SIGKILL);
	}

	switch (configuration.getRunMode()) {
	case sequence:
	  configuration.setRunMode(fault);

	  if ((pid = fork()) == 0) {
	    execv(command_fault[0], command_fault);
	    exit(0);
	  }

	  break;
	case fault:
	  configuration.setRunMode(sequence);

	  if ((pid = fork()) == 0) {
	    execv(command_sequence[0], command_sequence);
	    exit(0);
	  }

	  break;
	}
      }
    }
  }
}
