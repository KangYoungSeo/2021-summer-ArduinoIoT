#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include <IRremote.h>

//LCD 설정
LiquidCrystal_I2C lcd(0x27,16,2);  
 
//적외선 리모컨
IRrecv irrecv(8);  // 적외선 센서 핀
decode_results results; // 적외선 수신번호 저장변수

//온도 센서
int reading;
float temperature = 0;
int lm35Pin = A1;

//인체 감지
bool On_Off = false;
int SensorOut = 7;
int val = 0;
int OFF_NUM = 0;


void setup()
{
    Serial.begin(9600);
    lcd.init();
    irrecv.enableIRIn();  //적외선 센서 활성화
    
    //조명
    pinMode(9,OUTPUT);
    pinMode(5,OUTPUT);  
    pinMode(6,OUTPUT);  
    
    //온도 센서
    analogReference(DEFAULT); 

    //인체 감지
    pinMode(SensorOut,INPUT);
  }
 
void loop()
{
    //온도 측정
    reading = analogRead(lm35Pin);
    temperature = (5.0 * reading) / 1024.0;
    temperature *= 100;
    lcd.backlight();
    lcd.display();
    lcd.print("TEMPERATURE:   ");
    lcd.setCursor(0,1);
    lcd.print(temperature);
    lcd.print(" C ");
 
    Serial.print("temperature: ");
    Serial.print(temperature);
    Serial.print("°C ");
    Serial.println();


    val = digitalRead(SensorOut);
    if(val == HIGH){

      Serial.println(" On "); 
      //조도 센서
      int sensor = analogRead(A0);  //  A0핀에 있는 값을 아날로그 값으로 읽어온다. (조도센서)
                                //  sensor값은 0~1023의 값을 갖는다.
                                //  LED는 0~255까지의 값을 갖는다.

      int led = ((float)sensor / 1023) * 255; 
  
     /* Serial.println(led,HEX);      //  BIN은 binary로, led값을 이진수로 출력
                                //  HEX는 16진수*/
      analogWrite(9,led);
      analogWrite(5,led);
      analogWrite(6,led);
      OFF_NUM=0;
    }
    else 
    {
      Serial.println(" Off "); 
      OFF_NUM++;
      Serial.println(OFF_NUM); 
      
      if(OFF_NUM == 200)
      {
        OFF_NUM = 0;
        analogWrite(9,0);
        analogWrite(5,0);
        analogWrite(6,0);
      }
    }
      
    //사람이 있으면 조명 킴
  /*  if(On_Off)
    {
      //조도 센서
      int sensor = analogRead(A0);  //  A0핀에 있는 값을 아날로그 값으로 읽어온다. (조도센서)
                                //  sensor값은 0~1023의 값을 갖는다.
                                //  LED는 0~255까지의 값을 갖는다.

      int led = ((float)sensor / 1023) * 255; 
 
      analogWrite(9,led);
      analogWrite(5,led);
      analogWrite(6,led);

    }
   
     //인체감지센서 값 입력받음 
  int value = digitalRead(SensorOut);
  //시리얼창에 값 표시
  Serial.println(value);
  //인체감지되면 2번에 전류를 흘려라    
  if(value == HIGH) digitalWrite(9,HIGH);
  //인체감지가 안되면 전류를 끊어라
  else digitalWrite(9,LOW);
  delay(1000);
    */
  delay(100); //10초마다 Refresh
  lcd.clear();
  
}
