#include <Arduino.h>

Motor motor;
USSensor us_sensor;

static const float MAX_DIST = 100.0;
static const float FORWARD_SPEED = 69.0; // to change
static const float REVRSE_SPEED = 69.0; // to change
static const unsigned long PIVOT_TIME = 100; // to change

void setup(void) {
  Serial.begin(9600);
  us_sensor = USSensor(6,5);
}



void loop(void) {
  // Serial.println(sensor.get_distance());
   {
      float distance;
      while ((distance = sensor.get_distance()) <= MAX_DIST /*&& IRsensor doesn't detect tape*/) {
         motor.drive_forward(FORWARD_SPEED - distance); // Increase speed as it gets closer?
      }
   }
   
   motor.brake();
   
   // if (IR sensor detects tape) {
   //    motor.drive_reverse(REVERSE_SPEED);
   // }

   for (float time = PIVOT_TIME; time < 1000000 && (sensor.get_distance() > MAX_DIST); time += PIVOT_TIME) {
      // motor.pivot_right for time
      // motor.brake()
      // motor.pivot_left for 2 * time
      // motor.brake()
      // motor.pivot_right for time
      // motor.brake()
   }
}
