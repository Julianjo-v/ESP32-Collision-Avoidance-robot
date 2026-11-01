#include <Wire.h>
#include <VL53L0X.h>

// ============================================================
// ESP32 Obstacle Avoidance Robot
// Uses 3x VL53L0X Time-of-Flight sensors (left, center, right)
// and a DRV8833 dual motor driver to navigate around obstacles
// ============================================================

// -------------------- MOTOR PINS --------------------
// DRV8833 motor driver control pins
const int AIN1 = 26;  // Right motor - direction pin 1
const int AIN2 = 27;  // Right motor - direction pin 2
const int BIN1 = 32;  // Left motor - direction pin 1
const int BIN2 = 33;  // Left motor - direction pin 2

// PWM configuration for motor speed control
const int PWM_FREQ = 20000;  // 20 kHz frequency (above audible range)
const int PWM_BITS = 8;      // 8-bit resolution (0–255)

// Speed values (0–255 PWM duty cycle)
const int FORWARD_SPEED  = 255;  // Full speed forward
const int TURN_SPEED     = 200;  // Reduced speed for turning
const int REVERSE_SPEED  = 180;  // Moderate speed when reversing

// -------------------- DETECTION DISTANCES --------------------
// Minimum distances (mm) before the robot reacts to an obstacle
const int CENTER_DISTANCE = 300;  // Front sensor threshold (direct wall)
const int SIDE_DISTANCE   = 350;  // Diagonal sensor threshold (angled obstacle)

// -------------------- SENSOR PINS --------------------
// XSHUT pins used to enable/disable each sensor individually
// Required to assign unique I2C addresses at startup
#define XSHUT_LEFT   15
#define XSHUT_RIGHT  2
#define XSHUT_CENTER 4

// Unique I2C addresses assigned to each sensor after boot
#define ADDR_LEFT    0x30
#define ADDR_RIGHT   0x31
#define ADDR_CENTER  0x32

// VL53L0X sensor objects
VL53L0X sensorLeft;
VL53L0X sensorRight;
VL53L0X sensorCenter;

// Calibration offsets (mm) — adjust these if sensors read consistently high or low
int offsetLeft   =   0;
int offsetRight  =   0;
int offsetCenter = -20;  // Center sensor reads 20mm high; subtract to correct

// ============================================================
// MOTOR FUNCTIONS
// Each function sets the two direction pins per motor.
// ledcWrite(pin, 0)   = coast/brake
// ledcWrite(pin, 255) = full power in that direction
// ============================================================

// Drive both motors forward
void moveForward() {
  ledcWrite(AIN1, 0);             // Right motor: pin 1 off
  ledcWrite(AIN2, FORWARD_SPEED); // Right motor: pin 2 on → forward
  ledcWrite(BIN1, 0);             // Left motor: pin 1 off
  ledcWrite(BIN2, FORWARD_SPEED); // Left motor: pin 2 on → forward
}

// Drive both motors backward
void moveBackward() {
  ledcWrite(AIN1, REVERSE_SPEED); // Right motor: pin 1 on → backward
  ledcWrite(AIN2, 0);             // Right motor: pin 2 off
  ledcWrite(BIN1, REVERSE_SPEED); // Left motor: pin 1 on → backward
  ledcWrite(BIN2, 0);             // Left motor: pin 2 off
}

// Spin left: right motor forward, left motor backward
void turnLeft() {
  ledcWrite(AIN1, 0);
  ledcWrite(AIN2, TURN_SPEED);  // Right motor forward
  ledcWrite(BIN1, TURN_SPEED);  // Left motor backward
  ledcWrite(BIN2, 0);
}

// Spin right: left motor forward, right motor backward
void turnRight() {
  ledcWrite(AIN1, TURN_SPEED);  // Right motor backward
  ledcWrite(AIN2, 0);
  ledcWrite(BIN1, 0);
  ledcWrite(BIN2, TURN_SPEED);  // Left motor forward
}

// Stop all motors immediately
void stopRobot() {
  ledcWrite(AIN1, 0);
  ledcWrite(AIN2, 0);
  ledcWrite(BIN1, 0);
  ledcWrite(BIN2, 0);
}

