#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Keypad.h>

// WiFi and SMTP configuration
#define WIFI_SSID "Wifi_SSID"
#define WIFI_PASSWORD "Wifi_Password"
#define SMTP_server "smtp.gmail.com"
#define SMTP_Port 465
#define sender_email "hariomagrahari06@gmail.com"
#define sender_password "aqwf gkmx swev kjer"

// Recipient emails and names
#define Recipient_email1 "1ds21et028@dsce.edu.in"
#define Recipient_email2 "1ds21et015@dsce.edu.in"
#define Recipient_email3 "1ds21et013@dsce.edu.in"
#define Recipient_email4 "1ds21et109@dsce.edu.in"
#define Recipient_name "ESP32 User"

// Keypad configuration
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns

// Define the keymap
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Connect keypad ROW0, ROW1, ROW2, and ROW3 to these ESP32 pins
byte rowPins[ROWS] = {16, 17, 18, 19}; 

// Connect keypad COL0, COL1, COL2, and COL3 to these ESP32 pins
byte colPins[COLS] = {4, 5, 21, 22}; 

// Create the Keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Relay configuration
const int relayPin = 23;

// TOF sensor configuration
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// GPIO pins for feedback
const int correctOTPIndicatorPin = 27;
const int wrongOTPIndicatorPin = 12;

// Timer for distance measurement
unsigned long previousMillis = 0;
const long interval = 1000; // interval at which to measure distance (milliseconds)

// SMTP and OTP variables
SMTPSession smtp;
SMTP_Message message;
ESP_Mail_Session session;
String otp = "";
String inputSequence = "";
bool otpGenerated = false;

// Function to generate OTP
String generateOTP() {
  return String(random(1000, 10000));
}

// Function to send OTP via email
void sendOTP(String otp) {
  session.server.host_name = SMTP_server;
  session.server.port = SMTP_Port;
  session.login.email = sender_email;
  session.login.password = sender_password;
  session.login.user_domain = "";

  message.sender.name = "ESP 32";
  message.sender.email = sender_email;
  message.subject = "Your OTP Code";

  // Add recipients
  message.addRecipient(Recipient_name, Recipient_email1);
  message.addRecipient(Recipient_name, Recipient_email2);
  message.addRecipient(Recipient_name, Recipient_email3);
  message.addRecipient(Recipient_name, Recipient_email4);

  String htmlMsg = "<div style=\"color:#000000;\"><h1> Hello User!</h1><p>Your OTP is: <strong>" + otp + "</strong></p></div>";
  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  if (!smtp.connect(&session))
    return;

  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email: " + smtp.errorReason());
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println();

  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println("\nWiFi connected.");
  Serial.println("IP address: " + WiFi.localIP());

  // Initialize the relay pin as an output
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  // Initialize GPIO pins for feedback
  pinMode(correctOTPIndicatorPin, OUTPUT);
  pinMode(wrongOTPIndicatorPin, OUTPUT);
  digitalWrite(correctOTPIndicatorPin, LOW);
  digitalWrite(wrongOTPIndicatorPin, LOW);

  // Initialize the I2C communication for the VL53L0X sensor
  Wire.begin(32, 33); // SDA, SCL

  // Initialize the VL53L0X sensor
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  Serial.println(F("VL53L0X initialized."));
}

void loop() {
  char key = keypad.getKey();
  
  if (key) {
    Serial.println(key);
    inputSequence += key; // Append the entered key to the sequence
    
    // Generate OTP if '*' is pressed and OTP hasn't been generated yet
    if (key == '*' && !otpGenerated) {
      otp = generateOTP();
      sendOTP(otp);
      otpGenerated = true; // Set flag to indicate OTP has been generated and sent
    }

    // Check if 4 digits have been entered
    if (inputSequence.length() == 4) {
      if (inputSequence == otp) {
        Serial.println("Door Open");
        digitalWrite(relayPin, HIGH);
        digitalWrite(correctOTPIndicatorPin, HIGH); // Set indicator high
        delay(10000); // Keep the solenoid lock open for 10 seconds
        digitalWrite(relayPin, LOW);
        digitalWrite(correctOTPIndicatorPin, LOW); // Turn off indicator
        Serial.println("Enter OTP:");
        otpGenerated = false; // Reset OTP generation flag
      } else {
        // Incorrect OTP entered
        Serial.println("Wrong OTP");
        digitalWrite(wrongOTPIndicatorPin, HIGH); // Set indicator high
        delay(1000); // Show "Wrong OTP" for 1 second
        digitalWrite(wrongOTPIndicatorPin, LOW); // Turn off indicator
        Serial.println("Enter OTP:");
      }
      inputSequence = ""; // Reset input sequence
    }
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Measure distance
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
      Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);

      // Generate and send OTP if distance is less than 130mm and OTP hasn't been generated yet
      if (measure.RangeMilliMeter < 80 && !otpGenerated) {
        otp = generateOTP();
        sendOTP(otp);
        otpGenerated = true; // Set flag to indicate OTP has been generated and sent
      }
    } else {
      Serial.println("Out of range");
    }
  }
}
