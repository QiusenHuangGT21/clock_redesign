#include <DS3231.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>
#include <MsTimer2.h>
#include <EEPROM.h>


#define DHT22_PIN 4
#define Alarm 7


LiquidCrystal_I2C lcd(0x3f, 16, 2);
dht DHT;
DS3231 Clock;
int Dis;
bool Century = false;
bool h12;
bool PM;
bool alarmEN;
String templete = "HH:MM:SS  TT.ttC|D MM/DD   HH.hh%";

int templeteCol[] = {1, 4, 7, 11, 1, 3, 6, 11};
int templeteRow[] = {1, 1, 1, 1, 2, 2, 2, 2};
String templete1 = "HH:MM:SS  TEXT..|TT.TC  -CH:CM:CS";
int templete1Col[] = {1, 4, 7, 12, 1, 9};
int templete1Row[] = {1, 1, 1, 1, 2, 2};
String input;
String Mediate;
bool next;
int Year;
int Month;
int Date;
int DoW;
int Hour;
int Minute;
int Second;
int aHour;
int aMinute;
int aLast;
int aSnooze;
int cdH;
int cdM;
int cdS;
String templeteV = templete;
int T0;
int deltaT;

void lcdPrintMonth(int column, int row) {
  lcd.setCursor(column - 1, row - 1);
  Dis = Clock.getMonth(Century);
  if (Dis < 10) {
    lcd.print("0");
    lcd.print(Dis);
  } else {
    lcd.print(Clock.getMonth(Century), DEC);
  }
  //lcd.print('/');

}

void lcdPrintDate(int column, int row) {
  lcd.setCursor(column - 1, row - 1);
  Dis = Clock.getDate();
  if (Dis < 10) {
    lcd.print("0");
    lcd.print(Dis);
  } else {
    lcd.print(Clock.getDate(), DEC);
  }
}

