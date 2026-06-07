/*
  Modified FreeRTOS Button & Blink Example
*/
#define LED1_PIN 2
#define BUTTON_PIN 23

// Task handles
TaskHandle_t BlinkTaskHandle = NULL;
TaskHandle_t ControlTaskHandle = NULL;

// Volatile variables for ISR
volatile uint32_t lastInterruptTime = 0;
const uint32_t debounceDelay = 200; // Increased slightly for physical buttons

void IRAM_ATTR buttonISR() {
  uint32_t currentTime = millis();
  if (currentTime - lastInterruptTime < debounceDelay) {
    return;
  }
  lastInterruptTime = currentTime;

  // Notify the Control Task from the ISR
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  vTaskNotifyGiveFromISR(ControlTaskHandle, &xHigherPriorityTaskWoken);
  
  if (xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }
}

// Dedicated task to manage the state safely outside the ISR
void ControlTask(void *parameter) {
  bool taskSuspended = false;
  for (;;) {
    // Wait indefinitely for a notification from the ISR
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    
    taskSuspended = !taskSuspended;
    
    if (taskSuspended) {
      vTaskSuspend(BlinkTaskHandle);
      // It is now perfectly safe to use Serial.print
      Serial.println("--> BlinkTask SUSPENDED");
      digitalWrite(LED1_PIN, LOW); // Ensure LED turns off when suspended
    } else {
      vTaskResume(BlinkTaskHandle);
      Serial.println("--> BlinkTask RESUMED");
    }
  }
}

void BlinkTask(void *parameter) {
  for (;;) { 
    digitalWrite(LED1_PIN, HIGH);
    Serial.println("BlinkTask: LED ON");
    vTaskDelay(1000 / portTICK_PERIOD_MS); 
    
    digitalWrite(LED1_PIN, LOW);
    Serial.println("BlinkTask: LED OFF");
    Serial.print("BlinkTask running on core ");
    Serial.println(xPortGetCoreID());
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); 

  // Create the Control Task first so its handle is valid for the ISR
  xTaskCreatePinnedToCore(
    ControlTask,
    "ControlTask",
    2048,
    NULL,
    2,                  // Higher priority than BlinkTask to handle button promptly
    &ControlTaskHandle,
    1
  );

  // Create the Blink Task
  xTaskCreatePinnedToCore(
    BlinkTask,         
    "BlinkTask",       
    2048,              // Reduced stack size (10000 was excessively high for a simple blink)
    NULL,              
    1,                 
    &BlinkTaskHandle,  
    1                  
  );

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);
}

void loop() {
  // Empty
}