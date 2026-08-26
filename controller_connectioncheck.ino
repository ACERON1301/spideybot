#include <Bluepad32.h>

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

void onConnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      myControllers[i] = ctl;
      Serial.println("Controller Connected!");
      break;
    }
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      myControllers[i] = nullptr;
      Serial.println("Controller Disconnected!");
      break;
    }
  }
}

void processGamepad(ControllerPtr ctl) {

  // FACE BUTTONS
  if (ctl->a()) Serial.println("A");
  if (ctl->b()) Serial.println("B");
  if (ctl->x()) Serial.println("X");
  if (ctl->y()) Serial.println("Y");

  // SHOULDER BUTTONS
  if (ctl->l1()) Serial.println("L1");
  if (ctl->r1()) Serial.println("R1");

  // TRIGGERS
  if (ctl->brake() > 100) Serial.println("L2");
  if (ctl->throttle() > 100) Serial.println("R2");

  // D-PAD
  uint8_t dpad = ctl->dpad();

  if (dpad & DPAD_UP)    Serial.println("FORWARD");
  if (dpad & DPAD_DOWN)  Serial.println("BACKWARD");
  if (dpad & DPAD_LEFT)  Serial.println("LEFT");
  if (dpad & DPAD_RIGHT) Serial.println("RIGHT");

  // LEFT ANALOG STICK
  int lx = ctl->axisX();
  int ly = ctl->axisY();

  if (ly < -200) Serial.println("ANALOG FORWARD");
  if (ly > 200)  Serial.println("ANALOG BACKWARD");
  if (lx < -200) Serial.println("ANALOG LEFT");
  if (lx > 200)  Serial.println("ANALOG RIGHT");

  // RIGHT ANALOG STICK
  int rx = ctl->axisRX();
  int ry = ctl->axisRY();

  if (ry < -200) Serial.println("RIGHT STICK UP");
  if (ry > 200)  Serial.println("RIGHT STICK DOWN");
  if (rx < -200) Serial.println("RIGHT STICK LEFT");
  if (rx > 200)  Serial.println("RIGHT STICK RIGHT");

  // START / SELECT / SYSTEM
  uint8_t misc = ctl->miscButtons();

  if (misc & MISC_BUTTON_START)
    Serial.println("START");

  if (misc & MISC_BUTTON_SELECT)
    Serial.println("SELECT");

  if (misc & MISC_BUTTON_SYSTEM)
    Serial.println("SYSTEM / HOME");
}

void setup() {
  Serial.begin(115200);

  BP32.setup(
    &onConnectedController,
    &onDisconnectedController
  );

  // Uncomment only once when pairing a new controller
  // BP32.forgetBluetoothKeys();

  Serial.println("Bluepad32 Ready!");
  Serial.println("Connect your controller...");
}

void loop() {

  bool dataUpdated = BP32.update();

  if (dataUpdated) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {

      ControllerPtr ctl = myControllers[i];

      if (ctl && ctl->isConnected() && ctl->hasData()) {
        processGamepad(ctl);
      }
    }
  }

  delay(50);
}