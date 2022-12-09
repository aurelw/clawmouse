/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/


#define RELAY_1_PIN 3
#define RELAY_2_PIN 4
#define RELAY_3_PIN 5
#define RELAY_4_PIN 6
#define RELAY_5_PIN 7
#define RELAY_6_PIN 8
#define RELAY_7_PIN 9
#define RELAY_8_PIN 10


enum struct MotorState {
    PAUSE = 0,
    FORWARD = 1,
    BACKWARD = 2
};


void setup() {
    pinMode(RELAY_1_PIN, OUTPUT);
    digitalWrite(RELAY_1_PIN, HIGH);
    pinMode(RELAY_2_PIN, OUTPUT);
    digitalWrite(RELAY_2_PIN, HIGH);
    pinMode(RELAY_3_PIN, OUTPUT);
    digitalWrite(RELAY_3_PIN, HIGH);
    pinMode(RELAY_4_PIN, OUTPUT);
    digitalWrite(RELAY_4_PIN, HIGH);
    pinMode(RELAY_5_PIN, OUTPUT);
    digitalWrite(RELAY_5_PIN, HIGH);
    pinMode(RELAY_6_PIN, OUTPUT);
    digitalWrite(RELAY_6_PIN, HIGH);
    pinMode(RELAY_7_PIN, OUTPUT);
    digitalWrite(RELAY_7_PIN, HIGH);
    pinMode(RELAY_8_PIN, OUTPUT);
    digitalWrite(RELAY_8_PIN, HIGH);
    Serial.begin(9600);           // set up Serial library at 9600 bps
}


void switch_relay(int pin_id) {
    delay(500);
    digitalWrite(pin_id, LOW);
    delay(500);
    digitalWrite(pin_id, HIGH);
}

void switchXAxis(const MotorState to_state) {
    if (to_state != MotorState::PAUSE) {
        /* make sure we pause */
        digitalWrite(RELAY_1_PIN, HIGH);
        digitalWrite(RELAY_2_PIN, HIGH);
        digitalWrite(RELAY_3_PIN, HIGH);
        delay(500);
    }
    if (to_state == MotorState::FORWARD) {
        digitalWrite(RELAY_2_PIN, HIGH);
        digitalWrite(RELAY_3_PIN, LOW);
        digitalWrite(RELAY_1_PIN, LOW);
        return;
    }
    if (to_state == MotorState::BACKWARD) {
        digitalWrite(RELAY_1_PIN, HIGH);
        digitalWrite(RELAY_3_PIN, HIGH);
        digitalWrite(RELAY_2_PIN, LOW);
        return;
    }
    if (to_state == MotorState::PAUSE) {
        digitalWrite(RELAY_1_PIN, HIGH);
        digitalWrite(RELAY_2_PIN, HIGH);
        digitalWrite(RELAY_3_PIN, HIGH);
        return;
    }
}


void switchYAxis(const MotorState to_state) {
    if (to_state != MotorState::PAUSE) {
        /* make sure we pause */
        digitalWrite(RELAY_4_PIN, HIGH);
        digitalWrite(RELAY_5_PIN, HIGH);
        digitalWrite(RELAY_6_PIN, HIGH);
        delay(500);
    }
    if (to_state == MotorState::FORWARD) {
        digitalWrite(RELAY_5_PIN, HIGH);
        digitalWrite(RELAY_4_PIN, LOW);
        digitalWrite(RELAY_6_PIN, LOW);
        return;
    }
    if (to_state == MotorState::BACKWARD) {
        digitalWrite(RELAY_4_PIN, HIGH);
        digitalWrite(RELAY_6_PIN, HIGH);
        digitalWrite(RELAY_5_PIN, LOW);
        return;
    }
    if (to_state == MotorState::PAUSE) {
        digitalWrite(RELAY_4_PIN, HIGH);
        digitalWrite(RELAY_5_PIN, HIGH);
        digitalWrite(RELAY_6_PIN, HIGH);
        return;
    }
}


void switchZAxis(const MotorState to_state) {
    if (to_state != MotorState::PAUSE) {
        /* make sure we pause */
        digitalWrite(RELAY_7_PIN, HIGH);
        digitalWrite(RELAY_8_PIN, HIGH);
        delay(500);
    }
    if (to_state == MotorState::FORWARD) {
        digitalWrite(RELAY_7_PIN, HIGH);
        digitalWrite(RELAY_8_PIN, LOW);
        return;
    }
    if (to_state == MotorState::BACKWARD) {
        digitalWrite(RELAY_8_PIN, HIGH);
        digitalWrite(RELAY_7_PIN, LOW);
        return;
    }
    if (to_state == MotorState::PAUSE) {
        digitalWrite(RELAY_7_PIN, HIGH);
        digitalWrite(RELAY_8_PIN, HIGH);
        return;
    }
}


void play_token(String token) {
    MotorState motor_state;
    if (token.charAt(1) == 'F') {
        motor_state = MotorState::FORWARD;
    } else if (token.charAt(1) == 'B') {
        motor_state = MotorState::BACKWARD;
    } else if (token.charAt(1) == 'P') {
        motor_state = MotorState::PAUSE;
    }
    String hold_time_str = token.substring(2, 7);
    int hold_time = hold_time_str.toInt();
    if (token.charAt(0) == 'X') {
        switchXAxis(motor_state);
    } else if (token.charAt(0) == 'Y') {
        switchYAxis(motor_state);
    } else if (token.charAt(0) == 'Z') {
        switchZAxis(motor_state);
    }
    delay(hold_time);
}


void play_sequence(String seq_string) {
    int last_substr_pos = 0;
    for (int i=0; i<seq_string.length(); i++) {
        if (seq_string.charAt(i) == ';') {
            String token = seq_string.substring(last_substr_pos, i);
            last_substr_pos = i + 1;
            play_token(token);
        }
    }
}


void test_cycle() {
    switchXAxis(MotorState::PAUSE);
    delay(1000);
    switchXAxis(MotorState::FORWARD);
    delay(1000);
    switchXAxis(MotorState::PAUSE);
    delay(1000);
    switchXAxis(MotorState::BACKWARD);
    delay(1000);
    switchXAxis(MotorState::PAUSE);
    delay(1000);
    switchYAxis(MotorState::FORWARD);
    delay(1000);
    switchYAxis(MotorState::PAUSE);
    delay(1000);
    switchYAxis(MotorState::BACKWARD);
    delay(1000);
    switchYAxis(MotorState::PAUSE);
    delay(1000);
    switchZAxis(MotorState::FORWARD);
    delay(1000);
    switchZAxis(MotorState::PAUSE);
    delay(1000);
    switchZAxis(MotorState::BACKWARD);
    delay(1000);
    switchZAxis(MotorState::PAUSE);
    delay(1000);
}


void loop() {
    /* read line from Serial */
    String serial_line;
    int max_line_length = 256;
    for (int i=0; i<max_line_length; i++) {
        char c = Serial.read();
        if (c == '\n') {
          break;
        }
        serial_line += c;
    }

    /* check for command */
    if (serial_line.substring(0, 5) == "PLAY;") {
        if (serial_line.substring(serial_line.length() - 5, serial_line.length()) == "YALP;") {
            String play_command = serial_line.substring(5, serial_line.length()-5);
            Serial.println("[OK]");
            play_sequence(play_command);
        } else {
            Serial.println("[ERROR]");
        }
    }
    //play_sequence("XF3000;XP0100;YF2000;YP0100;ZF2000;ZP1000;ZB2000;ZP0100;XB3000;XP0100;");
    //Serial.println("loop done");
    delay(100);
}
