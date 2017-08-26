#include <Wire.h>
#include <FaBo3Axis_ADXL345.h>
#define switchPin 2

//モップを止めてから、計測を終えるまでの時間
//計測を終えるまでに再度モップが動き始めたら、同じ計測の中に含める
#define TIME_LAG_AFTER_MOP_STOP 20

//モップが動いている間、カウントアップして、その計測で
//どれだけ長くモップを動かしているか表す
int duration_of_mop_moving = 0;

//TIME_LAG_AFER_MOP_STOPのカウントダウンタイマー
int time_lag_after_mop_stop = TIME_LAG_AFTER_MOP_STOP; 

FaBo3Axis fabo3axis;

void setup() {
  Serial.begin(9600);
  Serial.println("SETUP");
  if(fabo3axis.searchDevice()){
    Serial.println("Find ADX1345");
  }
  fabo3axis.configuration();
  fabo3axis.powerOn();

  pinMode(switchPin,INPUT);
}

void loop() {
  
  //スイッチの状態
 int switchState = 0;
 //加速度センサーの値
 int x;
 int y;
 int z;
 fabo3axis.readXYZ(&x,&y,&z);

 //モップが動いているか判定
 //加速度センサーは、傾けるだけで重力の影響で反応してしまう。
 //ある程度アソビを作っておく
 if ( abs(x+5) > 100 || abs(y+1) > 100 || abs(z-31) > 100) {
   /*Serial.print("x: ");
   Serial.print(x+5);
   Serial.print("y: ");
   Serial.print(y+1);
   Serial.print("z: ");
   Serial.println(z-31);
   */duration_of_mop_moving++;
   time_lag_after_mop_stop = TIME_LAG_AFTER_MOP_STOP;  
 } else {
    time_lag_after_mop_stop--;
 }

//モップの動きがとまったことを判定し、ログにはきだす。
 if ( duration_of_mop_moving > 0 && time_lag_after_mop_stop == 0 ) {
   switchState = digitalRead(switchPin);
   if ( switchState == HIGH ) {
     Serial.print("\"H\",");
   } else {
     Serial.print("\"W\",");
   }
   Serial.println(duration_of_mop_moving);
   duration_of_mop_moving=0;
   time_lag_after_mop_stop = TIME_LAG_AFTER_MOP_STOP;
 }
 
 delay(100);
}
