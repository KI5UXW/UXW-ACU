#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>


const int callsignSize = 15;  // Maximum size for callsign
const int gridSquareSize = 7; // Maximum size for grid square
char callsign[callsignSize];  // Variable to store callsign
char gridSquare[gridSquareSize]; // Variable to store grid square


const int keyerOutput = 11;  // Pin for Morse code output
int dotDelay = 60;  // Duration of a dot in milliseconds
int currentWPM = 20;
const int dashMultiplier = 3;  // Multiplier for dash duration
int spaceDelay (dotDelay * 7);  // Duration of a space between words (7 times dot duration)
LiquidCrystal_I2C lcd(0x27, 16, 2);
int currentMenu = 1;
int numGroups = 5;
const char charSet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ,/?!.";


const int increaseButtonPin = 2;  // Pin for increasing WPM
const int decreaseButtonPin = 3;  // Pin for decreasing WPM
const int setButtonPin = 4;  // Pin for setting WPM
const int otherButtonPin = 5;


void setup() {
 lcd.begin();
 lcd.setBacklight(1);
 lcd.setCursor(0, 0);
 lcd.print("UXW-ACU");
 lcd.setCursor(0, 1);
 lcd.print("Version 0.0.3");
 delay(1000);
 lcd.clear();
 Serial.begin(9600);  // Initialize serial communication
 pinMode(keyerOutput, OUTPUT);  // Set LED pin as output
 pinMode(increaseButtonPin, INPUT_PULLUP);
 pinMode(decreaseButtonPin, INPUT_PULLUP);
 pinMode(setButtonPin, INPUT_PULLUP);
 pinMode(otherButtonPin, INPUT_PULLUP);
 getOpData();
 displayOpData();
}


void storeCallsign() {
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("See Serial.");
 lcd.setCursor(0, 1);


 int length = 0; // Track the length of the callsign


 while (true) {
   if (Serial.available() > 0) {
     char inputChar = Serial.read();
     if (inputChar == '\n' || length == callsignSize - 1) {
       break;
     }
     callsign[length++] = inputChar;
     lcd.print(inputChar);
   }
   delay(1);
 }


 callsign[length] = '\0';  // Null-terminate the callsign


 // Store callsign in EEPROM
 for (int i = 0; i < callsignSize; ++i) {
   EEPROM.write(i, callsign[i]);
   if (callsign[i] == '\0') {
     break;
   }
 }


 delay(500);  // Display for a short moment
}


void storeGrid() {
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("See Serial.");
 lcd.setCursor(0, 1);


 int length = 0; // Track the length of the gridSquare


 while (true) {
   if (Serial.available() > 0) {
     char inputChar = Serial.read();
     if (inputChar == '\n' || length == gridSquareSize - 1) {
       break;
     }
     gridSquare[length++] = inputChar;
     //lcd.print(inputChar);
   }
   delay(1);
 }


 gridSquare[length] = '\0';  // Null-terminate the grid square


 // Store grid square in EEPROM
 for (int i = callsignSize; i < callsignSize + gridSquareSize; ++i) {
   EEPROM.write(i, gridSquare[i - callsignSize]);
   if (gridSquare[i - callsignSize] == '\0') {
     break;
   }
 }


 delay(500);  // Display for a short moment
}


void getOpData() {
 // Read callsign from EEPROM and store it in the callsign variable
 //Serial.println("Callsign stored in EEPROM:");
 for (int i = 0; i < callsignSize; ++i) {
   char c = EEPROM.read(i);
   //Serial.print(c);
   if (c == '\0') {
     // Null terminator found, end the string
     callsign[i] = '\0';
     break;
   } else {
     callsign[i] = c;
   }
 }
// Serial.println();


 // Read grid square from EEPROM and store it in the gridSquare variable
 //Serial.println("Grid square stored in EEPROM:");
 for (int i = callsignSize; i < callsignSize + gridSquareSize; ++i) {
   char c = EEPROM.read(i);
   //Serial.print(c);
   if (c == '\0') {
     // Null terminator found, end the string
     gridSquare[i - callsignSize] = '\0';
     break;
   } else {
     gridSquare[i - callsignSize] = c;
   }
 }
 //Serial.println();
}


void clearEEPROM() {
 lcd.clear();
 lcd.print("Clearing EEPROM.");
 for (int i = 0; i < callsignSize + gridSquareSize; i++) {
   EEPROM.write(i, 0);  // Write 0 to each EEPROM address
 }
 lcd.clear();
 lcd.print("EEPROM Cleared.");
 delay(500);
}


