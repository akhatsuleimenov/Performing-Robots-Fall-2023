// OUTPUT pins
const int Motor11 = 2;
const int Motor12 = 3;

const int Motor21 = 4;
const int Motor22 = 5;

// unsigned long prevCount = 0;
int count = 0;
int prevCount = 0;

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

  // frameCount Algorithms
  count++;
  if (count > 3000) count = 0;
  Serial.println();
  if (count % 300 < 150) {
    Serial.print("front");

    digitalWrite(Motor11, LOW);
    digitalWrite(Motor12, HIGH);
    digitalWrite(Motor21, LOW);
    digitalWrite(Motor22, HIGH);
  }
  else {
    Serial.print("back");
    digitalWrite(Motor11, HIGH);
    digitalWrite(Motor12, LOW);
    digitalWrite(Motor21, HIGH);
    digitalWrite(Motor22, LOW);
  }
}
