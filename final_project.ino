//Librerías
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <MQ135.h>
#include <SoftwareSerial.h>

#define PIN_MQ135 A2 // MQ135 Analog Input Pin
#define DHTPIN 2 // DHT Digital Input Pin
#define DHTTYPE DHT11 

MQ135 mq135_sensor(PIN_MQ135);
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows
DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial BT(3,6);

float temperature, humidity;
int izqA = 5;
int izqB = 7;
int derA = 9;
int derB = 10;


void setup()
{
  Serial.begin(9600);
  
  //DHT sensor
  dht.begin();
  
  //Display
  lcd.init();
  lcd.backlight();

  //Car
  pinMode(derA, OUTPUT);
  pinMode(derB, OUTPUT);
  pinMode(izqA, OUTPUT);
  pinMode(izqB, OUTPUT);       
  
  BT.begin(9600);


}


void loop(){
  carMovement();
  sensors();
  //revisar delays
}

void sensors(){
  
  //Humidity, Temperature and Air Quality
  
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);

  lcd.setCursor(0,0);
  lcd.print("H");
  lcd.print(humidity);
  lcd.print("% T");

  lcd.print(temperature);
  lcd.print("C ");

  lcd.setCursor(0,1);
  lcd.print("QAir ");
  lcd.print(correctedPPM);
  lcd.print("ppm");
  
  //Serial.println(humidity);
  //Serial.println(temperature);

  delay(3000);
  lcd.clear();
}


void carMovement() {
  // put your main code here, to run repeatedly:
  if (BT.available()){
    int a=BT.read();
    if (a==97){
      //forward
      digitalWrite (derA, HIGH);
      digitalWrite (derB, LOW);
      digitalWrite (izqA, HIGH);
      digitalWrite (izqB, LOW);
      //Serial.println(a);
    }
    if (a==100){
      //left
      digitalWrite (derA, LOW);
      digitalWrite (derB, LOW);
      digitalWrite (izqA, HIGH);
      digitalWrite (izqB, LOW);
      //Serial.println(a);
    }
    if (a==114){
      //stop
      digitalWrite (derA, LOW);
      digitalWrite (derB, LOW);
      digitalWrite (izqA, LOW);
      digitalWrite (izqB, LOW);
      //Serial.println(a);   
    }
    if (a==105){
      //right
      digitalWrite (derA, HIGH);
      digitalWrite (derB, LOW);
      digitalWrite (izqA, LOW);
      digitalWrite (izqB, LOW);
      //Serial.println(a);
    }
     //Serial.println(a);
  }
  if(BT.available())  // Si llega un dato por el monitor serial se envía al puerto BT
  {
     BT.write(Serial.read());
  }
delay(100);
}
