int pushButton = 19;

// the setup function runs once when you press reset or power the board
void setup() {
  for (int i = 2; i < 14; i++)  {
      pinMode(i, OUTPUT);
  }

  for (int i = 2; i < 14; i++) {
      digitalWrite(i, LOW);
  }

  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT);
}

void loop() {
  int buttonState = digitalRead(pushButton);

  if (buttonState == HIGH) {
    for (int i = 2; i < 14; i++) {
      digitalWrite(i, LOW);
    }

    for (int i = 0; i < 4; i++ ) {
      for (int i = 2; i < 8; i++) {
        digitalWrite(i, !digitalRead(i));
        digitalWrite(i + 6, !digitalRead(i + 6));
        delay(50);
      }
      delay(200);
    }

    for (int i = 2; i < 14; i++) {
      digitalWrite(i, LOW);
    }
    
    int randNumber = random(2, 8);
    Serial.println(randNumber);
    for (int i = 2; i <= randNumber; i++) {
      digitalWrite(i, HIGH);
    }

    randNumber = random(8, 14);
    Serial.println(randNumber);
    for (int i = 8; i <= randNumber; i++) {
      digitalWrite(i, HIGH);
    }  
  }
}
