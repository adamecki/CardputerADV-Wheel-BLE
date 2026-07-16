#include <Arduino.h>
#include <M5Cardputer.h>
#include <M5GFX.h>
#include <BleGamepad.h>

// hotspot bro
BleGamepad bleGamepad;

M5Canvas canvas(&M5Cardputer.Display);
bool display_toggle = true;

int x = 120;
int wheelPos = 16384;

float X = 0.0, Y = 0.0, Z = 0.0;
const float alpha = 0.12; // smoothing parameter - the lower, the smoother (0.01 to 1.0)

void refresh_screen() {
    canvas.fillRect(0, 0, M5Cardputer.Display.width(), M5Cardputer.Display.height(), TFT_WHITE);

    canvas.drawString(bleGamepad.getDeviceName(), M5Cardputer.Display.width() / 2, 8);

    if(bleGamepad.isConnected()) {
        canvas.drawString("connected", M5Cardputer.Display.width() / 2, 36);
    } else {
        canvas.setTextColor(TFT_YELLOW);
        canvas.drawString("waiting", M5Cardputer.Display.width() / 2, 36);
        canvas.setTextColor(TFT_NAVY);
    }

    if(M5.Power.getBatteryLevel() <= 20) { canvas.setTextColor(TFT_RED); }
    canvas.drawString(String(M5.Power.getBatteryLevel()) + "%", M5Cardputer.Display.width() / 2, 68);
    canvas.setTextColor(TFT_NAVY);

    int wheelPosIndicator = wheelPos;
    if(wheelPosIndicator < 0) { wheelPosIndicator = 0; }
    wheelPosIndicator = map(wheelPosIndicator, 0, 32768, (M5Cardputer.Display.width() / 2) * -1, (M5Cardputer.Display.width() / 2) + 1);
    canvas.fillRect(M5Cardputer.Display.width() / 2, M5Cardputer.Display.height() - 16, wheelPosIndicator, 16, TFT_NAVY);

    canvas.pushSprite(0, 0);
}

void setup() {
    auto cfg = M5.config();
    M5Cardputer.begin(cfg, true);

    M5Cardputer.Display.setRotation(1);
    canvas.createSprite(M5Cardputer.Display.width(), M5Cardputer.Display.height());

    canvas.setTextFont(&fonts::FreeMonoBold12pt7b);
    canvas.setTextDatum(top_center);
    canvas.setTextSize(1);

    canvas.setTextColor(TFT_NAVY);
    refresh_screen();

    bleGamepad.begin();
    bleGamepad.setAxes(0, 0, 0, 0, 0, 0, 0, 0); // love this function
}

void loop() {
    M5Cardputer.update();
    
    if(M5Cardputer.Keyboard.isChange()){
        // sensitivity settings 
        if(M5Cardputer.Keyboard.isKeyPressed(';')){
            x += 5;
        }
        if(M5Cardputer.Keyboard.isKeyPressed('.')){
            x -= 5;
        }

        // display settings
        if(M5Cardputer.Keyboard.isKeyPressed(' ')) {
            display_toggle = !display_toggle;
            if(display_toggle) {
                M5Cardputer.Display.setBrightness(255);
            } else {
                M5Cardputer.Display.setBrightness(0);
            }
        }

        // gamepad buttons
        if(M5Cardputer.Keyboard.isKeyPressed('1')) {
            bleGamepad.press(BUTTON_2);
        } else {
            bleGamepad.release(BUTTON_2);
        }
        if(M5Cardputer.Keyboard.isKeyPressed('2')) {
            bleGamepad.press(BUTTON_3);
        } else {
            bleGamepad.release(BUTTON_3);
        }
        if(M5Cardputer.Keyboard.isKeyPressed('-')) {
            bleGamepad.press(BUTTON_4);
        } else {
            bleGamepad.release(BUTTON_4);
        }
        if(M5Cardputer.Keyboard.isKeyPressed('=')) {
            bleGamepad.press(BUTTON_1);
        } else {
            bleGamepad.release(BUTTON_1);
        }
        if(M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) {
            bleGamepad.press(BUTTON_5);
        } else {
            bleGamepad.release(BUTTON_5);
        }
        if(M5Cardputer.Keyboard.isKeyPressed('e')) {
            bleGamepad.press(BUTTON_6);
        } else {
            bleGamepad.release(BUTTON_6);
        }
        if(M5Cardputer.Keyboard.isKeyPressed('s')) {
            bleGamepad.press(BUTTON_7);
        } else {
            bleGamepad.release(BUTTON_7);
        }
        if(M5Cardputer.Keyboard.isKeyPressed('a')) {
            bleGamepad.press(BUTTON_8);
        } else {
            bleGamepad.release(BUTTON_8);
        }
        if(M5Cardputer.Keyboard.isKeyPressed('d')) {
            bleGamepad.press(BUTTON_9);
        } else {
            bleGamepad.release(BUTTON_9);
        }
        if(M5Cardputer.Keyboard.isKeyPressed('f')) {
            bleGamepad.press(BUTTON_10);
        } else {
            bleGamepad.release(BUTTON_10);
        }
        if(M5Cardputer.Keyboard.isKeyPressed('g')) {
            bleGamepad.press(BUTTON_11);
        } else {
            bleGamepad.release(BUTTON_11);
        }
        if(M5Cardputer.Keyboard.isKeyPressed('h')) {
            bleGamepad.press(BUTTON_12);
        } else {
            bleGamepad.release(BUTTON_12);
        }
        if(M5Cardputer.Keyboard.isKeyPressed('o')) {
            bleGamepad.press(BUTTON_13);
        } else {
            bleGamepad.release(BUTTON_13);
        }
        if(M5Cardputer.Keyboard.isKeyPressed('j')) {
            bleGamepad.press(BUTTON_14);
        } else {
            bleGamepad.release(BUTTON_14);
        }
        if(M5Cardputer.Keyboard.isKeyPressed('k')) {
            bleGamepad.press(BUTTON_15);
        } else {
            bleGamepad.release(BUTTON_15);
        }
        if(M5Cardputer.Keyboard.isKeyPressed('l')) {
            bleGamepad.press(BUTTON_16);
        } else {
            bleGamepad.release(BUTTON_16);
        }
    }

    if(M5Cardputer.BtnA.wasPressed()) {
        bleGamepad.press(BUTTON_1);
    } 
    if(M5Cardputer.BtnA.wasReleased()) {
        bleGamepad.release(BUTTON_1);
    }  

    // analog axis data, tuned for ETS2
    // (0 to 32767 instead of -32768 to 32767,
    // as ETS2 interprets 16384 as wheel centered for some reason)
    float rX, rY, rZ;
    M5.Imu.getAccelData(&rX, &rY, &rZ);

    // smoothing
    X = (alpha * rX) + ((1.0 - alpha) * X);
    //Y = (alpha * rY) + ((1.0 - alpha) * Y);

    wheelPos = map(X * 100, x * -1, x, 40959, -8192);
    wheelPos = constrain(wheelPos, -8192, 40959);
    if (wheelPos > 32767) {wheelPos = 32767;}
    bleGamepad.setX((int16_t)wheelPos);

    if(display_toggle) {
        refresh_screen();
    }
    delay(5);
}
