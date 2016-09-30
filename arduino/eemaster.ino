/*
  Serial Event example

 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and
 clears it.

 A good test for this is to try it with a GPS receiver
 that sends out NMEA 0183 sentences.

 Created 9 May 2011
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/SerialEvent

 */
 #include <SPI.h>
#include <SD.h>

File myFile;
String inputString = "";         // a string to hold incoming data
String inputStringB = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
unsigned char sd_status;
String filename;

void setup() {
  // initialize serial:
  Serial.begin(19200, SERIAL_8N1);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  // SD card initialize
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done...");
  sd_status = 0;
  filename = "";
}

void loop() {
  serialEvent(); //call the function
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.print(inputString);
    switch (sd_status) {
    case 0:
      inputStringB = inputString;
      inputStringB.trim();
      if ( inputStringB == "write_file" ) {
        Serial.println(" -> ready to write file");
        sd_status = 1;
      }
      break;
    case 1:
      inputStringB = inputString;
      inputStringB.trim();
      filename = inputStringB;
      if (filename != "") {
        Serial.print("got the filename = ");
        Serial.println(filename);
        // open the file. note that only one file can be open at a time,
        // so you have to close this one before opening another.
        if (SD.exists(filename)) {
          SD.remove(filename);
        }
        myFile = SD.open(filename, FILE_WRITE);
      }
      sd_status = 2;
      break;
    case 2: 
      inputStringB = inputString;
      inputStringB.trim();
      if ( inputStringB != "EOF" ) {
        myFile.println(inputStringB);  
        Serial.print("OK");
      }
      else {
        myFile.close();
        Serial.println("done.");
        sd_status = 3;  
      }
      break;
     default:
      Serial.print("sd_status = 0");
      sd_status = 0;
      break; 
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

