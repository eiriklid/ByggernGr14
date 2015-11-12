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
bool game_over = 0;
int lives = 3;
unsigned long start_time;
#define Bounce_delay 500

//--- Motor------------
#define DIR_pin A3
#define MOTOR_ENABLE A4
int dir_val = 0;

//----Encoder------------

unsigned int msb = 0;
unsigned int lsb = 0;
int16_t encoder_data = 0;

//-----Solenoid-----------
#define SOLENOID_pin A1

//----PI(D)- regulator-------

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
  encoder_data = 0;
  /*
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  
  SPI.transfer(0x00);
  int j = SPI.transfer(0xAA);
  Serial.print(j,HEX);
  
  SPI.endTransaction();
  */
  
  /*
  mcp2515_reset();
  mcp2515_write(0x36, 0x01);
  int i = mcp2515_read(0x66);
  Serial.print(i);
  */
  
  //Test for CAN - send & receive msg
  
      //**** CAN - Send
  can_message_t can_msg;
  
  can_msg.id = 155;
  can_msg.len = 3;
  can_msg.data[0] = 25;
  can_msg.data[1] = 70;  
  can_msg.data[2] = 9;

  CAN_message_send(&can_msg);

      //****  CAN - Send - End
  
 
  
      //**** CAN - Receive
  if(digitalRead(RX0BF_pin) == 0){
    
    can_receive= CAN_data_receive();
  }
  
  
  
  //----- Print Can message received-----
  /*
  Serial.print("\nID:\t ");
  Serial.print(can_receive.id);

  Serial.print("\nLenght:\t  ");
  Serial.print(can_receive.len);
  int i;
  Serial.print("\nData: \n");
  
  for(i = 0 ; i <  can_receive.len ; i++){
    Serial.print(can_receive.data[i]);
    Serial.print("\n");
    
  }
    

      //**** CAN - Receive - END
  */
  
  /*
  Serial.print("\n\n");
  Serial.println(mcp2515_read(MCP_CANINTF), BIN);
  */

  myservo.write( map(can_receive.data[1], 0, 255, 0, 180) ); 

  IR_val = analogRead(IR_pin);    // read the input pin
  //Serial.println(IR_val);
  
  if( ( (IR_val) < 20) && !(game_over) && (millis() > (start_time + Bounce_delay) ) ){   
    lives--;
    //Serial.println(lives);
    start_time = millis();
    
  }
  else{
    if (!(lives)){
      game_over = 1;
     Serial.print("Game Over! \t");
      
      }
    }

  //-----Motor-----------
  //Burde legge inn buffer som konstanter
  /*
  if( (can_receive.data[0]< 127) || (can_receive.data[0] > 131) ){
    if(can_receive.data[0] > 131){
        dir_val =( (can_receive.data[0]-130));  //Trekker fra offset og multipliserer med 2
        
        Serial.print("\t Hoyre: \t"); 
        Serial.println(dir_val);
        
        
        digitalWrite(DIR_pin, HIGH);
        digitalWrite(MOTOR_ENABLE, HIGH);

        Wire.beginTransmission(0b0101000);
        Wire.write(byte(0x00));
        Wire.write(dir_val);
        Wire.endTransmission();
      }
      else{
        dir_val =( (0x7F - can_receive.data[0]));  //Inverterer verdi og multipliserer med 2
        
        Serial.print("\t Venstre: \t"); 
        Serial.println(dir_val);
        
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
*/    
  //-----Encoder--------------
  digitalWrite(A6, HIGH); // !Reset encoder
  digitalWrite(A7, LOW); // !OE low
  digitalWrite(A5, LOW); // get high byte
  
  delayMicroseconds(20); 
  msb = PINK ;
  
  digitalWrite(A5, HIGH); // get low byte
  delayMicroseconds(20); //is this microseconds?
  lsb = PINK;
  
  for(i = 0; i <8; i++){
    
    if(msb & (1 << i)){
       
      encoder_data += (1 << (15-i));
    }
    if(lsb & (1 << i)){
      
      encoder_data += (1 << (7-i));
    }
  }
  
  digitalWrite(A7, HIGH); // !OE high
  //Serial.println(encoder_data);

  //-----Solenoid-----------
  
  if(can_receive.data[2]){
    digitalWrite(SOLENOID_pin, LOW);
    
  }
  else{
    digitalWrite(SOLENOID_pin, HIGH);
  }
  
  
  
  

  //----PI(D)- regulator-------
  
  
 
  current_time = millis();
  time_difference = current_time -last_sample_time;
  if(time_difference >= sample_time){
    Serial.print(time_difference);
    error = can_receive.data[5] - map( encoder_data, 0, 10000, 255, 0);
    
    if(time_difference > 200){    //for å fjerne feil tid på første måling 
      Serial.println("TIME FAIL!!!!!!!!");
       
    }
    else{
    error_integral += (error*time_difference);
    d_error = (float)(error - last_error)/time_difference;
    pid_val = K_p* error + K_i* error_integral + (int16_t)(K_d*d_error) ;    
    }
    
    last_error = error;
    last_sample_time = current_time;
  }
  
/*
  Serial.print("Encoder:\t");
  Serial.print(encoder_data);
  Serial.print("\t Encoder_map:\t");
  Serial.print(map( encoder_data, 0, 10000, 255, 0));
  */
  
  Serial.print("\tRef: \t");
  Serial.print(can_receive.data[5]);
  Serial.print("\terror: \t");
  Serial.print(error);
  Serial.print("\tK_P error: \t");
  Serial.print(K_p*error);
  Serial.print("\tIntegral: \t");
  Serial.print(K_i*error_integral);
  Serial.print("\tDerivative: \t");
  Serial.print(K_d*d_error,6);
  Serial.print("\t Output: ");
  Serial.println(pid_val);
  
  
  if(time_difference < 200){
    if(pid_val < 0){
        digitalWrite(DIR_pin, LOW);
        digitalWrite(MOTOR_ENABLE, HIGH);

        Wire.beginTransmission(0b0101000);
        Wire.write( byte(0x00));
        Wire.write( -pid_val);
        Wire.endTransmission();
  }
  else{
        digitalWrite(DIR_pin, HIGH);
        digitalWrite(MOTOR_ENABLE, HIGH);

        Wire.beginTransmission(0b0101000);
        Wire.write(byte(0x00));
        Wire.write( pid_val );
        Wire.endTransmission();
  }
  }
  
  

  

}
