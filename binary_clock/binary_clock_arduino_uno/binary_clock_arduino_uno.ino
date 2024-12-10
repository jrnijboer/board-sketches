/*

*/
int hours = 0;
int minutes = 0;
int seconds = 0;

int hourStartpin = 2;
int minuteStartpin = 7;

// the setup function runs once when you press reset or power the board
void setup() {
  for (int i = 2; i <= 13; i++ ) {
    pinMode(i, OUTPUT);
  }

  for (int i = A0; i <= A5; i++ ) {
    pinMode(i, OUTPUT);
  }

  hours = 16;
  minutes = 27;
  seconds = 0;
}

// the loop function runs over and over again forever
void loop() {
  seconds = seconds + 1;
  if (seconds == 60) {
    minutes = minutes + 1;
    seconds = 0;

    if (minutes == 60) {
      hours = hours + 1;
      minutes = 0;
      if (hours == 24) {
        hours = 0;
      }
    }
  }

  for (int i = 2; i <= 13; i++ ) {
    digitalWrite(i, LOW);
  }

  for (int i = A0; i < A6 ; i++) {
    digitalWrite(i, LOW);
  }

  int val = 1;
  for (int i = 0; i < 5; i++) {
    if (hours & val ){
      digitalWrite(6 - i, HIGH);
    }
    val *= 2;
  }

  val = 1;
  for (int i = 0; i < 6; i++) {
    if (minutes & val ){
      digitalWrite(12 - i, HIGH);
    }
    val *= 2;
  }

  val = 1;
  for (int i = 0; i < 6; i++) {
    if (seconds & val ){
      digitalWrite(A5 - i, HIGH);
    }
    val *= 2;
  }

  delay(1000);
}
