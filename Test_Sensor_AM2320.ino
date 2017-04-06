//Sensor
#include <AM2320.h>
#include <Wire.h>
AM2320 th;
//////////////////SD Card///////////////////
#include <SD.h>
File myFile;
const int chipSelect=10;
int p=0;
long int data1=0;
long int data2=0;
long int data3=0;
// Setup REC //////////////////////////
int buttonUp=5;
int buttonDown=11;
int buttonLeft=12;
int buttonRight=13;
int buttonU=0;
int buttonD=0;
int buttonL=0;
int buttonR=0;
int buttonS=0;
int THour=0;
//long SecREC=0;
//////////////////////////////////

int realminSD=0; //เก็บค่าเวลาเริ่ม Start
int realmin;
int Start;
int SecREC=15;
int sumtime;
int starttime=30;
int bts=5;
int button;

///////////////////////////////////////
// Time
#include <DS1302.h>
DS1302 rtc(8, 7, 6);
int count=6 ;
int nn=0 ;
Time t;
//LCD
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);
byte deGree[] = {
  B00010,
  B00101,
  B00010,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

//ประกาศตัวแปร
int Setup = 0 ;

void setup() {
lcd.begin();
Serial.begin(9600);
pinMode(buttonUp, INPUT); 
pinMode(buttonDown, INPUT); 
pinMode(buttonLeft, INPUT); 
pinMode(buttonRight, INPUT);
lcd.createChar(0, deGree);
//  Serial.begin(9600);
  //Time
rtc.halt(false);
rtc.writeProtect(false);
// Write label Excel
  myFile = SD.open("datalog.csv", FILE_WRITE);
  if(myFile)
  {
    myFile.print("Item");
    myFile.print(",");
    myFile.print("Time");
    myFile.print(",");
    myFile.print("Date");
    myFile.print(",");
    myFile.println("Humidity");
    myFile.print(",");
    myFile.println("Temperature ");
    myFile.close();
    
  }
 checkSD();
}
void loop(){
 
  while(Setup==0)
   {   
     t = rtc.getTime();
     realmin = t.sec;
       if(realmin<=60)
        {
          sumtime = realmin + SecREC;
          if(sumtime>60){sumtime = sumtime-60;}  
          if(sumtime==60){sumtime = sumtime-59;}
        }  
       buttonS = digitalRead(buttonLeft);
       th.Read();
       lcd.setCursor(0,0);
       lcd.print("H:");
       lcd.print(th.h);
       lcd.setCursor(6,0);
       lcd.print("%");
       lcd.setCursor(7,0);
       lcd.print(" T:");
       lcd.setCursor(10,0);
       lcd.print(th.t);
       lcd.setCursor(14,0);
       lcd.write(0);
       lcd.print("C");
       lcd.setCursor(0,1);
       lcd.print(" Time: ");
       lcd.print(rtc.getTimeStr());
   // sensor();
      if (buttonS==HIGH)
      {        
        Setup = 1;       
      }
}
while(Setup==1)
   {    
        lcd.setCursor(0,0);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print("                ");
        Progarm();       
    }    
}

/*void sensor()
{
//อ่านค่า AM2320 และเวลา ส่งออกแสดงบนจอ LCD
for (int i = 0; i <= 5; i++){
   switch(th.Read()){
     case 2:
       lcd.clear();
       delay (200);
       lcd.setCursor(4 ,0);
       lcd.print("CRC Filed");
       break;
     case 1:
       lcd.clear();
       delay (200);
       lcd.setCursor(1,0);
       lcd.print("Sensor Offline");
       lcd.setCursor(5,1);
       lcd.print("!!!!!!");
       delay (500);
       break;
     case 0:
       lcd.setCursor(0,0);
       lcd.print("H:");
       lcd.print(th.h);
       lcd.setCursor(6,0);
       lcd.print("%");
       lcd.setCursor(7,0);
       lcd.print(" T:");
       lcd.setCursor(10,0);
       lcd.print(th.t);
       lcd.setCursor(14,0);
       lcd.write(0);
       lcd.print("C");
       lcd.setCursor(0,1);
       lcd.print(" Time: ");
       lcd.print(rtc.getTimeStr());
       delay (100);
       break;  
       }     

    }
}*/

/*void datenow()
//แสดงเวลาวันที่
{
for (int i = 0; i <= 5; i++){
       lcd.setCursor(1,0);
       lcd.print(rtc.getDOWStr());
       lcd.setCursor(5,0);
       lcd.print(rtc.getDateStr());
       lcd.setCursor(1,1);
       lcd.print("Time:");
       lcd.setCursor(7,1);
       lcd.print(rtc.getTimeStr());
       delay (1000);                     
       }
       lcd.clear();
       delay (200);
}*/

void Progarm()
//Setup Mode
{
//Setup--;//ให้ตัวแปร Setup = 0
THour=0;
int proOut=0;
  while(proOut==0)
  {
    buttonU = digitalRead(buttonUp);
    buttonD = digitalRead(buttonDown);
    buttonL = digitalRead(buttonLeft);
    buttonR = digitalRead(buttonRight);
    lcd.setCursor(2,0);
    lcd.print("Setup Record");
    lcd.setCursor(3,1);
    if (SecREC < 100) lcd.print(" ");
    lcd.print(SecREC);
    lcd.setCursor(6,1);
    lcd.print("  m/REC");   
      if(buttonU==HIGH)
      {
        delay(200);
        THour ++;
           if(THour>=4)
           {
           THour=4;
           } 
        SecREC = 15*THour ;        
      }
      if(buttonD==HIGH)
      {
        delay(200);
        THour --;
            if(THour<=0)
            {
            THour=0;
            }
        SecREC = 15*THour;
      }
      if(buttonR==HIGH)
      {
        lcd.clear();
        delay(100);
        lcd.setCursor(5,0);
        lcd.print("Cancel");  
        delay(3000);
        proOut++;
        Setup=0;        
      }
      if(buttonL==HIGH)
      {
            for (int i = 0; i <= 3; i++)
            {  
              lcd.clear();
              delay(100);
              lcd.setCursor(3,0);
              lcd.print("Start  REC");
              delay(1000);
            }
            Start = sumtime;
        Serial.println("Save Now!!");
        writeSD();
        delay(200);
        REC();
        proOut++;
        //Do It ( Start REC )
        
      }
      
  }
}

void REC()
{ 
  int i=0;
  while(i==0){
  t = rtc.getTime();
  realmin = t.sec;
  realminSD=1;
        if(realmin<=60)
              {
                sumtime = realmin + SecREC;
                if(sumtime==60){sumtime = 0;}
                if(sumtime>60){sumtime = sumtime-60;}  
              }
              th.Read();
       lcd.setCursor(0,0);
       lcd.print("H:");
       lcd.print(th.h);
       lcd.setCursor(6,0);
       lcd.print("%");
       lcd.setCursor(7,0);
       lcd.print(" T:");
       lcd.setCursor(10,0);
       lcd.print(th.t);
       lcd.setCursor(14,0);
       lcd.write(0);
       lcd.print("C");
       lcd.setCursor(0,1);
       lcd.print("[");
       lcd.setCursor(1,1);
       lcd.print("Time: ");
       lcd.print(rtc.getTimeStr());
       lcd.setCursor(15,1);
       lcd.print("]");
        Serial.print("Setup min/REC: ");
        Serial.print(SecREC);
        Serial.print(" Starttime: ");
        Serial.print(Start);
        Serial.print(" Real: ");
        Serial.print(realmin);
        Serial.print(" Sum: ");
        Serial.println(sumtime);

              if(Start==realmin)
              {
              p++;
              Serial.println("Save Now!!");
              Start=sumtime;
              writeSD();
              }
              buttonR = digitalRead(buttonRight);
               if(buttonR==HIGH)
              {
                lcd.clear();
                delay(100);
                lcd.setCursor(5,0);
                lcd.print("Stop REC");  
                delay(3000);
                i++;                     
              }
     Setup=0; 
  }
}




void checkSD()//Check SD Card
{
  for (int s = 0; s <= 2; s++)
     {
     lcd.clear();
     delay(200);
        if (!SD.begin(chipSelect)) 
        {
          // don't do anything more:
           lcd.setCursor(3,0);
           lcd.print("SD Card OK");
          Serial.println("Check SD Card OK!!!!");
          
        }
     delay(500);
     }
     lcd.clear();
     delay(200);
     return;
}

void writeSD()// เขียนค่าลง SD การ์ด
{
  lcd.clear();
  delay(200);
  lcd.setCursor(1,0);
  lcd.print("REC To SD Card");
  delay(1000);
  Serial.println("Memory!!!!");
  myFile = SD.open("datalog.csv", FILE_WRITE);
  if(myFile)
  {
    myFile.print(p);
    myFile.print(",");
    myFile.print(rtc.getTimeStr());
    myFile.print(",");
    myFile.print(rtc.getDateStr());
    myFile.print(",");
    myFile.println(th.t);
    myFile.print(",");
    myFile.println(th.h);
    myFile.close();
    
  }
}

