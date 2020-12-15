#include "DHT.h" // Including DHT22 library 
#include "LiquidCrystal.h" // Including LCD library 
#include "Adafruit_Sensor.h" // Including the DHT22 library

#define DHTPIN 2 // Declaring pin 6 for communicating to DHT22 sensor 
#define DHTTYPE DHT11 // Declaring the type of DHT sensor we are using (DHT22 or DHT11)

DHT dht(DHTPIN, DHTTYPE); // Declaring DHT connection and type 
LiquidCrystal lcd_count(12,11,10,9,8,7); // Declaring LCD connections for counting no. of people 
LiquidCrystal lcd_temp(22,23,24,25,26,27);// Declaration for LCD connections for displaying temp and fan speed

int LDR = A0; 
int LED = 44;
int count=0;

 int Motor_Pin1 = 29; // Input 1 on L293D 
 int Motor_Pin2 = 28; // Input 2 on L293D 
 int Enable = 30; //Enable 1 on L293D 

 void IN() 
 { 
  count++; 
  lcd_count.clear(); 
  lcd_count.print("Person In Room:"); 
  lcd_count.setCursor(0,1); 
  lcd_count.print(count); 
  }

 void OUT() 
 {
  count--; 
  lcd_count.clear(); 
  lcd_count.print("Person In Room:"); 
  lcd_count.setCursor(0,1); 
  lcd_count.print(count); 
 }

 void setup() 
 {
  //temperature controlled fan section 
  pinMode(Motor_Pin1, OUTPUT); 
  pinMode(Motor_Pin2, OUTPUT);
  pinMode(Enable, OUTPUT);
  lcd_temp.begin(16,2); // Initializes the 16x2 
  LCD dht.begin(); // Initializes DHT sensor 

  // ir sensor and led lights section 
  lcd_count.begin(16,2); 
  lcd_count.print("Visitor Counter");
  delay(2000); 
  pinMode(4, INPUT); 
  pinMode(5, INPUT); 
  pinMode(6, OUTPUT); 
  lcd_count.clear(); 
  lcd_count.print("Person In Room:"); 
  lcd_count.setCursor(0,1); 
  lcd_count.print(count); 
  }

  void loop()
  {
    if(digitalRead(4)== LOW ) 
    IN(); 
    if(digitalRead(5)== HIGH) 
    OUT(); 

    if(count<=0) 
    { 
      //temperature section 
      analogWrite(Enable,0); // fan is off lcd_temp.clear();
      lcd_temp.print("Nobody In Room"); 
      lcd_temp.setCursor(0,1); 
      lcd_temp.print("Fan Is Off"); 
      delay(200); 

      // counter section 
      lcd_count.clear(); 
      digitalWrite (LED , LOW ); 
      lcd_count.clear(); 
      lcd_count.print("Nobody In Room"); 
      lcd_count.setCursor(0,1); 
      lcd_count.print("Light Is Off"); 
      }

      else
      {
        lcd_temp.clear(); // Clear LCD 
        float temp = dht.readTemperature(); // Reading the temperature in Celsius 
        
        // Validating received data 
        if (isnan(temp)) 
        { 
          lcd_temp.print("Failed to read"); 
          delay(1000); 
          return; 
          }

          // setting the lcd_temp 
          lcd_temp.setCursor(0,0); 
          lcd_temp.print("Per:"); 
          lcd_temp.print(count); // Writing temperature in the first row 
          lcd_temp.print("Tmp:");
          lcd_temp.print(temp); // Writing temperature in the first row 
          lcd_temp.print(" C"); 
          lcd_temp.setCursor(0,1); // Setting the position to display fan speed


          if (count > 1 && count < 5) 
          {
            int reading=analogRead(LDR); 
            int bright=reading; 
            delay(500); 
            analogWrite(LED, bright);

            if(temp <30 ) 
            { 
              // If the temperature less than 30 
              analogWrite(Enable,0); // 0% PWM duty cycle 
              lcd_temp.print("Fan OFF "); 
              delay(100); 
              }

            else if(temp>=30 & temp<40) 
              { // If the temperature is between 30 & 40 
                analogWrite(Enable, 127); // 50% of maximum duty cycle value (255). 
                lcd_temp.print("Fan Speed:50% "); 
                delay(100); 
                }
                
            else if(temp>=40) 
              { 
                // If the temperature is above 40 
                analogWrite(Enable, 255); // 100% duty cycle 
                lcd_temp.print("Fan Speed: 100% "); 
                delay(100);

              } 
          }


          else if (count > 5 && count < 10) 
          {
            int reading=analogRead(LDR); 
            int bright=reading; 
            delay(500); 
            analogWrite(LED, bright); 
            
            if(temp <25 ) 
            
            { 
              // If the temperature less than 25 
              analogWrite(Enable,0); // 0% PWM duty cycle 
              lcd_temp.print("Fan OFF "); 
              delay(100);
              } 
            else if(temp>=25 & temp<30) 
            { 
              // If the temperature is between 25 & 30 
              analogWrite(Enable, 77); // 30% of maximum duty cycle value (255). 
              lcd_temp.print("Fan Speed: 30% "); 
              delay(100);
            } 
            
            else if(temp>=30 & temp<35) 
            {
              // If the temperature is between 30 & 35 
              analogWrite(Enable, 157) // 60% of maximum duty cycle value (255). 
              lcd_temp.print("Fan Speed: 60% "); 
              delay(100); 
              }
            
            else if(temp>=35) 
            { 
              // If the temperature is above 35 
              analogWrite(Enable, 255); // 100% duty cycle 
              lcd_temp.print("Fan Speed: 100% ");
              delay(100); 
              }
              
              }

              else if (count > 10 ) 
              {
                int reading=analogRead(LDR); 
                int bright=reading; 
                delay(500); 
                analogWrite(LED, bright); 
                analogWrite(Enable, 255); // 100% duty cycle 
                lcd_temp.print("Fan Speed: 100% "); 
                delay(100); 
                }
              
            
        
            
          
        }
      
    digitalWrite(Motor_Pin1, LOW); // To drive the motor in a particular direction 
    digitalWrite(Motor_Pin2, HIGH); // To drive the motor in a particular direction 
    delay(2000); // 2 seconds delay
    
    
    
    }