void displayOpData() {
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("EEPROM Data");
 lcd.setCursor(0, 1);
 lcd.print("Callsign & Grid.");
 delay(1000);
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Call: ");
 lcd.print(callsign);
 lcd.setCursor(0, 1);
 lcd.print("Grid: ");
 lcd.print(gridSquare);
 delay(2000);  // Display for 2 seconds (adjust as needed)
}


void displayWPM() {
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("WPM: ");
 lcd.print(map(currentWPM, 1, 60, 1, 60));  // Map dotDelay to WPM range
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
     currentWPM += 1;
     if (currentWPM > 60) {
       currentWPM = 1;
     }
     displayWPM();
     delay(100);
   } else if (digitalRead(decreaseButtonPin) == HIGH) {
     currentWPM -= 1;
     if (currentWPM < 1) {
       currentWPM = 60;
     }
     displayWPM();
     delay(100);
   }
 }


 if (digitalRead(setButtonPin) == HIGH) {
   dotDelay = (1000 * (1.2 / currentWPM));
   spaceDelay = (dotDelay * 7);
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("WPM: ");
   lcd.print(currentWPM);
   lcd.setCursor(0, 1);
   lcd.print("Dit Timing: ");
   lcd.print(dotDelay);
   delay(1000);
   return;
 }
}


