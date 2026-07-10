#include <Arduino.h>
#include <M5Cardputer.h>
#include <BleGamepad.h>

// hotspot bro
BleGamepad bleGamepad;

int x = 120;

float X = 0.0, Y = 0.0, Z = 0.0;
const float alpha = 0.12; // smoothing parameter - the lower, the smoother (0.01 to 1.0)

void setup() {
    M5.begin();
    M5Cardputer.begin();

    bleGamepad.begin();
    bleGamepad.setX(0);
    bleGamepad.setY(0);

    // Disable display backlight
    M5Cardputer.Display.setBrightness(0);
}

void loop() {
    M5.update();
    M5Cardputer.update();
    
    if (M5Cardputer.Keyboard.isChange()){
        //sensitivity settings 
        if (M5Cardputer.Keyboard.isKeyPressed(';')){
            x += 5;
            M5Cardputer.Display.clear();
            M5Cardputer.Display.setCursor(20, 20);
            M5Cardputer.Display.print(x);
        }
        if (M5Cardputer.Keyboard.isKeyPressed('.')){
            x -= 5;
            M5Cardputer.Display.clear();
            M5Cardputer.Display.setCursor(20, 20);
            M5Cardputer.Display.print(x);
        }
    }

    
    if (M5Cardputer.Keyboard.isKeyPressed('1')) {
        bleGamepad.press(BUTTON_4);
    } else {
        bleGamepad.release(BUTTON_4);
    }

    if (M5Cardputer.Keyboard.isKeyPressed('2')) {
        bleGamepad.press(BUTTON_2);
    } else {
        bleGamepad.release(BUTTON_2);
    }

    if (M5Cardputer.Keyboard.isKeyPressed('q')) {
        bleGamepad.press(BUTTON_3);
    } else {
        bleGamepad.release(BUTTON_3);
    }

    if (M5Cardputer.Keyboard.isKeyPressed('`')) {
        bleGamepad.press(BUTTON_1);
    } else {
        bleGamepad.release(BUTTON_1);
    }
    
    
    float rX, rY, rZ;
    M5.Imu.getAccelData(&rX, &rY, &rZ);

    // smoothing
    X = (alpha * rX) + ((1.0 - alpha) * X);
    //Y = (alpha * rY) + ((1.0 - alpha) * Y);

    int wheelPos = map(X * 100, x * -1, x, 40959, -8192);
    wheelPos = constrain(wheelPos, -8192, 40959);
    if (wheelPos > 32767) {wheelPos = 32767;}

    bleGamepad.setX((int16_t)wheelPos);

    delay(5); 
}
