#include <Servo.h>

Servo servo;

void setup() {
    
    Serial.begin(115200);
    delay(10);

    Serial.println();
    Serial.print("START");


    servo.attach(5); //D1
    servo.write(0);

    delay(2000);

}

void loop() {

    Serial.println();
    Serial.print("A loop");

    servo.write(90);

    delay(1000);

    servo.write(0);

    delay(1000);

}
