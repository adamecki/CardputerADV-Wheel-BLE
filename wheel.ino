#include <M5Cardputer.h>
#include "USBHIDGamepad.h"

USBHIDGamepad Gamepad;

void setup() {
    M5.begin();
    M5Cardputer.begin();
    Gamepad.begin();
    M5Cardputer.Display.setTextSize(3.0, 3.0); 
    
}
int x=50;

void loop() {
    M5.update();
    M5Cardputer.update();
    
    if (M5Cardputer.Keyboard.isChange()){
        if (M5Cardputer.Keyboard.isKeyPressed(';')){
            x+=5;
            M5Cardputer.Display.clear();
            M5Cardputer.Display.setCursor(20,20);
            M5Cardputer.Display.print(x);
        };
        if (M5Cardputer.Keyboard.isKeyPressed('.')){
            x-=5;
            M5Cardputer.Display.clear();
            M5Cardputer.Display.setCursor(20,20);
            M5Cardputer.Display.print(x);
        }
    }
    
    float X,Y,Z;
    M5.Imu.getAccelData(&X,&Y,&Z);

    int wheelPos = map(X * 100,x*-1,x,127, -128);

    if (wheelPos > 127) wheelPos = 127;
    if (wheelPos < -128) wheelPos = -128;

    Gamepad.leftStick((int8_t)wheelPos, 0);

    delay(5); 
}