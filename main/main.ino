#include <Arduino.h>

class Motor {
   private:
    // todo: put values here for pins
    // motor 1 - assumed left
    static constexpr int PIN_IN1 = 1;
    static constexpr int PIN_IN2 = 2;

    // motor 2 - assumed right
    static constexpr int PIN_IN3 = 3;
    static constexpr int PIN_IN4 = 4;

    static constexpr int PIN_ENA = 5;
    static constexpr int PIN_ENB = 6;

    void forward_left() {
        digitalWrite(PIN_IN1, HIGH);
        digitalWrite(PIN_IN2, LOW);
    }

    void set_speed_left(int speed) {
        analogWrite(PIN_ENA, speed);
    }

    void forward_right() {
        digitalWrite(PIN_IN3, HIGH);
        digitalWrite(PIN_IN4, LOW);
    }

    void set_speed_right(int speed) {
        analogWrite(PIN_ENB, speed);
    }

    void reverse_left() {
        digitalWrite(PIN_IN1, LOW);
        digitalWrite(PIN_IN2, HIGH);
    }

    void reverse_right() {
        digitalWrite(PIN_IN3, LOW);
        digitalWrite(PIN_IN4, HIGH);
    }

    void brake_left() {
        digitalWrite(PIN_IN1, HIGH);
        digitalWrite(PIN_IN2, HIGH);
    }

    void brake_right() {
        digitalWrite(PIN_IN3, HIGH);
        digitalWrite(PIN_IN4, HIGH);
    }

   public:
    //        ~Motor() = default;
    Motor(void) {
        pinMode(PIN_IN1, OUTPUT);
        pinMode(PIN_IN2, OUTPUT);
        pinMode(PIN_IN3, OUTPUT);
        pinMode(PIN_IN4, OUTPUT);
        pinMode(PIN_ENA, OUTPUT);
        pinMode(PIN_ENB, OUTPUT);
    }

    void drive_forward(const int speed) {
        set_speed_left(speed);
        set_speed_right(speed);
        forward_left();
        forward_right();
    }

    void drive_reverse(const int speed) {
        set_speed_left(speed);
        set_speed_right(speed);
        reverse_left();
        reverse_right();
    }

    void pivot_left(const int speed) {
        set_speed_left(speed);
        set_speed_right(speed);
        forward_left();
        reverse_right();
    }

    void pivot_right(const int speed) {
        set_speed_left(speed);
        set_speed_right(speed);
        forward_right();
        reverse_left();
    }
    
    void brake() {
        brake_left();
        brake_right();
    }
};

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
static const float PIVOT_TIME = 1.0; // to change

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

   for (float time = PIVOT_TIME; time < 1000000; time += PIVOT_TIME) {
      // motor.pivot_right for time
      // motor.pivot_left for 2 * time
      // motor.pivot_right for time
   }
}
