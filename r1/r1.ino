/* r1.ino, r1 Mark I - Rover, Arduino main sketch file
 *
 * Copyright 2019-2020 Marcio Pessoa
 *
 * This sketch was developed and tested on: Arduino Leonardo
 * To work on other Arduino models, some adaptations may be necessary.
 *
 * Author: Márcio Pessoa <marcio.pessoa@gmail.com>
 * Contributors: none
 */

#include <Arduino.h>      // Main library
#include <Project.h>      // Basic project definitions
#include <Blinker.h>      // Blink leds nicely
#include <Timer.h>        // Timer library with nice features
#include <Alarm.h>        // Manage alarms
#include <Temperature.h>  // Temperature Sensors
#include <L298.h>         // Temperature Sensors
#include "./config.h"     // Configuration
#include <MemoryFree.h>   //

// Project definitions
Project r1("r1",  // Platform
           "I",  // Mark
           "Rover",  // Name
           "0.1",  // Version
           "2020-01-22",  // Version date
           "9",  // Serial number
           "Copyright 2019-2020 Marcio Pessoa",  // Owner
           "GPLv2. There is NO WARRANTY.",  // License
           "https://github.com/marcio-pessoa/r1",  // Website
           "Marcio Pessoa <marcio.pessoa@gmail.com>");  // Contact

// Status LED
Blinker status_led(led_status_pin);

// Teperature sensor
Temperature lm35;
Alarm temperature(60,   // Maximum warning
                  70,   // Maximum critical
                  10,   // Minimum warning
                   5);  // Minimum critical  // TODO: Change values

// Check timer
Timer health_check(health_check_timer * 1000);

// Sensors timer
Timer sensors_status(sensors_timer * 1000);

// Drivers
L298 driver_a;
L298 driver_b;

// Power save options
Timer standby((unsigned long)standby_timer * 60 * 1000, COUNTDOWN);
bool standby_status = false;
bool standby_done = false;

void setup() {
  // Serial interface
  Serial.begin(serial_speed);
  // Start up message
  Serial.println("Starting...");
  CommandM92();  // System information
  // Temperature
  lm35.attach(lm35_pin);
  temperature.nameWrite("Temperature");
  temperature.unitWrite(" *C");
  // Drivers
  driver_a.attach(1, 2, 3, 4);  // TODO: Change pins
  driver_b.attach(1, 2, 3, 4);  // TODO: Change pins
  // Random number generator seed
  pinMode(random_Seed_pin, INPUT);
  randomSeed(analogRead(random_Seed_pin));
  // G-code ready to receive commands
  GcodeReady();
}

void loop() {
  SensorsHandler();
  HealthCheckHandler();
  NotificationHandler();
  AxesHandler();
  PowerHandler();
  GcodeCheck();
}
