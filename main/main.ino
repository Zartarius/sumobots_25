#include <Arduino.h>

class USSensor {
  private:
    int trig_pin_;
    int echo_pin_;
  public:
    USSensor(void) = default;
    USSensor(int trig_pin, int echo_pin) : trig_pin_{trig_pin}, echo_pin_{echo_pin} {
      pinMode(trig_pin_, OUTPUT);
      pinMode(echo_pin_, INPUT);
    }
    //USSensor(const USSensor&& s) : trig_pin_{s.trig_pin_}, echo_pin_{s.echo_pin_}  {}
    USSensor& USSensor::operator=(USSensor&& s) {
        trig_pin_ = s.trig_pin_;
        echo_pin_ = s.echo_pin_;
        return *this;
    }

    float get_distance() {
      digitalWrite(trig_pin_, LOW);
      // wait to clear out
      delayMicroseconds(2);
      digitalWrite(trig_pin_, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig_pin_, LOW);
      
      float time = pulseIn(echo_pin_, HIGH);
      // distance formula
      return (time * 0.0343) / 2;
      // same as return time * 0.01715
    }
};

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
