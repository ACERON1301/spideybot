#include <Bluepad32.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <ESP32Servo.h>

// ==================================================
// PCA9685
// ==================================================

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define PCA_FREQ 50

// Servo pulse limits for PCA9685
#define SERVO_MIN 150
#define SERVO_MAX 600

// ==================================================
// ESP32 NATIVE SERVOS
// ==================================================

#define SERVO_17_PIN 25
#define SERVO_18_PIN 26

Servo servo17;
Servo servo18;

// ==================================================
// BLUEPAD32
// ==================================================

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

bool sequenceRunning = false;
bool startPreviouslyPressed = false;

// ==================================================
// CONTROLLER CONNECT
// ==================================================

void onConnectedController(ControllerPtr ctl) {

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {

    if (myControllers[i] == nullptr) {

      myControllers[i] = ctl;

      Serial.printf(
        "Controller connected at slot %d\n",
        i);

      break;
    }
  }
}

// ==================================================
// CONTROLLER DISCONNECT
// ==================================================

void onDisconnectedController(ControllerPtr ctl) {

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {

    if (myControllers[i] == ctl) {

      myControllers[i] = nullptr;

      Serial.println("Controller disconnected");

      break;
    }
  }
}

// ==================================================
// MOVE PCA9685 SERVO
// ==================================================

void movePCAServo(int channel, int angle) {

  int pulse = map(
    angle,
    0,
    180,
    SERVO_MIN,
    SERVO_MAX);

  pwm.setPWM(
    channel,
    0,
    pulse);
}

// ==================================================
// SERVO SEQUENCE
// ==================================================

void runServoSequence() {

  if (sequenceRunning)
    return;

  sequenceRunning = true;

  Serial.println("================================");
  Serial.println("START BUTTON PRESSED");
  Serial.println("Starting Servo Sequence...");
  Serial.println("================================");

  // ----------------------------------------------
  // PCA9685 SERVOS 1 TO 16
  // ----------------------------------------------

  for (int i = 0; i < 16; i++) {

    Serial.print("Moving Servo ");
    Serial.println(i + 1);

    // Move servo to 90 degrees
    movePCAServo(i, 90);

    delay(500);

    // Move servo back to 0
    movePCAServo(i, 0);

    delay(300);
  }

  // ----------------------------------------------
  // ESP32 SERVO 17
  // ----------------------------------------------

  Serial.println("Moving Servo 17");

  servo17.write(90);

  delay(500);

  servo17.write(0);

  delay(300);

  // ----------------------------------------------
  // ESP32 SERVO 18
  // ----------------------------------------------

  Serial.println("Moving Servo 18");

  servo18.write(90);

  delay(500);

  servo18.write(0);

  delay(300);

  Serial.println("================================");
  Serial.println("Servo Sequence Complete!");
  Serial.println("================================");

  sequenceRunning = false;
}

// ==================================================
// READ CONTROLLER
// ==================================================

void processGamepad(ControllerPtr ctl) {

  uint8_t misc = ctl->miscButtons();

  bool startPressed =
    (misc & MISC_BUTTON_START);

  // Detect single press
  if (startPressed && !startPreviouslyPressed) {

    runServoSequence();
  }

  startPreviouslyPressed = startPressed;
}

// ==================================================
// SETUP
// ==================================================

void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println("ESP32 Servo Controller Starting");

  // ----------------------------------------------
  // I2C
  // ----------------------------------------------

  Wire.begin(21, 22);

  // ----------------------------------------------
  // PCA9685
  // ----------------------------------------------

  pwm.begin();

  pwm.setPWMFreq(PCA_FREQ);

  delay(10);

  // ----------------------------------------------
  // ESP32 SERVOS
  // ----------------------------------------------

  servo17.setPeriodHertz(50);
  servo18.setPeriodHertz(50);

  servo17.attach(
    SERVO_17_PIN,
    500,
    2400);

  servo18.attach(
    SERVO_18_PIN,
    500,
    2400);

  // Initial positions
  servo17.write(0);
  servo18.write(0);

  // PCA servos initial position
  for (int i = 0; i < 16; i++) {

    movePCAServo(i, 0);
  }

  // ----------------------------------------------
  // BLUEPAD32
  // ----------------------------------------------

  BP32.setup(
    &onConnectedController,
    &onDisconnectedController);

  // Uncomment ONLY when pairing
  // BP32.forgetBluetoothKeys();

  Serial.println("================================");
  Serial.println("SYSTEM READY");
  Serial.println("Press START on controller");
  Serial.println("================================");
}

// ==================================================
// LOOP
// ==================================================

void loop() {

  bool dataUpdated = BP32.update();

  if (dataUpdated) {

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {

      ControllerPtr ctl =
        myControllers[i];

      if (
        ctl != nullptr && ctl->isConnected() && ctl->hasData()) {

        processGamepad(ctl);
      }
    }
  }

  delay(10);
}