void sendCW() {
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Current WPM: ");
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


void txCWString(String message) {
 for (int i = 0; i < message.length(); i++) {
   char character = message.charAt(i);
   txCW(character);
   delay(dotDelay + dotDelay * dashMultiplier);  // Delay between characters
 }
}


void noMode() {
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("ERROR:");
 lcd.setCursor(0, 1);
 lcd.print("Mode N/A.");
 delay(500);
}


void foxMode() {
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Setting Fox Hunt");
 delay(500);


 String foxMessage = String(callsign) + " Foxhunt";
 lcd.setCursor(0, 1);
 lcd.print("Message: " + foxMessage);


 unsigned long totalDelayMillis = 0;
 int repeatCount = 1;  // Default number of times to repeat the message
 int delayBetweenMessages = 1;  // Default delay between messages in 0.5 minutes
 int runHours = 1;  // Default number of hours to run


 lcd.clear();
 lcd.print("Setting Repeat");
 lcd.setCursor(0, 1);
 lcd.print("A: Inc  V: Dec  O: Set");


 while (true) {
   if (digitalRead(increaseButtonPin) == HIGH) {
     repeatCount = min(99, repeatCount + 1);
     lcd.clear();
     lcd.print("Repeat: " + String(repeatCount));
     delay(200);
   } else if (digitalRead(decreaseButtonPin) == HIGH) {
     repeatCount = max(1, repeatCount - 1);
     lcd.clear();
     lcd.print("Repeat: " + String(repeatCount));
     delay(200);
   } else if (digitalRead(setButtonPin) == HIGH) {
     break;
   }
   delay(1);
 }


 delay(500);
 lcd.clear();
 lcd.print("TX MSG Delay");
 lcd.setCursor(0, 1);
 lcd.print("A: Inc  V: Dec  O: Set");


 while (true) {
   if (digitalRead(increaseButtonPin) == HIGH) {
     delayBetweenMessages = min(120, delayBetweenMessages + 1);
     lcd.clear();
     lcd.print("Delay: " + String(delayBetweenMessages / 2.0) + "min");
     delay(200);
   } else if (digitalRead(decreaseButtonPin) == HIGH) {
     delayBetweenMessages = max(1, delayBetweenMessages - 1);
     lcd.clear();
     lcd.print("Delay: " + String(delayBetweenMessages / 2.0) + "min");
     delay(200);
   } else if (digitalRead(setButtonPin) == HIGH) {
     break;
   }
   delay(1);
 }


 delay(500);
 lcd.clear();
 lcd.print("Foxhunt Hours");
 lcd.setCursor(0, 1);
 lcd.print("A: Inc  V: Dec  O: Set");


 while (true) {
   if (digitalRead(increaseButtonPin) == HIGH) {
     runHours = min(60, runHours + 1);
     lcd.clear();
     lcd.print("Run Hours: " + String(runHours));
     delay(200);
   } else if (digitalRead(decreaseButtonPin) == HIGH) {
     runHours = max(1, runHours - 1);
     lcd.clear();
     lcd.print("Run Hours: " + String(runHours));
     delay(200);
   } else if (digitalRead(setButtonPin) == HIGH) {
     lcd.clear();
     lcd.print("Fox Hunt Set");
     delay(1000);
     break;
   }
   delay(1);
 }


 lcd.clear();
 lcd.print("Fox Hunt Active.");
 delay(2000);


 unsigned long endTime = millis() + (runHours * 3600000UL);


 while (millis() < endTime) {
   lcd.clear();
   lcd.print("Sending Fox Message");
   txCWString(foxMessage);


   // Delay between sending messages
   delay(delayBetweenMessages * 30000);  // Convert 0.5 minutes to milliseconds
 }


 lcd.clear();
 lcd.print("Fox Hunt Complete");
 delay(2000);
}


void learnCWLetters() {
 while (digitalRead(setButtonPin) == LOW) {
   if (digitalRead(increaseButtonPin) == HIGH) {
     numGroups += 1;
     if (numGroups > 100) {  // Adjust the upper limit as needed
       numGroups = 1;
     }
     lcd.clear();
     lcd.print("Groups: ");
     lcd.print(numGroups);
     delay(100);
   } else if (digitalRead(decreaseButtonPin) == HIGH) {
     numGroups -= 1;
     if (numGroups < 1) {
       numGroups = 100;  // Adjust the lower limit as needed
     }
     lcd.clear();
     lcd.print("Groups: ");
     lcd.print(numGroups);
     delay(100);
   }
 }


 if (digitalRead(setButtonPin) == HIGH) {
   for (int i = 0; i < numGroups; ++i) {
     // Generate and send random groups
     char char1 = random('A', 'Z' + 1);
     char char2 = random('A', 'Z' + 1);
     char char3 = random('A', 'Z' + 1);


     txCW(char1);
     txCW(char2);
     txCW(char3);


     // Delay between characters
     delay(spaceDelay * 2);
   }
 }
}


void learnCWNumbers() {
 while (digitalRead(setButtonPin) == LOW) {
   if (digitalRead(increaseButtonPin) == HIGH) {
     numGroups += 1;
     if (numGroups > 100) {  // Adjust the upper limit as needed
       numGroups = 1;
     }
     delay(100);
   } else if (digitalRead(decreaseButtonPin) == HIGH) {
     numGroups -= 1;
     if (numGroups < 1) {
       numGroups = 100;  // Adjust the lower limit as needed
     }
     lcd.clear();
     lcd.print("Groups: ");
     lcd.print(numGroups);
     delay(100);
   }
 }


 if (digitalRead(setButtonPin) == HIGH) {
   for (int i = 0; i < numGroups; ++i) {
     // Generate and send random groups
     char char1 = random('0', '9' + 1);
     char char2 = random('0', '9' + 1);
     char char3 = random('0', '9' + 1);


     txCW(char1);
     txCW(char2);
     txCW(char3);


     // Delay between groups
     delay(spaceDelay * 2);
   }
 }
}


void learnCWPunctuation() {
 while (digitalRead(setButtonPin) == LOW) {
   if (digitalRead(increaseButtonPin) == HIGH) {
     numGroups += 1;
     if (numGroups > 100) {  // Adjust the upper limit as needed
       numGroups = 1;
     }
     delay(100);
   } else if (digitalRead(decreaseButtonPin) == HIGH) {
     numGroups -= 1;
     if (numGroups < 1) {
       numGroups = 100;  // Adjust the lower limit as needed
     }
     lcd.clear();
     lcd.print("Groups: ");
     lcd.print(numGroups);
     delay(100);
   }
 }


 if (digitalRead(setButtonPin) == HIGH) {
   for (int i = 0; i < numGroups; ++i) {
     // Generate and send random groups
     char char1 = random(",/?!."[i % 5]);
     char char2 = random(",/?!."[i % 5]);
     char char3 = random(",/?!."[i % 5]);


     txCW(char1);
     txCW(char2);
     txCW(char3);


     // Delay between groups
     delay(spaceDelay * 2);
   }
 }
}


char randomChar() {
 // Get a random index within the charSet array
 int index = random(sizeof(charSet));


 // Return the character at the random index
 return charSet[index];
}


void learnCW() {
 while (digitalRead(setButtonPin) == LOW) {
   if (digitalRead(increaseButtonPin) == HIGH) {
     numGroups += 1;
     if (numGroups > 10) {  // Adjust the upper limit as needed
       numGroups = 1;
     }
     lcd.clear();
     lcd.print("Groups: ");
     lcd.print(numGroups);
     delay(100);
   } else if (digitalRead(decreaseButtonPin) == HIGH) {
     numGroups -= 1;
     if (numGroups < 1) {
       numGroups = 10;  // Adjust the lower limit as needed
     }
     lcd.clear();
     lcd.print("Groups: ");
     lcd.print(numGroups);
     delay(100);
   }
 }


 if (digitalRead(setButtonPin) == HIGH) {
   for (int i = 0; i < numGroups; ++i) {
     // Generate and send random groups
     char char1 = randomChar();
     char char2 = randomChar();
     char char3 = randomChar();


     txCW(char1);
     txCW(char2);
     txCW(char3);


     delay(spaceDelay);
   }
 }
}


void loop() {
 if (currentMenu == 1){
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("A: OPR   0: RXM");
   lcd.setCursor(0, 1);
   lcd.print("V: TXM   O: NXT");
 } else if (currentMenu == 2) {
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("A: SPD   0: BCN");
   lcd.setCursor(0, 1);
   lcd.print("V: FOX   O: NXT");
 } else if (currentMenu == 3) {
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("A: CLL   0: RDD");
   lcd.setCursor(0, 1);int numGroups = 5;
   lcd.print("V: GRD   O: NXT");
 } else if (currentMenu == 4) {
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("A: CLR   0: N/A");
   lcd.setCursor(0, 1);
   lcd.print("V: LRN   O: NXT");
 } else if (currentMenu == 5) {
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("A: ABC   0: 123");
   lcd.setCursor(0, 1);
   lcd.print("V: .!?   O: NXT");
 }


 while (digitalRead(increaseButtonPin) == LOW && digitalRead(decreaseButtonPin) == LOW && digitalRead(setButtonPin) == LOW && digitalRead(otherButtonPin) == LOW) {
   // Wait for either button to be pressed
   delay(1);
 }


 if (currentMenu == 1){
   if (digitalRead(increaseButtonPin) == HIGH) {
     noMode(); //Operating Mode; goes between TX & RX.
   } else if (digitalRead(decreaseButtonPin) == HIGH) {
     sendCW();
   } else if (digitalRead(setButtonPin) == HIGH) {
     noMode(); //CW RX Mode.
   } else if (digitalRead(otherButtonPin) == HIGH) {
     currentMenu += 1;
     delay(100);
   }
 } else if (currentMenu == 2) {
   if (digitalRead(increaseButtonPin) == HIGH) {
     setSpeed();
   } else if (digitalRead(decreaseButtonPin) == HIGH) {
     foxMode(); //Foxhunting Beacon Mode.
   } else if (digitalRead(setButtonPin) == HIGH) {
     noMode(); //General Purpose Beacon Mode.
   } else if (digitalRead(otherButtonPin) == HIGH) {
     currentMenu += 1;
     delay(100);
   }
 } else if (currentMenu == 3) {
   if (digitalRead(increaseButtonPin) == HIGH) {
     storeCallsign(); //Set callsign.
   } else if (digitalRead(decreaseButtonPin) == HIGH) {
     storeGrid(); //Set grid.
   } else if (digitalRead(setButtonPin) == HIGH) {
     displayOpData(); //Clear EEPROM.
   } else if (digitalRead(otherButtonPin) == HIGH) {
     currentMenu += 1;
     delay(100);
   }
 } else if (currentMenu == 4) {
   if (digitalRead(increaseButtonPin) == HIGH) {
     clearEEPROM(); //Clear EEPROM.
   } else if (digitalRead(decreaseButtonPin) == HIGH) {
     learnCW();
   } else if (digitalRead(setButtonPin) == HIGH) {
     noMode();
   } else if (digitalRead(otherButtonPin) == HIGH) {
     currentMenu += 1;
     delay(100);
   }
 } else if (currentMenu == 5) {
   if (digitalRead(increaseButtonPin) == HIGH) {
     learnCWLetters();
   } else if (digitalRead(decreaseButtonPin) == HIGH) {
     learnCWPunctuation();
   } else if (digitalRead(setButtonPin) == HIGH) {
     learnCWNumbers();
   } else if (digitalRead(otherButtonPin) == HIGH) {
     currentMenu = 1;
     delay(100);
   }
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
   case '/':
     Serial.print("/");
     blinkMorse("-..-.");
     break;
   case ' ':
     Serial.print(" ");
     delay(spaceDelay);
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
 delay(dotDelay);  // Gap between characters
}

