#include <PID_v1.h> 

// --- PINS ---
const int airspeedPin = A0;      // Pitot Tube Sensor
const int hydraulicValvePin = 9; // PWM Output for Hydraulic Valve
const int feedbackPin = A1;      // Potentiometer on the piston (Position Feedback)

// --- CONSTANTS ---
double setpoint, input, output;
double Kp=2, Ki=5, Kd=1; // PID Tuning parameters
PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

const float RETRACT_SPEED = 300.0; // Speed to retract wings (km/h)
const float EXTEND_SPEED = 150.0;  // Speed to extend wings (km/h)

void setup() {
    pinMode(hydraulicValvePin, OUTPUT);
    myPID.SetMode(AUTOMATIC);
    myPID.SetOutputLimits(0, 255); // PWM range for valve
}

void loop() {
    // 1. Get Airspeed
    float currentAirspeed = readAirspeed();

    // 2. Logic for Target Wing Position
    if (currentAirspeed >= RETRACT_SPEED) {
        setpoint = 1023; // Fully Retracted (Closed)
    } else if (currentAirspeed <= EXTEND_SPEED) {
        setpoint = 0;    // Fully Extended (Open)
    }

    // 3. Feedback Loop (Current position of the hydraulic piston)
    input = analogRead(feedbackPin);

    // 4. Compute PID (Smoothly moves the valve)
    myPID.Compute();

    // 5. Send Signal to Hydraulic Valve
    analogWrite(hydraulicValvePin, output);

    delay(10);
}

float readAirspeed() {
    // Basic sensor conversion logic
    int val = analogRead(airspeedPin);
    return map(val, 0, 1023, 0, 500); 
}
