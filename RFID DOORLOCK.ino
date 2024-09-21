#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10  // Slave Select pin
#define RST_PIN 9  // Reset pin
#define greenLed 6 // Green LED for authorized access
#define redLed 7   // Red LED for unauthorized access
#define buzzer 8   // Buzzer for unauthorized access
#define servoPin 5 // Servo control pin

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
Servo myServo;                       // Create Servo instance

void setup() {
  Serial.begin(9600);     // Initialize serial communication
  SPI.begin();            // Initialize SPI bus
  mfrc522.PCD_Init();     // Initialize the MFRC522 reader
  myServo.attach(servoPin); // Attach the servo to the control pin

  // Initialize LEDs and buzzer
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  Serial.println("Place your RFID card/tag near the reader...");
}

void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Show the UID on the Serial Monitor
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") + String(mfrc522.uid.uidByte[i], HEX);
  }
  content.toUpperCase(); // Convert to uppercase for comparison
  Serial.print("Card UID: ");
  Serial.println(content);

  // Check if the UID matches an authorized card (example UID)
  if (content.equalsIgnoreCase("C3F4C1A6") || 
      content.equalsIgnoreCase("DEADBEEF")) {
    Serial.println("Authorized access");
    digitalWrite(greenLed, HIGH); // Turn on green LED
    myServo.write(90);             // Unlock or move servo
    delay(2000);                   // Keep LED on for 2 seconds
    myServo.write(0);              // Return servo to initial position
    digitalWrite(greenLed, LOW);
  } else {
    Serial.println("Access denied");
    digitalWrite(redLed, HIGH);   // Turn on red LED
    digitalWrite(buzzer, HIGH);   // Sound the buzzer
    delay(2000);                  // Keep LED and buzzer on for 2 seconds
    digitalWrite(redLed, LOW);
    digitalWrite(buzzer, LOW);
  }
}
