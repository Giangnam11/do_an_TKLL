#include <Wire.h> 
//#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trig1 = 8;
const int echo1 = 7;
const int trig2 = A2;
const int echo2 = A1;
const int relay = 13;   // chân máy bơm

int moc_tren = 14;  // Thay đổi theo thực tế
int moc_duoi = 4;  // Thay đổi theo thực tế
int moc_tren_nguon = 14;  
int moc_duoi_nguon = 12;
int relay_status = 0;  
unsigned long milli, milli2;

int count_1 =0;
int count = 0;
int alarm_flag = 0;
int sec_default = 5;
int btnPIN[7] = {53,51,49,47,45,43,41};
int btn[7];
int d1 ,d2;

int hour = 0;
int minute = 0;
int sec = sec_default;

//...............................................
void setup()
{
  Serial.begin(9600);
//  lcd.init();
//  lcd.backlight();
  
 pinMode(trig1, OUTPUT);
 pinMode(echo1, INPUT);
 pinMode(trig2, OUTPUT);
 pinMode(echo2, INPUT);
 pinMode(relay,OUTPUT);
digitalWrite (relay, HIGH);

  for (int i = 0; i < 7; i++)        pinMode(btnPIN[i], INPUT_PULLUP);
  for (int i = 0; i < 7; i++) btn[i] = 0;
  milli = milli2 = millis(); 
//  

}
//...............................................
void loop()
{
 unsigned long duration1;
 unsigned long duration2;
 int distance1;
 int distance2;

// do cao binh chua 
 digitalWrite(trig1, 0);
 delayMicroseconds(2);
 digitalWrite(trig1, 1);
 delayMicroseconds(5);
 digitalWrite(trig1, 0);
 duration1 = pulseIn(echo1, HIGH);
 distance1 = int(duration1 / 2 / 29.412);

// do cao nguon nuoc
 digitalWrite(trig2, 0);
 delayMicroseconds(2);
 digitalWrite(trig2, 1);
 delayMicroseconds(5);
 digitalWrite(trig2, 0);
 duration2 = pulseIn(echo2, HIGH);
 distance2 = int(duration2 / 2 / 29.412);

// man hinh lcd dong 1

 Serial.print("nguon : ");
 Serial.print(distance2 );
 Serial.print("cm \t");
 
 Serial.print("be chua : ");
 Serial.print(distance1 );
 Serial.print("cm \t");
if((distance1 == d1 || distance1 == d1 - 1 || distance1 == d1 + 1) && (distance2 == d2 || distance2 == d2 - 1 || distance2 == d2 + 1))
{
  if (distance1 > moc_tren && distance2 <= moc_tren_nguon && count ==0) // nước bể hết, nước nguồn vẫn trong mức bơm được
     {
//      Serial.print("nc be het nc nguon con \t ");
       digitalWrite (relay, LOW);    
      count =1;
     }
     else if (distance2 > moc_tren_nguon) //nước nguồn hết
     {
//      Serial.print("nc nguon het \t");
       count_1 = 1;
       digitalWrite (relay, HIGH);
       relay_status = 0;
     }
      if(distance1 > moc_tren && count_1==1){ // nước bể hết, yêu cầu bơm, nguồn hết nước
//        Serial.print("nc be het \t ");
         if ( sec > 0 && (millis() - milli) > 1000) //dem nguoc 
        {
         milli = millis();
          sec -= 1;
          if(sec == 0){
          relay_status =1;
          sec= sec_default;
          count_1 =0;
          }
        }
       }
  
    else if (relay_status == 1 && distance2 <= moc_duoi_nguon) //đếm thời gian xong, nước nguồn đầy
     {
//      Serial.print("dem thoi gian xong \t");
       digitalWrite (relay, LOW);  
       relay_status = 0;
     }
    
   else if (distance1 < moc_duoi) // nước bể đầy
     {
//      Serial.print("nc be day \t ");
      digitalWrite (relay,HIGH);
      count =0; 
     }
}
  hour = sec/3600;
  minute = (sec%3600)/60;
  int sec1 = sec%3600%60;



Serial.print(hour );
 Serial.print("h \t");
 Serial.print(minute );
 Serial.print("m \t");
 Serial.print(sec1 );
 Serial.print("s \n");

// hen gio
  if (millis() - milli2 > 300) //Cứ sau mỗi 300 mili giây là đọc tất cả giá trị button
  { 
    
     for (int i = 0; i < 7; i++)      btn[i] = digitalRead(btnPIN[i]);
     
     
     if (!btn[2] && count_1 ==1) // cong 1 tieng
     {
        sec = min(99*3600-1, sec + 3600);
        Serial.print("btn[2] : 49 \n");
     }
     
     else if (!btn[3] && count_1 ==1) // cong 1 phut
     {
        sec = min(60*60-1, sec + 60);
        Serial.print("btn[3] : 47 \n");
     }
     
     else if (!btn[4] && count_1 ==1) // tru 1 tieng
     {
        sec = max(5, sec - 3600);
        Serial.print("btn[4] : 45 \n");
     }
     
     else if (!btn[5] && count_1 ==1) // tru 1 phut
     {
        sec = max(5, sec - 60);
        Serial.print("btn[5] : 43 \n");
     }
     
     milli2 = millis(); 
  }
  d1 = distance1;
  d2 = distance2;
//lcd.setCursor(0, 0);
// lcd.print("D1:");
// lcd.setCursor(5, 0);
//  lcd.print("cm");
//
//lcd.setCursor(8, 0);
// lcd.print("D2:");
// lcd.setCursor(13, 0);
//  lcd.print("cm");
//
//    lcd.setCursor(0, 1);
//  lcd.print("Time:");
//  
//lcd.setCursor(3, 0);
// lcd.print(distance2);
// if (distance1 < 10) {
//   lcd.setCursor(4, 0);
//   lcd.print(" ");
// }
//
//lcd.setCursor(11, 0);
// lcd.print(distance1);
// if (distance2 < 10) {
//   lcd.setCursor(12, 0);
//   lcd.print(" ");
// }
//
//  // man hinh lcd dong 2
//    lcd.setCursor(6, 1);
//    lcd.print(hour);
//    lcd.print("h");
//    lcd.setCursor(9, 1);
//    lcd.print(minute);
//    lcd.print("m");
//    lcd.setCursor(12, 1);
//    lcd.print(sec1);
//    lcd.print("s");
//  delay(1000);
//  lcd.clear();
}
