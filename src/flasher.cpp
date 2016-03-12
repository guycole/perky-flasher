//
// Title:flasher.cpp
//
// Description:
//   Traffic light simulator.
//   Sequence mode: cycle through red, green, yellow
//   Fault mode: blink red
//
//   g++ -Wall flasher.cpp -o flasher
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
  void setMode(char *mode);
  int getGreenDuration(){return green_duration;}
  int getGreenGpio(){return green_gpio;}
  int getNoneDuration(){return none_duration;}
  int getRedDuration(){return red_duration;}
  int getRedGpio(){return red_gpio;}
  int getYellowDuration(){return yellow_duration;}
  int getYellowGpio(){return yellow_gpio;}
  run_mode getRunMode(){return current_mode;}
private:
  int green_duration;
  int green_gpio;
  int none_duration;
  int red_duration;
  int red_gpio;
  int yellow_duration;
  int yellow_gpio;
  run_mode current_mode; 
};

void Configuration::setMode(char *mode) {
  green_gpio = 27;
  red_gpio = 23;
  yellow_gpio = 26;

  if (!strcmp("fault", mode)) {
    current_mode = fault;
    green_duration = 0;
    none_duration = 500000;
    red_duration = 500000;
    yellow_duration = 0;
  } else {
    current_mode = sequence;
    green_duration = 500000;
    none_duration = 0;
    red_duration = 1000000;
    yellow_duration = 200000;
  }
}

class LedLight {
public:
  LedLight(int gpio);
  void lightFlag(bool flag);
private:
  char gpio_string[5];
};

LedLight::LedLight(int gpio) {
  sprintf(gpio_string, "%d", gpio);

  FILE *export_file = fopen("/sys/class/gpio/export", "w");
  fwrite(gpio_string, strlen(gpio_string), 1, export_file);
  fclose(export_file);
}

void LedLight::lightFlag(bool flag) {
  char file_name[64];
  sprintf(file_name, "/sys/class/gpio/gpio%s/direction", gpio_string);

  FILE *direction_file = fopen(file_name, "w");

  if (flag) {
    char command[] = "high";
    fwrite(command, strlen(command), 1, direction_file);
  } else {
    char command[] = "low";
    fwrite(command, strlen(command), 1, direction_file);
  }
  
  fclose(direction_file);
}

enum lamp_color { none, red, green, yellow };

void set_lamp(lamp_color arg, LedLight green_light, LedLight red_light, LedLight yellow_light) {
  green_light.lightFlag(false);
  red_light.lightFlag(false);
  yellow_light.lightFlag(false);

  switch(arg) {
  case none:
    break;
  case red:
    red_light.lightFlag(true);
    break;
  case green:
    green_light.lightFlag(true);
    break;
  case yellow:
    yellow_light.lightFlag(true);
    break;
  }
}

void sequence_mode(Configuration configuration) {
  LedLight green_light(configuration.getGreenGpio());
  LedLight red_light(configuration.getRedGpio());
  LedLight yellow_light(configuration.getYellowGpio());

  lamp_color current = red;

  while(1) {
    set_lamp(current, green_light, red_light, yellow_light);

    switch(current) {
    case none:
      //pacifier
      break;
    case red:
      usleep(configuration.getRedDuration());
      current = green;
      break;
    case green:
      usleep(configuration.getGreenDuration());
      current = yellow;
      break;
    case yellow:
      usleep(configuration.getYellowDuration());
      current = red;
      break;
    }
  }
}

void fault_mode(Configuration configuration) {
  LedLight green_light(configuration.getGreenGpio());
  LedLight red_light(configuration.getRedGpio());
  LedLight yellow_light(configuration.getYellowGpio());

  while(1) {
    set_lamp(red, green_light, red_light, yellow_light);
    usleep(configuration.getRedDuration());
    set_lamp(none, green_light, red_light, yellow_light);
    usleep(configuration.getNoneDuration());
  }
}

int main(int argc, char *argv[]) {
  Configuration configuration;

  if (argc < 2) {
    configuration.setMode(strdup("fault"));
  } else {
    configuration.setMode(argv[1]);
  }

  switch (configuration.getRunMode()) {
  case fault:
    fault_mode(configuration);
    break;
  case sequence:
    sequence_mode(configuration);
    break;
  }
}