// ============================================================
// SENSOR SETUP
// Sensors all start with address 0x29 (default).
// They are enabled one at a time via XSHUT so each can be
// reassigned a unique address before the next one is enabled.
// ============================================================
void setupSensors() {
  Wire.begin(21, 22);  // Initialize I2C: SDA = GPIO21, SCL = GPIO22

  // Configure all XSHUT pins as outputs
  pinMode(XSHUT_LEFT,   OUTPUT);
  pinMode(XSHUT_RIGHT,  OUTPUT);
  pinMode(XSHUT_CENTER, OUTPUT);

  // Disable all sensors by pulling XSHUT LOW
  digitalWrite(XSHUT_LEFT,   LOW);
  digitalWrite(XSHUT_RIGHT,  LOW);
  digitalWrite(XSHUT_CENTER, LOW);
  delay(10);  // Allow sensors to fully reset

  // --- Enable and configure LEFT sensor ---
  digitalWrite(XSHUT_LEFT, HIGH);  // Wake up left sensor only
  delay(10);
  sensorLeft.setTimeout(500);      // Timeout after 500 ms if no reading
  if (!sensorLeft.init()) {
    Serial.println("LEFT sensor failed!");
    while (1);  // Halt if sensor doesn't respond
  }
  sensorLeft.setAddress(ADDR_LEFT);  // Reassign to 0x30
  Serial.println("LEFT sensor OK");

  // --- Enable and configure RIGHT sensor ---
  digitalWrite(XSHUT_RIGHT, HIGH);
  delay(10);
  sensorRight.setTimeout(500);
  if (!sensorRight.init()) {
    Serial.println("RIGHT sensor failed!");
    while (1);
  }
  sensorRight.setAddress(ADDR_RIGHT);  // Reassign to 0x31
  Serial.println("RIGHT sensor OK");

  // --- Enable and configure CENTER sensor ---
  digitalWrite(XSHUT_CENTER, HIGH);
  delay(10);
  sensorCenter.setTimeout(500);
  if (!sensorCenter.init()) {
    Serial.println("CENTER sensor failed!");
    while (1);
  }
  sensorCenter.setAddress(ADDR_CENTER);  // Reassign to 0x32
  Serial.println("CENTER sensor OK");

  // Start continuous ranging mode on all three sensors
  // (faster than single-shot; readings are always fresh)
  sensorLeft.startContinuous();
  sensorRight.startContinuous();
  sensorCenter.startContinuous();

  Serial.println("All sensors ready");
}

// ============================================================
// SETUP
// Runs once on power-on or reset
// ============================================================
void setup() {
  Serial.begin(115200);

  // Attach all motor pins to LEDC PWM channels (ESP32 Core 3.x API)
  ledcAttach(AIN1, PWM_FREQ, PWM_BITS);
  ledcAttach(AIN2, PWM_FREQ, PWM_BITS);
  ledcAttach(BIN1, PWM_FREQ, PWM_BITS);
  ledcAttach(BIN2, PWM_FREQ, PWM_BITS);

  setupSensors();  // Initialize all three ToF sensors
  stopRobot();     // Ensure motors are off at startup
  delay(500);

  Serial.println("Object avoidance robot started");
}

// ============================================================
// MAIN LOOP
// Reads all three sensors, applies calibration offsets,
// then decides what action to take based on obstacle distances
// ============================================================
void loop() {
  // Read distance from each sensor and apply calibration offset
  int left   = sensorLeft.readRangeContinuousMillimeters()   + offsetLeft;
  int right  = sensorRight.readRangeContinuousMillimeters()  + offsetRight;
  int center = sensorCenter.readRangeContinuousMillimeters() + offsetCenter;

  // Print all three readings to Serial Monitor for debugging
  Serial.print("LEFT 45deg: ");  Serial.print(left);
  Serial.print(" mm | CENTER: "); Serial.print(center);
  Serial.print(" mm | RIGHT 45deg: "); Serial.print(right);
  Serial.println(" mm");

  // ---- CASE 1: Wall directly ahead ----
  // Center sensor detects obstacle within threshold → stop, reverse, then
  // turn toward whichever side has more open space
  if (center <= CENTER_DISTANCE) {
    Serial.println("FRONT WALL DETECTED");
    stopRobot();
    delay(100);

    moveBackward();   // Reverse to create turning room
    delay(350);
    stopRobot();
    delay(100);

    // Compare left vs right clearance and turn toward the more open side
    if (left > right) {
      Serial.println("More space on LEFT, turning LEFT");
      turnLeft();
      delay(550);
    } else {
      Serial.println("More space on RIGHT, turning RIGHT");
      turnRight();
      delay(550);
    }
    stopRobot();
    delay(100);
  }

  // ---- CASE 2: Obstacle on the front-left diagonal ----
  // Left sensor (angled 45°) detects something close → nudge right
  else if (left <= SIDE_DISTANCE) {
    Serial.println("Obstacle on LEFT 45deg, turning RIGHT");
    turnRight();
    delay(300);
    stopRobot();
    delay(50);
  }

  // ---- CASE 3: Obstacle on the front-right diagonal ----
  // Right sensor (angled 45°) detects something close → nudge left
  else if (right <= SIDE_DISTANCE) {
    Serial.println("Obstacle on RIGHT 45deg, turning LEFT");
    turnLeft();
    delay(300);
    stopRobot();
    delay(50);
  }

  // ---- CASE 4: Path is clear ----
  // No obstacles detected within any threshold → drive forward
  else {
    Serial.println("Path clear, moving forward");
    moveForward();
  }

  delay(50);  // Short pause between sensor readings (~20 Hz loop rate)
}
