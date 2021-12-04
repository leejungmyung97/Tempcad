#include <Wire.h>                       
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

#define ENA 12
#define EN1 9
#define EN2 8

int RED = 10;
int GREEN = 11;
int buttonState = 0;
int button = 2;
int buzzer = 4;
int temp;

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(ENA, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(button,INPUT);
  Serial.begin(9600);

  Serial.begin(9600);

  Serial.println("Adafruit MLX90614 test");  

  mlx.begin();  
}

void loop() {
  /*Print on progress screen*/
//  temp = Serial.parseInt();
  temp = mlx.readObjectTempC();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Come Close");
  lcd.setCursor(0,1);
  lcd.print("Please...");

  
  if (Serial.available() > 0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Check...");
    lcd.setCursor(0,1);
    lcd.print("Temp :");
    lcd.print(temp);
    lcd.print("'C");

    /*Temperature output on the manager screen*/
    Serial.print("온도를 측정중입니다...");
    Serial.print("현재 온도 :");
    Serial.println(temp);
    delay(500);
    lcd.clear();
  }

  /*Print the light color according to temperature.*/
  if(temp > 37.0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temp is too high!");
    lcd.setCursor(0,1);
    lcd.print("Temp :");
    lcd.print(temp);
    lcd.print("'C");
    digitalWrite(RED, HIGH);
    
    /*If the temperature exceeds the range, it makes 3 sounds.*/
    for (int i = 0; i < 3; i++){
      tone (buzzer, 262, 500);
      delay(500);
      noTone;
      lcd.clear();
    }
    digitalWrite(RED, LOW);
  }

  else if(temp > 33 && temp<=37.0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("PASS...!!");
    lcd.setCursor(0,1);
    lcd.print("Temp :");
    lcd.print(temp);
    lcd.print("'C");
    digitalWrite(GREEN, HIGH); /*Green LED ON*/
    
    Serial.println(temp);
    /*Water pump operating*/
    while(1){
      if(digitalRead(button) == HIGH){
        digitalWrite(EN1, HIGH);   // 모터A 설정 
        digitalWrite(EN2, LOW);  
        analogWrite(ENA, 255); 

        delay(2000); /*Delay 2 second*/

        digitalWrite(EN1, LOW);   // 모터A 설정 
        digitalWrite(EN2, LOW); 
        break;
     }
    } 
    
    delay(3000); /*Delay 3 second*/
    digitalWrite(GREEN, LOW); 
  }
  /*When body temperature is outside the normal range*/
  else if(temp > 0 &&temp<=33){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Try Again...");
      Serial.println("온도를 다시 측정해주세요");
      delay(1500);
      lcd.clear();
   }
   /*In the initial state both LED OFF*/
  else if(temp == 0){
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);
    
   }
}
