// in below lines we are defining the pins that we have used for our 3 sensors and led's
#define Soil_sense 3
#define PIR_1 2
#define PIR_2 4

#define Soil_sense_LED 13
#define PIR_2_LED 12
#define timer_LED 11
#define PIR_1_LED 10

// Declaring variables to check and change the state of the led's
volatile bool PIR_1_LED_state = false;
volatile bool Soil_sense_LED_state = false;
volatile bool PIR_2_LED_state = false;
volatile bool timer_LED_state = false;

int a = 2; // A variable to track the timer value

// Function to set up the timer for the LED blink
void starttimmerfunc()
  {
     // Reset timer registers
  	TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
    // Set the timer compare value to 31249, which gives us a 2ms interval
    OCR1A = 31249; //2ms
    // Set the timer mode to CTC and prescaler to 1024
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A);
  }
  

ISR(TIMER1_COMPA_vect){
    // Display the timer's current value on the serial monitor.
  Serial.print("Led Timer: ");
  Serial.print(a);
  Serial.println(" milliseconds");
  // Change the LED's state linked to pin 6. 
  digitalWrite(timer_LED, digitalRead(timer_LED) ^ 1);
  a = a+2;
}

void setup() {
  pinMode(PIR_1_LED, OUTPUT);
  pinMode(Soil_sense_LED, OUTPUT);
  pinMode(PIR_2_LED, OUTPUT);
  pinMode(timer_LED, OUTPUT);
  
  pinMode(PIR_1, INPUT);
  pinMode(PIR_2, INPUT);
  pinMode(Soil_sense, INPUT);
  
  Serial.begin(9600);
  starttimmerfunc();
  
  // Enable interrupts for the pins connected to the sensors
  noInterrupts();
  PCIFR |= bit(PCIE2);
  PCICR |= bit(PCIE2);
  PCMSK2 |= bit (PCINT18);
  PCMSK2 |= bit (PCINT19);
  PCMSK2 |= bit (PCINT20);
  interrupts();
  
}

void loop() {
 
}

ISR(PCINT2_vect)
{
  if(digitalRead(Soil_sense) == HIGH) 
  {
    // Check if the soil moisture sensor has detected a rise in moisture level
    // change the state of the led on pin 6
    Soil_sense_LED_state = !Soil_sense_LED_state;
    digitalWrite(Soil_sense_LED, Soil_sense_LED_state);
    Serial.println("Soil Moisture Detected");
  }
  if(digitalRead(PIR_1) == HIGH) 
  {
    // Check if the PIR sensor connected to pin 3 has detected motion
    // change the state of the LED
    PIR_1_LED_state = !PIR_1_LED_state;
    digitalWrite(PIR_1_LED, PIR_2_LED_state);
    Serial.println("motion dected by 1st sensor ");
  }
  if(digitalRead(PIR_2) == HIGH) 
  {
    PIR_2_LED_state = !PIR_2_LED_state;
  	digitalWrite(PIR_2_LED, PIR_2_LED_state);
  	Serial.println("motion dectected by 2nd sensor ");
  }  
}
