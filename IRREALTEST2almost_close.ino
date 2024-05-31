 #define IR_PIN D2 // IR sensor output pin
#define LED_PIN_1 D5 // LED 1 pin
#define LED_PIN_2 D6 // LED 2 pin

bool led1State = false; // Variable to store the state of LED 1
bool led2State = false; // Variable to store the state of LED 2
bool obstacleDetected = false; // Variable to store if an obstacle is currently detected
unsigned long detectionStartTime = 0; // Variable to store the time when the obstacle was first detected
unsigned long led2StartTime = 0; // Variable to store the time when LED2 was turned on
int detectionCount1 = 0; // Counter for the number of detections for LED 1
int detectionCount2 = 0; // Counter for the number of detections for LED 2

void setup() {
  pinMode(IR_PIN, INPUT);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  Serial.begin(115200); // Initialize serial communication for debugging
  Serial.println("IR Sensor LED Control");
}

void loop() {
  int irValue = digitalRead(IR_PIN); // Read the value from the IR sensor

  if (irValue == LOW) { // Obstacle detected
    if (!obstacleDetected) {
      // First time obstacle detected
      obstacleDetected = true;
      detectionStartTime = millis(); // Record the detection start time
    } else {
      // Calculate detection duration
      unsigned long detectionDuration = millis() - detectionStartTime;
      
      if (detectionDuration > 5000) {
        // Obstacle detected for more than 5 seconds
        if (!led2State) {
          // Turn on LED2 if it was off
          led2State = true;
          digitalWrite(LED_PIN_2, HIGH);
          led2StartTime = millis(); // Record the LED2 start time
          Serial.println("Obstacle detected for more than 5 seconds. LED 2 ON");
        }
      }
    }
  } else {
    if (obstacleDetected) {
      // Obstacle no longer detected
      obstacleDetected = false;
      unsigned long detectionDuration = millis() - detectionStartTime;

      if (detectionDuration < 5000) {
        // Obstacle detected for less than 5 seconds
        // Toggle LED1
        led1State = !led1State;
        digitalWrite(LED_PIN_1, led1State ? HIGH : LOW);
        Serial.println(led1State ? "Obstacle detected for less than 5 seconds. LED 1 ON" : "Obstacle detected for less than 5 seconds. LED 1 OFF");
      } else if (detectionDuration > 5000 && led2State) {
        // Obstacle detected for more than 5 seconds and LED2 is on
        // Turn off LED2 after the second obstacle detection
        detectionCount2++;
        if (detectionCount2 == 2) {
          led2State = false;
          digitalWrite(LED_PIN_2, LOW);
          Serial.println("Second obstacle detected for more than 5 seconds. LED 2 OFF");
          detectionCount2 = 0; // Reset the detection count
        }
      }
    }
  }

  delay(100); // Small delay to debounce
}
