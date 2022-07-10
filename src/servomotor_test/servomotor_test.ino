#include <Servo.h>

Servo servo;

int i = 0;

void setup() {
    
    Serial.begin(9600);

    servo.attach(5); //D1
    servo.write(0);

    delay(2000);

}

void loop() {

    int turn=(90*i++)%360;
    servo.write(turn);
    Serial.print("Turn");
    Serial.println(turn);
    delay(500);
    
}