void lcdPrintDOW(int column, int row) {
  lcd.setCursor(column - 1, row - 1);
  Dis = Clock.getDoW();

  switch (Dis) {
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
}

void lcdPrintHUM(int column, int row) {
  lcd.setCursor(column - 1, row - 1);
  lcd.print(DHT.humidity);
  //lcd.print('%');
}


void lcdPrintTMP(int column, int row) {
  lcd.setCursor(column - 1, row - 1);
  lcd.print(DHT.temperature);
  //lcd.print('C');
}

bool lcdPrintHour(int column, int row) {
  lcd.setCursor(column - 1, row - 1);
  Dis = Clock.getHour(h12, PM);
  if (Dis == 0) {
    next = true;
  } else {
    next = false;
  }
  if (Dis < 10) {
    lcd.print("0");
    lcd.print(Dis);
  } else {
    lcd.print(Clock.getHour(h12, PM), DEC);
  }

  //lcd.print(':');
  return next;
}
bool lcdPrintMinute(int column, int row) {
  lcd.setCursor(column - 1, row - 1);
  Dis = Clock.getMinute();
  if (Dis == 0) {
    next = true;
  } else {
    next = false;
  }
  if (Dis < 10) {
    lcd.print("0");
    lcd.print(Dis);
  } else {
    lcd.print(Clock.getMinute(), DEC);
  }
  //lcd.print(':');
  return next;
}

bool lcdPrintSecond(int column, int row) {
  lcd.setCursor(column - 1, row - 1);
  Dis = Clock.getSecond();
  if (Dis == 0) {
    next = true;
  } else {
    next = false;
  }
  if (Dis < 10) {
    lcd.print("0");
    lcd.print(Dis);
  } else {
    lcd.print(Dis);
  }
  return next;
}
void getAlarm() {
  if (EEPROM.read(4) == 1) {
    lcd.setCursor(9, 1);
    lcd.print("A");
    aHour = EEPROM.read(0);
    aMinute = EEPROM.read(1);
    aLast = EEPROM.read(2);
    aSnooze = EEPROM.read(3);
  }
}

//Setting


void refresh(String Templete) {
  if (Templete == "default") {
    String temp;

    temp = templete.substring(0, 16);
    lcd.setCursor(0, 0);
    lcd.print(temp);
    temp = templete.substring(17, 33);
    lcd.setCursor(0, 1);
    lcd.print(temp);
    lcdPrintHour(templeteCol[0], templeteRow[0]);
    lcdPrintMinute(templeteCol[1], templeteRow[1]);
    lcdPrintSecond(templeteCol[2], templeteRow[2]);
    lcdPrintTMP(templeteCol[3], templeteRow[3]);
    lcdPrintDOW(templeteCol[4], templeteRow[4]);
    lcdPrintMonth(templeteCol[5], templeteRow[5]);
    lcdPrintDate(templeteCol[6], templeteRow[6]);
    lcdPrintHUM(templeteCol[7], templeteRow[7]);
  } else if (Templete == "cd") {
    String temp;
    temp = templete1.substring(0, 16);
    lcd.setCursor(0, 0);
    lcd.print(temp);
    temp = templete1.substring(17, 33);
    lcd.setCursor(0, 1);
    lcd.print(temp);

  }

  getAlarm();

}
String getInput(int mili, int second) {
  int i;
  second = second * 10;
  String serialInput = "";
  while (Serial.available() == 0 || (i < second)) {
    delay(100);
    i = i + 1;
  }
  if (Serial.available() == 0) {
    serialInput = "timeout";
    return serialInput;
  }
  while (Serial.available() > 0) {
    serialInput += char(Serial.read());
    delay(mili);
  }
  return serialInput;
}

void flash(int mili, int iter) {
  int i = 1;
  while (i < iter) {
    lcd.noBacklight();
    delay(mili);
    lcd.backlight();
    delay(mili);
    i = i + 1;
  }
}


void setup() {
  //peripheral setup
  Wire.begin();
  Serial.begin(9600);
  pinMode(Alarm, OUTPUT);
  lcd.init();
  lcd.backlight();
  int chk = DHT.read22(DHT22_PIN);
  //loading display templete
  //complete refresh

  refresh("default");

}
void loop() {
  if (Serial.available() > 0) {
    input = getInput(2, 30);
    Serial.println(input);
    if (input == "set time") {
      Serial.println("Time Setting Mode");
      Serial.println("Current RTC Time:");
      Serial.print(Clock.getHour(h12, PM), DEC);
      Serial.print(':');
      Serial.print(Clock.getMinute());
      Serial.print(':');
      Serial.println(Clock.getSecond());

      Serial.print(Clock.getMonth(Century), DEC);
      Serial.print('/');
      Serial.print(Clock.getDate(), DEC);
      Serial.print('/');
      Serial.print(Clock.getYear());
      Serial.print(" DOW:" );
      Serial.println(Clock.getDoW());
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Setting Time...");
      Serial.println("Time Setting Format:");
      Serial.println("yyyy/mm/dd/dow/hh/mm/ss");

      

      input = getInput(2, 30);
      if (input.length() >= 15) {

        Serial.print("New Date:");
        Serial.println(input.substring(0, 8));
        Serial.print("New DOW:");
        Serial.println(input.substring(8, 9));
        Serial.print("New Time:");
        Serial.println(input.substring(9, 16));
        Serial.println("Are you sure? 0/1)");

        Mediate = getInput(2, 30);
        if (Mediate != "0") {


          Mediate = input.substring(2, 4);
          Year = Mediate.toInt();
          Clock.setYear(Year);

          Mediate = input.substring(4, 6);
          Month = Mediate.toInt();
          Clock.setMonth(Month);

          Mediate = input.substring(6, 8);
          Date = Mediate.toInt();
          Clock.setDate(Date);

          Mediate = input.substring(8, 9);
          DoW = Mediate.toInt();
          Clock.setDoW(DoW);

          Mediate = input.substring(9, 11);
          Hour = Mediate.toInt();
          Clock.setHour(Hour);

          Mediate = input.substring(11, 13);
          Minute = Mediate.toInt();
          Clock.setMinute(Minute);

          Mediate = input.substring(13, 15);
          Second = Mediate.toInt();
          Clock.setSecond(Second);
        }
       else if (Mediate == "0"){
        Serial.println("No changing");
        }
      }
      else {
        Serial.print("Check your formatt!");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Check your formatt!");
        flash(500, 3);


      }
    }
    else if (input == "alarm") {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Setting Alarm");
      Serial.println("Alarm Setting");
      Serial.println("Toggle alarm 0/1");
      input = getInput(2, 30);
      if (input != "0") {
        EEPROM.update(4, 1);
        aHour = EEPROM.read(0);
        aMinute = EEPROM.read(1);
        aLast = EEPROM.read(2);
        aSnooze = EEPROM.read(3);
        Serial.print(aHour);
        Serial.print(':');
        Serial.print(aMinute);
        Serial.print(" ");
        Serial.print(aLast);
        Serial.print("min S");
        Serial.print(aSnooze);
        Serial.print("Format:");
        Serial.println("HHMMLLSS");

        input = getInput(2, 30);
        if (input != "timeout") {
          Serial.print("New Date:");
          Serial.println(input.substring(0, 9));
          Serial.print("New DOW:");
          Serial.println(input.substring(9, 10));
          Serial.print("New Time:");
          Serial.println(input.substring(10, 16));
          Serial.println("Are you sure? 0/1)");

          if (input != "0" || input != "timeout") {
            Mediate = input.substring(0, 2);
            EEPROM.update(0, Mediate.toInt());
            Mediate = input.substring(2, 4);
            EEPROM.update(1, Mediate.toInt());
            Mediate = input.substring(4, 6);
            EEPROM.update(2, Mediate.toInt());
            Mediate = input.substring(6, 8);
            EEPROM.update(3, Mediate.toInt());
          }
        }
        else if (input == "0") {
          EEPROM.update(4, 0);
          Serial.print("Alarm Disabled");
        }
      }


    } else {
      Serial.println("wrong command");

    }
    lcd.clear();

    refresh("default");
  }
  //time keeping
    next = lcdPrintSecond(templeteCol[2], templeteRow[2]);

    if(next){
      next = lcdPrintMinute(templeteCol[1], templeteRow[1]);
         int chk = DHT.read22(DHT22_PIN);
          lcdPrintTMP(templeteCol[3], templeteRow[3]);
          lcdPrintHUM(templeteCol[7], templeteRow[7]);
      if(next){
        next = lcdPrintHour(templeteCol[0], templeteRow[0]);
        if(next){
          lcdPrintDOW(templeteCol[4], templeteRow[4]);
          lcdPrintMonth(templeteCol[5], templeteRow[5]);
          lcdPrintDate(templeteCol[6], templeteRow[6]);
        }
      }
    }
    delay(1000);
}


