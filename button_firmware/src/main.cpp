#include "Arduino.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 25
#endif
#define LEFT_PIN 22
#define RIGHT_PIN 21
#define BACK_PIN 18
#define FRONT_PIN 16
#define UP_PIN 13
#define DOWN_PIN 15

class Button {

    public:

        Button(String name, int pin_number,
               unsigned long debounce_millis=200) :
                id(name), pin_n(pin_number),
                debounce_ms(debounce_millis)
            {}

        void check() {
            const int current_pin = digitalRead(pin_n); 
            if (current_pin != pin_status) {
                if ((millis()  - pin_last_ms) > debounce_ms) {
                    pin_last_ms = millis();
                    pin_status = current_pin;
                    if (current_pin == HIGH) {
                        Serial.println(id + "_HIGH");
                    } else {
                        Serial.println(id + "_LOW");
                    }
                }
            }
        }

    private:

        String id;
        int pin_n;
        unsigned long debounce_ms;
        unsigned long pin_last_ms = 0;
        int pin_status = LOW;

};


void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LEFT_PIN, INPUT);
  pinMode(RIGHT_PIN, INPUT);
  pinMode(BACK_PIN, INPUT);
  pinMode(FRONT_PIN, INPUT);
  pinMode(UP_PIN, INPUT);
  pinMode(DOWN_PIN, INPUT);
  Serial.begin(9600);           // set up Serial library at 9600 bps
}


void loop() {

    const unsigned long debounce_millis = 200;

    static Button left_button("LEFT", LEFT_PIN, debounce_millis);
    static Button right_button("RIGHT", RIGHT_PIN, debounce_millis);
    static Button front_button("FRONT", FRONT_PIN, debounce_millis);
    static Button back_button("BACK", BACK_PIN, debounce_millis);
    static Button up_button("UP", UP_PIN, debounce_millis);
    static Button down_button("DOWN", DOWN_PIN, debounce_millis);

    left_button.check();
    right_button.check();
    front_button.check();
    back_button.check();
    up_button.check();
    down_button.check();

    delay(10);
    //if (Serial.available() > 0) {
    //    String serial_input = Serial.readString();
    //    Serial.println(serial_input.c_str());
    //}
}
