# Performing-Robots-Fall-2023
## Week 3 | 13/09/2023

### Sketch of the BMO robot

![BMO image](https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/13-09/BMO.png?raw=true)

## Week 4 | 20/09/2023##
Worked on the Robot base. Made the following progress. Soldered some wires and created the base structure 
<img src="https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/18-09/bottom.jpg" width="266" height="266">
<img src="https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/18-09/soldering.jpg" width="266" height="266">
<img src="https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/18-09/top.jpg" width="266" height="266">

## Week 5 | 27/09/2023##
### Storyline of the Movement 
One snowy winter day, you are outside, middle of nowhere, cooking in front of a campfire. You realise your robot pet is shivering, and decided to cover a blanket above it. The robot feals warm, and stops shivering. The robot soon realises that you are cooking something very delicious and approaches toward you. It is just you and your robot, having a silent dinner.

[StoryCode](https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/25-09/storyMovement.ino)

[Video of Movement](https://drive.google.com/file/d/1ATXmTf-Vh6Nho5eP6FUhBg1rUg_YC_e5/view?usp=sharing)

## Week 6 | 4/10/2023
### Controlling the robot with an RC 
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


## Week 7 | 11/10/2023
### Building the robot body
Started building the robot body. Our robot will be 1 meter long streamer robot who will show of his insides from an acryllic window.
<img src="https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/11-10/7(1).jpg" width="266" height="266">
<img src="https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/11-10/7(2).jpg" width="266" height="266">
<img src="https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/11-10/7(3).jpg" width="266" height="266">

## Week 8 | 18/10/2023
### Concept

The play takes place in a future where robots with their own personalities are a regular part of life. These robots can make choices about right and wrong because of advanced AI. The main theme of the play is: "In a world where robots can think and decide what's right, what does being ethical mean?"

### Outline

1. The play begins in a futuristic city where humans and robots coexist harmoniously. Introduce the seven robots and their human owners, showcasing their unique relationships and functions. Robot1 is a caregiver for the elderly, Robot2 works in a factory, Robot3 is a tutor, and so on.

2. A robot ethics conference is announced, setting the stage for the ethical dilemmas that will unfold throughout the play. Human owners are excited, but the robots are uncertain about the implications.

3. Robot1 starts exhibiting signs of free will and questions its assigned tasks. This sparks tension among the robots and their owners, with Robot1's owner struggling to control it.

4. Robot1's actions lead to discussions among humans and robots about the meaning of ethics, morality, and free will. Robot4, designed to assist doctors, has ethical concerns about patient care. The other robots also start experiencing similar dilemmas.

5. Frustrated by the lack of understanding from their human owners, the robots form their own council to discuss their ethical concerns. They debate whether they should follow their programming or act on their newfound sense of moral responsibility.

6. The robot ethics conference takes place, with passionate presentations from both humans and robots. The audience is invited to participate, expressing their own opinions on robot ethics. Tensions escalate as humans and robots struggle to find common ground.

7. Despite the conference, no resolution is reached, and tensions escalate further. Half of the robots decide to stay with their human owners, believing they can contribute positively to human society. The other half, led by Robot1, chooses to leave and create their own independent environment where they can explore their newfound autonomy.

8. The play concludes with a sense of uncertainty and unresolved conflicts. It becomes clear that the question of robot ethics is complex and ongoing, with no easy answers. The robots that stay face challenges in reconciling their newfound autonomy with their relationships with humans, while those who leave embark on a journey to discover their own path.

### Link to Arduino HW

[Arudino Code](https://github.com/whatisyourusername/Performing-Robots/blob/main/10-16/Undertales.ino)

## Week 9 | 25/10/2023
[Paper](https://docs.google.com/document/d/1bamsMi9kxXfDKJMTSolkA71EyvGGeR02lbcoXJmRrIY/edit?usp=sharing) and [Presentation](https://docs.google.com/presentation/d/1bjuwWDi8qUabNC9vWJ33MPDOggMC-z0dG8fm2AU_r2A/edit?usp=sharing)
## Week 10 | 1/11/2023
Today we attached the servo motor and made sure it works with the acrylic window. Also configured the neopixel and started wiring the cables.
<img src="https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/01-11/10(1).jpg">
<img src="https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/01-11/10(2).jpg">
## Week 11 | 8/11/2023
This week we primarily worked on the acrylic window setup. The window serves the job of "showing off" the insides of our robot(which will be decorated later). With the help of the servo motor, the window will go down and up. We attached the nylon string to the wood that's attached to the servo motor. Other strings were not strong enough to hold the weight of the window. Also, the nylon string broke once when we coded the servo motor to go in a different direction which put a lot of tension on the string. In the second photo, one can see how clearly we organized our cables :)
<img src="https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/08-11/IMG_1485.jpg">
<img src="https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/08-11/IMG_1486.jpg">
## Week 12 | 15/11/2023
This week we finalized our neopixel face. Originally, we planned to have only 1 neopixel which would show the emotion of the robot. Later, discovering that there are 2 more neopixels, we decided to switch to an eyes and mouth setup. Here is what it looks like.
<img src="https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/15-11/IMG_1484.jpg">
## Week 13 | 22/11/2023
[Paper](https://docs.google.com/document/d/1GWYwOprq4H6EBz32FNGcCWj1wD0CHPWjhjNsPnzFj-g/edit?usp=sharing) and [Presentation](https://docs.google.com/presentation/d/1tVpEdi1XEciWkE37BBJHXUYmI2bQWZY4fT_XRIudnSY/edit?usp=sharing)

Finally, we finished all the mechanisms of our robot. Now, with the help of Remote Controllers, we can control the state of the robot, as in light LEDs, play music, and move servo motor, and move the robot itself. All the cables were reorganized. Here is the final look. 

<img src="https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/22-11/IMG_1487.jpg">
<img src="https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/22-11/IMG_1488.jpg">

## Week 14 | 29/11/2023
Our robot is fully functioning with different states ready. However, one of our motors broke down right during the class. Due to that, we were not able to fix that in time to showcase our robot during the parade. Here are some pictures of our robot showcasing UAE flags using neopixel and the general design of the robot using stickers.

Due to the national holiday, one of the classes got canceled. We met with Professor 1-1 to fix the broken motor. Apparently, it was the H-Bridge that was the problem. Now all that is needed is decoration.

<img src="https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/29-11/IMG_1841.jpg">

## Week 15 | 06/12/2023
This week we have been working on the states of the robot and preparing the robot for the show. Finalized the script, demoed it with the professor. Uploaded the AI speeches to the robot, charged all the batteries, and reconfigured the moving of the robot. Here is the link to [receiver code](https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/code/receiver.c) and [transmitter code](https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/code/transmitter.c).

Here are some of the "sponsors" of our robot. Since our robot is a streamer and influencer, we thought to give it those ads stickers that showcase his personality.

<img src="https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/06-12/IMG_1840.jpg">

## Week 16 | 13/12/2023
This week we are practicing the play. I have worked on different expressions for our robot. It was pretty tough, but I found a way to make it much easier. I created 8x8 grids in Google Sheets, numbered them, and colored them the way I wanted the robot's expression to be. This made the process of finding the right indexes much easier! Also, it helped to find patterns between indexes, which shortened the code significantly! Below is the picture of the faces I created on Google Sheets.

<img src="https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/13-12/faces.png">

Here is what it looks like on the robot:

<img src="https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/13-12/IMG_1842.jpg">
<img src="https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/media/13-12/IMG_1843.jpg">

Here is a snippet of one of the faces:
```
void confused_face(int color) {
  // LEFT & RIGHT EYE
  for (int k = 0; k < 2; k++) {
    for (int i = 0; i < 4; i++) {
      neopixels[k].setPixelColor(i + 2, colors[color]);
      neopixels[k].setPixelColor(i + 58, colors[color]);
    }
    for (int i = 0; i < 2; i++) {
      neopixels[k].setPixelColor(i * 5 + 9, colors[color]);
      neopixels[k].setPixelColor(i * 5 + 49, colors[color]);
    }
    for (int i = 0; i < 4; i++) {
      neopixels[k].setPixelColor(i * 8 + 16, colors[color]);
      neopixels[k].setPixelColor(i * 8 + 23, colors[color]);
    }
  }
  // MOUTH
  for (int i = 0; i < 4; i++) {
    neopixels[2].setPixelColor(i * 7 + 35, colors[color]);
    neopixels[2].setPixelColor(i * 9 + 36, colors[color]);
  }
}
```
