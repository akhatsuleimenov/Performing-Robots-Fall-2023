# Controlling the robot with an RC

- First, we used the code from [this page](https://github.com/michaelshiloh/resourcesForClasses/blob/master/src/arduinoSketches/hobbyRC/minimalMoreChannels/minimalMoreChannels.ino) to test out the connection between the RC and the hobby RC shield.
- We used pins 8, 7, 4, 2 for the RC channels.
- Once we got the code running, we twisted and pushed on the RC knob and trigger to see if there were any changes in the seriel monitor.
- Once we confirmed that we could reliably control the numbers being displayed for all four radio channels, we connected the H-bridge back onto the arduino board, this time using pins 5, 6, and 10, 11 as these were free.
- We then modified [the code used previously](https://github.com/michaelshiloh/resourcesForClasses/blob/master/src/arduinoSketches/hobbyRC/minimalMoreChannels/minimalMoreChannels.ino) to introduce variables that represent turning left and right, and going forward or backward. the variables just controlled the motors through high/low values based on what we've learned from the previous homework assignments.
- We deciphered the values for each of these variables in the void loop() function using the following code as a baseline:

```
// use whichever channel is the trigger to control forward and reverse movement
if (rc_values[RC_CH3] > 17) reverse();
if (rc_values[RC_CH3] < 13) forward();
if ((rc_values[RC_CH3] < 17) && (rc_values[RC_CH3] < 13)) stop();

// use whichever channel is the steering wheel to turn:
if (rc_values[RC_CH2] > 18) turnRight();
```

- Through trial and error, we would note down the baseline value of the RC control, and then move the knob or trigger to gauge how we wanted the robot to react. We decided that the trigger (channel 2) would be forward and backwards and the knob (channel 1) would be left and right.
- Once we figured out those values, we integrated them into the function and tested out the robot's movement. below is the final code along with a link to the video demo:
- [Video link](https://drive.google.com/file/d/1w8YngO4UvmPTCI5fD_FNyaO5eG2fgP0n/view?usp=sharing)

```
// install this library from the library manager
// by Mike "GreyGnome" Schwager
#include <EnableInterrupt.h>

#define SERIAL_PORT_SPEED 9600
#define RC_NUM_CHANNELS  4

#define RC_CH1  1
#define RC_CH2  2
#define RC_CH3  3
#define RC_CH4  4

#define RC_CH1_PIN  8
#define RC_CH2_PIN  7
#define RC_CH3_PIN  4
#define RC_CH4_PIN  2

uint16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile uint16_t rc_shared[RC_NUM_CHANNELS];

void rc_read_values() {
  noInterrupts();
  memcpy(rc_values, (const void *)rc_shared, sizeof(rc_shared));
  interrupts();
}

void calc_input(uint8_t channel, uint8_t input_pin) {
  if (digitalRead(input_pin) == HIGH) {
    rc_start[channel] = micros();
  } else {
    uint16_t rc_compare = (uint16_t)(micros() - rc_start[channel]);
    rc_shared[channel] = rc_compare;
  }
}

void calc_ch1() {
  calc_input(RC_CH1, RC_CH1_PIN);
}
void calc_ch2() {
  calc_input(RC_CH2, RC_CH2_PIN);
}
void calc_ch3() {
  calc_input(RC_CH3, RC_CH3_PIN);
}
void calc_ch4() {
  calc_input(RC_CH4, RC_CH4_PIN);
}

void reverse() {
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(11, HIGH);
}

void forward() {
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(11, LOW);
}

void turnRight() {
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, HIGH);
}

void turnLeft() {
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, LOW);
}

void stop() {
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}

void setup() {
  Serial.begin(SERIAL_PORT_SPEED);

  pinMode(RC_CH1_PIN, INPUT);
  pinMode(RC_CH2_PIN, INPUT);
  pinMode(RC_CH3_PIN, INPUT);
  pinMode(RC_CH4_PIN, INPUT);

  enableInterrupt(RC_CH1_PIN, calc_ch1, CHANGE);
  enableInterrupt(RC_CH2_PIN, calc_ch2, CHANGE);
  enableInterrupt(RC_CH3_PIN, calc_ch3, CHANGE);
  enableInterrupt(RC_CH4_PIN, calc_ch4, CHANGE);
}

void loop() {
  rc_read_values();

  Serial.print("CH1:"); Serial.print(rc_values[RC_CH1]); Serial.print("\t");
  Serial.print("CH2:"); Serial.print(rc_values[RC_CH2]); Serial.print("\t");
  Serial.print("CH3:"); Serial.print(rc_values[RC_CH3]); Serial.print("\t");
  Serial.print("CH4:"); Serial.println(rc_values[RC_CH4]);

  delay(200);
  // use whichever channel is the trigger to control forward and reverse movement
if (rc_values[RC_CH2] < 1500) reverse();
if (rc_values[RC_CH2] > 1550) forward();
if ((rc_values[RC_CH2] < 1520) && (rc_values[RC_CH2] > 1500)) stop();

// use whichever channel is the steering wheel to turn:
 if (rc_values[RC_CH1] > 1535) turnRight();

 if (rc_values[RC_CH1] < 1520) turnLeft();
}
```
