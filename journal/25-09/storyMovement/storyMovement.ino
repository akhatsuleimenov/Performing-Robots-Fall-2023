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
