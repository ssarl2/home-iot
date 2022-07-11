#include <Servo.h>

Servo servo;
int turn=0;

int i = 0;

void setup() {
    
    Serial.begin(9600);

    servo.attach(5); //D1
    servo.write(0);

    delay(2000);

}

void loop() {

    // if(turn == 630)
    // {
    //     turn = 720;
    // }
    // else
    // {
    //     turn=(90*i++)%720;
    // }

    servo.write(turn);
    Serial.print("Turn");
    Serial.println(turn);
    delay(300);

    if(turn == 0)
        turn = 180;
    else
        turn = 0;
/*
    if(turn < 360 && turn >= 0)
        turn += 30;
    else if(turn >= 360)
    {
        turn = 0;
        delay(250);
    }
*/

/*
    if(turn < 0 && turn >= -360)
        turn += 30;
    else if(turn >= 0)
        turn = -360;
*/

}
