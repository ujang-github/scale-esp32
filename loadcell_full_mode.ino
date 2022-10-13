#include <EEPROM.h>

#define DT 32
#define SCK 33

#define button_zero 36
#define button_calibrate 39
bool status_button_zero ;
bool status_button_calibrate; 
int sample2 ;
int val2;
int w;
long sample=0;
float val=0;
long count=0;

unsigned long  waktuAkhir = 0 ;
unsigned long interval= 500;
float berat = 0.000;
String calString;
unsigned long readCount(void)
      {
            unsigned long Count;
            unsigned char i;
            pinMode(DT, OUTPUT);
            digitalWrite(DT,HIGH);
            digitalWrite(SCK,LOW);
            Count=0;
            pinMode(DT, INPUT);
            while(digitalRead(DT));
            for (i=0;i<24;i++)
      {
digitalWrite(SCK,HIGH);
Count=Count<<1;
digitalWrite(SCK,LOW);
if(digitalRead(DT))
Count++;
}
digitalWrite(SCK,HIGH);
Count=Count^0x800000;
digitalWrite(SCK,LOW);
return(Count);




}
const int potPin = 36;
int potValue = 0;
 
void setup()
{
 pinMode (button_zero, INPUT);
 pinMode (button_calibrate, INPUT);
Serial.begin(115200);
pinMode(SCK, OUTPUT);



EEPROM.begin(512);

Serial.print(" Weight ");

Serial.println(" Measurement ");
//EEPROM_put(10, "#8789798,@989!");
       String saveddata;
       saveddata = EEPROM_get(10);
       int ix1=saveddata.indexOf("#");
       int ix2=saveddata.indexOf(",");
       String  pengirim=saveddata.substring(ix1+1,ix2);
      
       int jx3=saveddata.indexOf("@");
       int jx4=saveddata.indexOf("!");
       String  pengirim1=saveddata.substring(jx3+1,jx4);
       
      sample2 =pengirim.toInt();
      val2 = pengirim1.toInt();  
// cali/brate();

}    
 
void loop()
{

  if (Serial.available()){
      char c = Serial.read();
        if (c == 'c'){
           calibrate();
          }else if (c == 'z'){
             for(int i=0;i<10;i++)
              {
                count=readCount();
                sample2=count;
              }
          }
     }


 
// status_button_calibrate = digitalRead(button_calibrate);
// status_button_zero = digitalRead (button_zero);


//  if (status_button_calibrate == 1)
//      {
//       sample=0;
//       count=0;
//       calibrate();
//      }
//
//  if (status_button_zero == 1 ) 
//      {
//          for(int i=0;i<25;i++)
//              {
//              count=readCount();
//              sample2=count;
//              }
//      }
      

count= readCount();    
w=(((count-sample2)/val2)-2*((count-sample2)/val2));
berat = w;
//berat -=;
//  if (berat < 0.005)
//      {
//        berat =0.000;
//      }

//
//berat/=5;
//berat = round(berat);
//berat*=5;
berat = berat/1000.00;


//  if(berat <= 0.010){
//    unsigned long waktuAwal = millis();
//    if(waktuAwal- waktuAkhir >= interval){
//      for(int i=0;i<10;i++){
//         count=readCount();
//         sample2=count;
//         }
//      waktuAkhir = waktuAwal;
//    }
//  }
  
      

Serial.print(sample2); Serial.print("\t");
Serial.print(count); Serial.print("\t");
Serial.print(val2); Serial.print("\t");
Serial.print("Measured Weight");

Serial.print(berat, 3);
Serial.println("g ");

// analogSetPinAttenuation(potPin, ADC_6db);
//   potValue = analogRead(potPin);
//   Serial.println(potValue);

 

}
 
void calibrate()
{
//lcd.clear();
Serial.print("Calibrating...");
//lcd.setCursor(0,1);
Serial.print("Please Wait...");
for(int i=0;i<100;i++)
{
count=readCount();
sample+=count;
Serial.print(i);Serial.print("\t");
Serial.println(sample);
}
sample/=100;

Serial.print("Put 100g & wait");
count=0;
while(count<57000)
{
count=readCount();
Serial.print(count);Serial.print("\t");
count=sample-count;
Serial.println(count);
}

Serial.print("Please Wait....");
delay(2000);
for(int i=0;i<1124;i++)
{
count=readCount();
val+=sample-count;
Serial.println(val);
Serial.println(i);
}
val=val/1124.0;
val=val/1124.0;
String sample1= String (sample);
String val1 = String (val);
String stringOne= "#"+sample1+","+"@"+val1+"!";
 EEPROM_put(10, stringOne);
 stringOne = "";

ESP.restart();
}
