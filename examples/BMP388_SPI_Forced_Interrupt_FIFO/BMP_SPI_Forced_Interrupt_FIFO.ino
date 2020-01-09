//////////////////////////////////////////////////////////////////////////////////////////////////
// BMP388_DEV - SPI Communications, Default Configuration, Forced Conversion, Interrupts, FIFO 
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <BMP388_DEV.h>                             // Include the BMP388_DEV.h library

#define NO_OF_MEASUREMENTS   10                     // Number of measurements to be stored in the FIFO before reading

volatile float temperature[NO_OF_MEASUREMENTS];     // Create the temperature, pressure and altitude array variables
volatile float pressure[NO_OF_MEASUREMENTS];
volatile float altitude[NO_OF_MEASUREMENTS];
volatile uint32_t sensorTime;                       // Sensor time
volatile boolean fifoDataReady = false;                
BMP388_DEV bmp388(10);                              // Instantiate (create) a BMP388_DEV object and set-up for SPI operation on digital pin D10

void setup() 
{
  Serial.begin(115200);                             // Initialise the serial port
  bmp388.begin();                                   // Default initialisation, place the BMP388 into SLEEP_MODE 
  bmp388.enableFIFOInterrupt();                     // Enable the BMP388's FIFO interrupts on the INT pin
  bmp388.usingInterrupt(digitalPinToInterrupt(2));     // Invoke the SPI usingInterrupt() function
  attachInterrupt(digitalPinToInterrupt(2), interruptHandler, RISING);   // Set interrupt to call interruptHandler function on D2 
  bmp388.enableFIFO();                              // Enable the BMP388's FIFO
  bmp388.setFIFONoOfMeasurements(NO_OF_MEASUREMENTS);   // Store 10 measurements in the FIFO before reading
  Serial.println(F("Please wait for 10 seconds...")); // Wait message
}

void loop() 
{
  bmp388.startForcedConversion();                   // Start BMP388 forced conversion (if we're in SLEEP_MODE)
  if (fifoDataReady)                                // If the FIFO data is ready: get the 10 measurement readings
  {
    fifoDataReady = false;                           // Clear the FIFO data ready flag
    for (uint16_t i = 0; i < NO_OF_MEASUREMENTS; i++)     // Display the results 
    {
      Serial.print(i + 1);                            
      Serial.print(F(" : "));
      Serial.print(temperature[i]);                         
      Serial.print(F("*C   "));
      Serial.print(pressure[i]);    
      Serial.print(F("hPa   "));
      Serial.print(altitude[i]);
      Serial.println(F("m"));  
    }
    Serial.print(F("Sensor Time: "));
    Serial.println(sensorTime);
    Serial.println();
    Serial.println(F("Please wait for 10 seconds..."));   // Wait message  
  }
  delay(1000)                                       // Wait for 1 second
}

void interruptHandler()
{  
  bmp388.getFIFOData(temperature, pressure, altitude, sensorTime);   // Get the FIFO data
  fifoDataReady = true;                               // Set the FIFO data ready flag  
}
