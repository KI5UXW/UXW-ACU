#include <LiquidCrystal_I2C.h>

const int keyerOutput = 11;  // Pin for Morse code output
int dotDelay = 5;  // Duration of a dot in milliseconds
int currentWPM = 5;
const int dashMultiplier = 3;  // Multiplier for dash duration
int spaceDelay;  // Duration of a space between words (7 times dot duration)
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int increaseButtonPin = 2;  // Pin for increasing WPM
const int decreaseButtonPin = 3;  // Pin for decreasing WPM
const int setButtonPin = 4;  // Pin for setting WPM
const int otherButtonPin = 5;

void setup() {
  lcd.begin(16, 2);
  lcd.setBacklight(1);
  lcd.setCursor(0, 0);
  lcd.print("UXW-MCA");
  lcd.setCursor(0, 1);
  lcd.print("Version 0.0.1");
  delay(1000);
  lcd.clear();
  Serial.begin(9600);  // Initialize serial communication
  pinMode(keyerOutput, OUTPUT);  // Set LED pin as output
  pinMode(increaseButtonPin, INPUT_PULLUP);
  pinMode(decreaseButtonPin, INPUT_PULLUP);
  pinMode(setButtonPin, INPUT_PULLUP);
  pinMode(otherButtonPin, INPUT_PULLUP);
}

void displayWPM() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WPM: ");
  lcd.print(map(dotDelay, 1, 60, 1, 60));  // Map dotDelay to WPM range
}

void displayCharacter(char character) {
  lcd.setCursor(0, 0);
  lcd.print("Now sending...");
  lcd.setCursor(0, 1);
  lcd.print(character);
}

void setSpeed() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Please set");
  lcd.setCursor(0, 1);
  lcd.print("operating speed.");
  delay(500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("A = Higher WPM");
  lcd.setCursor(0, 1);
  lcd.print("V = Lower WPM");
  
  while (digitalRead(setButtonPin) == LOW) {
    if (digitalRead(increaseButtonPin) == HIGH) {
      dotDelay += 1;
      if (dotDelay > 60) {
        dotDelay = 60;
      }
      displayWPM();
      delay(100);
    } else if (digitalRead(decreaseButtonPin) == HIGH) {
      dotDelay -= 1;
      if (dotDelay < 1) {
        dotDelay = 1;
      }
      displayWPM();
      delay(100);
    }
  }

  if (digitalRead(setButtonPin) == HIGH) {
    lcd.clear();
    lcd.print("Speed set.");
	currentWPM = dotDelay;
    dotDelay = 60.0 / (dotDelay / 20.0);
    delay(500);

    lcd.clear();
    spaceDelay = 7.0 * dotDelay;
    return;
  }
}


void sendCW() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Current WPM:");
  lcd.print(currentWPM);
  while (Serial.available() == 0) {
    // Wait for serial input
    delay(1);
  }
  while (Serial.available() > 0) {
    char character = Serial.read();
    txCW(character);
  }
}

void rxCW() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ERROR:");
  lcd.setCursor(0, 1);
  lcd.print("Decoder N/A.");
  delay(500);
}

void operateMode() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ERROR:");
  lcd.setCursor(0, 1);
  lcd.print("Mode N/A.");
  delay(500);
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("A: SPD   0: OPR");
  lcd.setCursor(0, 1);
  lcd.print("V: TXM   O: RXM");

  while (digitalRead(increaseButtonPin) == LOW && digitalRead(decreaseButtonPin) == LOW && digitalRead(setButtonPin) == LOW && digitalRead(otherButtonPin) == LOW) {
    // Wait for either button to be pressed
    delay(1);
  }

  if (digitalRead(increaseButtonPin) == HIGH) {
    setSpeed();
  } else if (digitalRead(decreaseButtonPin) == HIGH) {
    sendCW();
  } else if (digitalRead(setButtonPin) == HIGH) {
    operateMode();
  } else if (digitalRead(otherButtonPin) == HIGH) {
    rxCW();
  }
}

