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
  // shivering fast
  delay(2000);
  for (int i = 0; i < 70; i++) {
    digitalWrite(Motor11, forward);
    digitalWrite(Motor12, !forward);
    digitalWrite(Motor21, !forward);
    digitalWrite(Motor22, forward);
    forward = !forward;
    delay(50);
  }

  // shivering slow
  for (int i = 0; i < 8; i++) {
    forward = !forward;
    digitalWrite(Motor11, forward);
    digitalWrite(Motor12, !forward);
    digitalWrite(Motor21, !forward);
    digitalWrite(Motor22, forward);
    delay(150);
  }
  //stop for 10 seconds
  digitalWrite(Motor11, LOW);
  digitalWrite(Motor12, LOW);
  digitalWrite(Motor21, LOW);
  digitalWrite(Motor22, LOW);
  delay(8000);

  // move a bit forward
  digitalWrite(Motor11, HIGH);
  digitalWrite(Motor12, LOW);
  digitalWrite(Motor21, HIGH);
  digitalWrite(Motor22, LOW);
  delay(500);

  // stop
  digitalWrite(Motor11, LOW);
  digitalWrite(Motor12, LOW);
  digitalWrite(Motor21, LOW);
  digitalWrite(Motor22, LOW);
  delay(20000);
}
