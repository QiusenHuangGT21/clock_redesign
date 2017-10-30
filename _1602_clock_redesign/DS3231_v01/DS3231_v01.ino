#include <DS3231.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>
#include <MsTimer2.h>
dht DHT;

#define DHT22_PIN 4
#define Alarm 7

LiquidCrystal_I2C lcd(0x3f,16,2); 

DS3231 Clock;
bool Century=false;
bool h12;
bool PM;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;
boolean a = false;
boolean b = false;
int Year;
int Month;
int Date;
int DoW;
int Hour;
int Minute;
int Second;
int cdYear;
int cdMonth;
int cdDate;
int cdDoW;
int cdHour;
int cdMinute;
int cdSecond;
int Dis;
int x;
int alarmMinute;
int alarmHour;
int alarmLast;
int alarmDet;
String Mediate;
String Time;
String Output;


void setup() {
  int chk = DHT.read22(DHT22_PIN);
	Wire.begin();
	// Start the serial interface
	Serial.begin(9600);
  pinMode(Alarm,OUTPUT);
  
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.setCursor(2,1);
  Clock.setClockMode(false);  // set to 24h
  Dis = Clock.getMonth(Century);
   if (Dis <10){
    lcd.print("0");
    lcd.print(Dis);
   } else {
    lcd.print(Clock.getMonth(Century), DEC);
   }
  
  lcd.print('/');
  // then the date
  Dis = Clock.getDate();
  if (Dis <10){
    lcd.print("0");
    lcd.print(Dis);
   } else {
    lcd.print(Clock.getDate(), DEC);
   }
  
  lcd.setCursor(0,1);
  // and the day of the week
  
  Dis = Clock.getDoW();

  switch (Dis){
    case 1:
      lcd.print("M");
      break;
    case 2:
      lcd.print("T");
      break;
    case 3:
      lcd.print("W");
      break;
    case 4:
      lcd.print("R");
      break;
    case 5:
      lcd.print("F");
      break;
    case 6:
      lcd.print("S");
      break;
    case 7:
      lcd.print("N");
      break;
         
  }
  lcd.setCursor(10,1);
  lcd.print(DHT.humidity);
  
  lcd.setCursor(0,0);
  // Finally the hour, minute, and second
 Dis = Clock.getHour(h12, PM);
  if (Dis <10){
    lcd.print("0");
    lcd.print(Dis);
   } else {
    lcd.print(Clock.getHour(h12, PM), DEC);
   }
   
   lcd.print(':');
  
  Dis = Clock.getMinute();
   if (Dis <10){
    lcd.print("0");
    lcd.print(Dis);
   } else {
    lcd.print(Clock.getMinute(), DEC);
   }
  
  lcd.print(':');

  Dis = Clock.getSecond();
   if (Dis <10){
    lcd.print("0");
    lcd.print(Dis);
   } else {
    lcd.print(Dis);
   }
  
  lcd.print("  ");
  // Display the temperature
  lcd.print(DHT.temperature);
  lcd.print('C');
  
  /*lcd.setCursor(2,0);
  lcd.print(':');
  lcd.setCursor(4,0);
  lcd.print(':');
  lcd.setCursor(10,0);
  lcd.print("T=");

  lcd.setCursor(4,1);
  lcd.print('/');
  lcd.setCursor(6,1);
  lcd.print('/');
*/
  lcd.setCursor(15,1);
  lcd.print('%');
}