void txCW(char character) {
  lcd.clear();
  displayCharacter(character);
  // Convert character to Morse code
  switch (character) {
    case 'a':
    case 'A':
      Serial.print("A");
      blinkMorse(".-");
      break;
    case 'b':
    case 'B':
      Serial.print("B");
      blinkMorse("-...");
      break;
    case 'c':
    case 'C':
      Serial.print("C");
      blinkMorse("-.-.");
      break;
    case 'd':
    case 'D':
      Serial.print("D");
      blinkMorse("-..");
      break;
    case 'e':
    case 'E':
      Serial.print("E");
      blinkMorse(".");
      break;
    case 'f':
    case 'F':
      Serial.print("F");
      blinkMorse("..-.");
      break;
    case 'g':
    case 'G':
      Serial.print("G");
      blinkMorse("--.");
      break;
    case 'h':
    case 'H':
      Serial.print("H");
      blinkMorse("....");
      break;
    case 'i':
    case 'I':
      Serial.print("I");
      blinkMorse("..");
      break;
    case 'j':
    case 'J':
      Serial.print("J");
      blinkMorse(".---");
      break;
    case 'k':
    case 'K':
      Serial.print("K");
      blinkMorse("-.-");
      break;
    case 'l':
    case 'L':
      Serial.print("L");
      blinkMorse(".-..");
      break;
    case 'm':
    case 'M':
      Serial.print("M");
      blinkMorse("--");
      break;
    case 'n':
    case 'N':
      Serial.print("N");
      blinkMorse("-.");
      break;
    case 'o':
    case 'O':
      Serial.print("O");
      blinkMorse("---");
      break;
    case 'p':
    case 'P':
      Serial.print("P");
      blinkMorse(".--.");
      break;
    case 'q':
    case 'Q':
      Serial.print("Q");
      blinkMorse("--.-");
      break;
    case 'r':
    case 'R':
      Serial.print("R");
      blinkMorse(".-.");
      break;
    case 's':
    case 'S':
      Serial.print("S");
      blinkMorse("...");
      break;
    case 't':
    case 'T':
      Serial.print("T");
      blinkMorse("-");
      break;
    case 'u':
    case 'U':
      Serial.print("U");
      blinkMorse("..-");
      break;
    case 'v':
    case 'V':
      Serial.print("V");
      blinkMorse("...-");
      break;
    case 'w':
    case 'W':
      Serial.print("W");
      blinkMorse(".--");
      break;
    case 'x':
    case 'X':
      Serial.print("X");
      blinkMorse("-..-");
      break;
    case 'y':
    case 'Y':
      Serial.print("Y");
      blinkMorse("-.--");
      break;
    case 'z':
    case 'Z':
      Serial.print("Z");
      blinkMorse("--..");
      break;
    case '0':
      Serial.print("0");
      blinkMorse("-----");
      break;
    case '1':
      Serial.print("1");
      blinkMorse(".----");
      break;
    case '2':
      Serial.print("2");
      blinkMorse("..---");
      break;
    case '3':
      Serial.print("3");
      blinkMorse("...--");
      break;
    case '4':
      Serial.print("4");
      blinkMorse("....-");
      break;
    case '5':
      Serial.print("5");
      blinkMorse(".....");
      break;
    case '6':
      Serial.print("6");
      blinkMorse("-....");
      break;
    case '7':
      Serial.print("7");
      blinkMorse("--...");
      break;
    case '8':
      Serial.print("8");
      blinkMorse("---..");
      break;
    case '9':
      Serial.print("9");
      blinkMorse("----.");
      break;
    case '.':
      Serial.print(".");
      blinkMorse(".-.-.-");
      break;
    case ',':
      Serial.print(",");
      blinkMorse("--..--");
      break;
    case '?':
      Serial.print("?");
      blinkMorse("..--..");
      break;
    case '!':
      Serial.print("!");
      blinkMorse("-.-.--");
      break;
    case ' ':
      Serial.print(" ");
      delay(spaceDelay);  // Space between words
      break;
    default:
      // If the character is not a supported symbol, ignore it
      break;
  }
}

void blinkMorse(String morseCode) {
  for (int i = 0; i < morseCode.length(); i++) {
    char morseChar = morseCode.charAt(i);
    if (morseChar == '.') {
      // Dot
      digitalWrite(keyerOutput, HIGH);  // Full brightness for dot
      delay(dotDelay);
    } else if (morseChar == '-') {
      // Dash
      digitalWrite(keyerOutput, HIGH);  // Full brightness for dash
      delay(dashMultiplier * dotDelay);
    }
    digitalWrite(keyerOutput, LOW);  // Turn off LED
    delay(dotDelay);  // Gap between symbols
  }
  delay(spaceDelay);  // Gap between characters
}


