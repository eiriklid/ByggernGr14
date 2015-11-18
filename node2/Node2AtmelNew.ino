#include "can.h"
#include "MCP2515.h"

#include "Arduino.h"
#include "SPI.h"
#include "Servo.h"
#include "Wire.h"


Servo myservo;
unsigned int i = 0;

//--- CAN------------
#define RX0BF_pin 2
can_message_t can_receive; 

//--- PWM------------
#define PWM_pin 9
#define PWM_min 900
#define PWM_max 2100

//--- IR------------
#define IR_pin A0
int IR_val = 0;
int val = 0;

//--- Game------------
int score = 0;
bool game_over = 1;
int lives = 3;
unsigned long start_time;     //change name -- bounce_start_time
#define Bounce_delay 500
uint16_t highscore_time = 0;
uint16_t game_start_time = 0;

//--- Motor------------
#define DIR_pin A3
#define MOTOR_ENABLE A4
int dir_val = 0;

//----Encoder------------
/*
unsigned int msb = 0;
unsigned int lsb = 0;
int16_t encoder_data = 0;
*/
//-----Solenoid-----------
#define SOLENOID_pin A1

//----PI(D)- regulator-------
/*
float K_p = 0.7;
float K_i = 0.001;
float K_d = 3;

int16_t error = 0;
int16_t last_error = 0;
int32_t error_integral = 0;
float d_error = 0;
int16_t pid_val = 0;

#define min_out = 70;
#define max_out = 120;

#define sample_time 20
uint32_t current_time = 0;
uint32_t last_sample_time = 0;
uint32_t time_difference = 0;
*/

//---- Game ----------

void setup()
{
  Serial.begin(9600);
  pinMode(RX0BF_pin, INPUT);
  CAN_init();
  Serial.println("Reboot"); 
  Wire.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV64);
  
  
  myservo.attach(PWM_pin,PWM_min, PWM_max); //Maybe check if servo.attached
  myservo.write(90);    //Set servo to mid-point

  pinMode(IR_pin, INPUT);
  pinMode(DIR_pin, OUTPUT);
  pinMode(MOTOR_ENABLE, OUTPUT);
  
  pinMode(A7, OUTPUT); // !OE
  pinMode(A6, OUTPUT); // !RES
  pinMode(A5, OUTPUT); // SEL
  
  //-----Encoder calibration---------
  uint16_t cal_time = millis();
  while( (millis()- cal_time )< 500){
        digitalWrite(DIR_pin, HIGH);
        digitalWrite(MOTOR_ENABLE, HIGH);

        Wire.beginTransmission(0b0101000);
        Wire.write(byte(0x00));
        Wire.write(0xD0);
        Wire.endTransmission();
        
  }
  
  Wire.beginTransmission(0b0101000);
  Wire.write(byte(0x00));
  Wire.write(0x00);
  Wire.endTransmission();       
  
  digitalWrite(A6, LOW); // !Reset encoder
  
  pinMode(SOLENOID_pin, OUTPUT);
  
}

void loop()
{
  //encoder_data = 0
  
      //**** CAN - Receive
  if(digitalRead(RX0BF_pin) == 0){
   
    can_receive= CAN_data_receive();
    //Serial.println(can_receive.id);
    if (can_receive.id == 1){

      game_over = 0;
      lives = 3;
      game_start_time = millis();
      Serial.println("Starting Game!");
      can_message_t start_msg;
      start_msg.id = 5;
      start_msg.len = 3;
      start_msg.data[0] = lives;
      start_msg.data[1] = 0;
      start_msg.data[2] = 0;
      CAN_message_send(&start_msg);
    }
    
  }
 if(!game_over){
  myservo.write( map(can_receive.data[1], 0, 255, 0, 180) ); 

  IR_val = analogRead(IR_pin);    // read the input pin
  //Serial.println(IR_val);
  
  if( ( (IR_val) < 20) && (millis() > (start_time + Bounce_delay) ) ){   
    lives--;
    Serial.println(lives);
    start_time = millis();
    can_message_t can_msg;
  
    can_msg.id = 3;
    can_msg.len = 3;
    highscore_time = millis()-game_start_time;
    Serial.println(highscore_time);
    can_msg.data[0]= lives;
    can_msg.data[1] = (highscore_time & 0xFF);           //low_highscore
    can_msg.data[2] = ((highscore_time >> 8) & 0xFF);    //high_highscore
    CAN_message_send(&can_msg);
  }
  else{
    if (!(lives)){
      
      game_over = 1;
      highscore_time = millis()-game_start_time;
      Serial.println(highscore_time);
      Serial.print("Game Over! \t");
      can_message_t can_msg;
  
      can_msg.id = 2;
      can_msg.len = 3;
      can_msg.data[0] = lives; 
      can_msg.data[1] = (highscore_time & 0xFF);           //low_highscore
      can_msg.data[2] = ((highscore_time >> 8) & 0xFF);    //high_highscore
  
      CAN_message_send(&can_msg);
      }
    }

  //-----Motor-----------
  //Burde legge inn buffer som konstanter
  
  if( (can_receive.data[0]< 127) || (can_receive.data[0] > 131) ){
    if(can_receive.data[0] > 131){
        dir_val =( (can_receive.data[0]-130));  //Trekker fra offset og multipliserer med 2
        
        digitalWrite(DIR_pin, HIGH);
        digitalWrite(MOTOR_ENABLE, HIGH);

        Wire.beginTransmission(0b0101000);
        Wire.write(byte(0x00));
        Wire.write(dir_val);
        Wire.endTransmission();
      }
      else{
        dir_val =( (0x7F - can_receive.data[0]));  //Inverterer verdi og multipliserer med 2
        
        digitalWrite(DIR_pin, LOW);
        digitalWrite(MOTOR_ENABLE, HIGH);

        Wire.beginTransmission(0b0101000);
        Wire.write(byte(0x00));
        Wire.write(dir_val);
        Wire.endTransmission();
        
        }
    }
  else{
    digitalWrite(MOTOR_ENABLE, LOW);
    }  

  //-----Solenoid-----------
  
  if(can_receive.data[2]){
    digitalWrite(SOLENOID_pin, LOW);
    
  }
  else{
    digitalWrite(SOLENOID_pin, HIGH);
  }
  
 }
  
  


  

}
