# Performing-Robots-Fall-2023
**Homework Assignment for 14/09/2023**

<title>Sketch of the BMO robot</title>

![BMO image](https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/journal/13-09/BMO.png?raw=true)
![Sketch image](https://github.com/akhatsuleimenov/Performing-Robots-Fall-2023/blob/main/journal/13-09/20230913_140833.jpg?raw=true)

<br>Storyline of the Movmement</br>
<p>One snowy winter day, you are outside, middle of nowhere, cooking in front of a campfire.</p>
<p>You realise your robot pet is shivering, and decided to cover a blanket above it.</p>
<p>The robot feals warm, and stops shivering.</p>
<p>The robot soon realises that you are cooking something very delicious and approaches toward you.</p>
<p>It is just you and your robot, having a silent dinner.</p>

<code>
// OUTPUT pins
const int Motor11 = 2;
const int Motor12 = 3;

const int Motor21 = 4;
const int Motor22 = 5;

boolean forward = false;

// MAIN
void setup() {
  Serial.begin(9600);
  // MOTOR
  pinMode(Motor11, OUTPUT);
  pinMode(Motor12, OUTPUT);

  pinMode(Motor21, OUTPUT);
  pinMode(Motor22, OUTPUT);
}

void loop() {
  digitalWrite(Motor11, forward);
  digitalWrite(Motor12, !forward);
  digitalWrite(Motor21, !forward);
  digitalWrite(Motor22, forward);
  delay(50);

  // shivering fast
  for (i=0; i<70; i++){
    forward = !forward;
    delay(50);
  }

  // shivering slow
  for (i=0; i<10; i++){
    forward = !forward;
    delay(150);
  }  
  //stop for 10 seconds
  digitalWrite(Motor11, LOW);
  digitalWrite(Motor12, LOW);
  digitalWrite(Motor21, LOW);
  digitalWrite(Motor22, LOW);
  delay(10000);

  // move a bit forward
  analogWrite(Motor11, 100);
  analogWrite(Motor12, LOW);
  analogWrite(Motor21, 100);
  analogWrite(Motor22, LOW);
  delay(800);

  // stop
  digitalWrite(Motor11, LOW);
  digitalWrite(Motor12, LOW);
  digitalWrite(Motor21, LOW);
  digitalWrite(Motor22, LOW);
}
</code>