void loop() {
	// send what's going on to the serial monitor.
	// Start with the year

    while (Serial.available()>0) {
      Time = "";
  while(Serial.available()>0){
      Time += char(Serial.read());
    delay(2);
    }

    Mediate = Time.substring(0,3);
    if (Mediate == "set"){
   

//  201707285225200
    Serial.println(Time);
    Mediate = Time.substring(5,7);
    Year = Mediate.toInt();
    Clock.setYear(Year);

    Mediate = Time.substring(7,9);
    Month = Mediate.toInt();
    Clock.setMonth(Month);

    Mediate = Time.substring(9,11);
    Date = Mediate.toInt();
    Clock.setDate(Date);

    Mediate = Time.substring(11,12);
    DoW = Mediate.toInt(); 
    Clock.setDoW(DoW);

    Mediate = Time.substring(12,14);
    Hour = Mediate.toInt(); 
    Clock.setHour(Hour);

    Mediate = Time.substring(14,16);
    Minute = Mediate.toInt(); 
    Clock.setMinute(Minute);

    Mediate = Time.substring(16,18);
    Second = Mediate.toInt(); 
    Clock.setSecond(Second);

    lcd.setCursor(2,1);
  
   Dis = Clock.getMonth(Century);
   if (Dis <10){
    lcd.print("0");
    lcd.print(Dis);
   } else {
    lcd.print(Clock.getMonth(Century), DEC);
   }
  
  lcd.print('/');
  // then the date
  Dis = Clock.getDate();
  if (Dis <10){
    lcd.print("0");
    lcd.print(Dis);
   } else {
    lcd.print(Clock.getDate(), DEC);
   }
  
  lcd.setCursor(0,1);
  // and the day of the week
  
  Dis = Clock.getDoW();

  switch (Dis){
    case 1:
      lcd.print("M");
      break;
    case 2:
      lcd.print("T");
      break;
    case 3:
      lcd.print("W");
      break;
    case 4:
      lcd.print("R");
      break;
    case 5:
      lcd.print("F");
      break;
    case 6:
      lcd.print("S");
      break;
    case 7:
      lcd.print("N");
      break;
         
  }
  lcd.setCursor(10,1);
  lcd.print(DHT.humidity);
  
  lcd.setCursor(0,0);
  // Finally the hour, minute, and second
 Dis = Clock.getHour(h12, PM);
  if (Dis <10){
    lcd.print("0");
    lcd.print(Dis);
   } else {
    lcd.print(Clock.getHour(h12, PM), DEC);
   }
   
   lcd.print(':');
  
  Dis = Clock.getMinute();
   if (Dis <10){
    lcd.print("0");
    lcd.print(Dis);
   } else {
    lcd.print(Clock.getMinute(), DEC);
   }
  
  lcd.print(':');

  Dis = Clock.getSecond();
   if (Dis <10){
    lcd.print("0");
    lcd.print(Dis);
   } else {
    lcd.print(Dis);
   }
  
  lcd.print(" ");
  // Display the temperature
  lcd.print("T=");
  lcd.print(DHT.temperature);
  
  /*lcd.setCursor(2,0);
  lcd.print(':');
  lcd.setCursor(4,0);
  lcd.print(':');
  lcd.setCursor(10,0);
  lcd.print("T=");

  lcd.setCursor(4,1);
  lcd.print('/');
  lcd.setCursor(6,1);
  lcd.print('/');
*/
  lcd.setCursor(15,1);
  lcd.print('%');

    
  }  else if (Mediate == "scd"){
    Mediate = Time.substring(5,7);
    cdYear = Mediate.toInt();
    
    Mediate = Time.substring(7,9);
    cdMonth = Mediate.toInt();
    
    Mediate = Time.substring(9,11);
    cdDate = Mediate.toInt();

    Mediate = Time.substring(11,13);
    cdHour = Mediate.toInt(); 
    
    Mediate = Time.substring(13,15);
    cdMinute = Mediate.toInt(); 
    
    Mediate = Time.substring(15,17);
    cdSecond = Mediate.toInt(); 
        
  }else if (Mediate == "alm"){
    
    Mediate = Time.substring(3,5);
    alarmHour = Mediate.toInt();
    
    Mediate = Time.substring(5,7);
    alarmMinute = Mediate.toInt();
    
    Mediate = Time.substring(7,8);
    alarmLast= Mediate.toInt();
    Serial.print("alarm set for: ");
    Serial.print(alarmHour);
    Serial.print(':');
    Serial.println(alarmMinute);
    Serial.print("alarm last ");
    Serial.print(alarmLast);
    Serial.println(" min(s)");
    Hour = Clock.getHour(h12, PM);
    if (Hour == alarmHour){
      b = true;
    }
  }
    }

Dis = Clock.getSecond();
    
    
   
    lcd.setCursor(6,0);
   if (Dis <10){
    
    lcd.print("0");
    lcd.print(Dis);
   } else {
    lcd.print(Dis);
   }
   
   if (Dis == 0){
   
    int chk = DHT.read22(DHT22_PIN);
    lcd.setCursor(10,0);
    lcd.print(DHT.temperature);
    lcd.setCursor(10,1);
    lcd.print(DHT.humidity);
    Dis = Clock.getMinute();
    lcd.setCursor(3,0);
    if (Dis == alarmMinute || b == true){
      a = true;
    }
   if (Dis <10){
    lcd.print("0");
    lcd.print(Dis);
   } else {
    lcd.print(Dis);
   }
   if (Dis == 0){
    Dis = Clock.getHour(h12, PM);
    lcd.setCursor(0,0);
    if (Dis == alarmHour){
      b = true;
    }
  if (Dis <10){
    lcd.print("0");
    lcd.print(Dis);
   } else {
    lcd.print(Dis);
   }
   if (Dis == 0){
Dis = Clock.getDoW();
  lcd.setCursor(0,1);
  switch (Dis){
 case 1:
      lcd.print("M");
      break;
    case 2:
      lcd.print("T");
      break;
    case 3:
      lcd.print("W");
      break;
    case 4:
      lcd.print("R");
      break;
    case 5:
      lcd.print("F");
      break;
    case 6:
      lcd.print("S");
      break;
    case 7:
      lcd.print("N");
      break;
  }
    
    Dis = Clock.getDate();
    lcd.setCursor(5,1);
  if (Dis <10){
    lcd.print("0");
    lcd.print(Dis);
   } else {
    lcd.print(Dis);
   }
   if (Dis == 1){

    Dis = Clock.getMonth(Century);
    lcd.setCursor(5,1);
   if (Dis <10){
    lcd.print("0");
    lcd.print(Dis);
   } else {
    lcd.print(Dis);
   }
   if (Dis == 1){

   lcd.setCursor(0,1);
  lcd.print("20");

 lcd.print(Clock.getYear());
       }//y
      }//m
     }//d
    }//h
   }//m
  
	
	// Tell whether the time is (likely to be) valid
	
	/*if (Clock.oscillatorCheck()) {
		Serial.print(" O+");
	} else {
		Serial.print(" O-");
	}
*/
while (a&&b){
  Serial.print("inLoop");
  for ( int x = 1000; x>0; x = x - 90 ){
    digitalWrite(Alarm, HIGH);
    delay(x); 
    digitalWrite(Alarm, LOW);    // turn the LED off by making the voltage LOW
  delay(x);  
  }// wait for a second
  for (int y = 0 ; y<11 ; y=y+1){
     digitalWrite(Alarm, HIGH);
    delay(100); 
    digitalWrite(Alarm, LOW);    // turn the LED off by making the voltage LOW
  delay(100);  
  }
   alarmDet= Clock.getMinute();
  Serial.print(Dis);
  if (alarmDet == alarmMinute + alarmLast){
    a = false;
    b = false;
  }
  Serial.println(a);
}
delay(1000);
}
