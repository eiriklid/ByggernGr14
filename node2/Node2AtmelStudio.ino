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
#define DO0 A15
#define DO1 A14
#define DO2 A13
#define DO3 A12
#define DO4 A11
#define DO5 A10
#define DO6 A9
#define DO7 A8

unsigned int msb = 0;
unsigned int lsb = 0;
int16_t encoder_data = 0;

//-----Solenoid-----------
#define SOLENOID_pin A1

//----PI(D)- regulator-------

float K_p = 1;
float K_i = 0.05;

int16_t error = 0;
int32_t error_integral = 0;
int16_t pid_val = 0;
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
  
  
  delay(20);
  
      //**** CAN - Receive
  if(digitalRead(RX0BF_pin) == 0){
    Serial.println("CAN");
    
  
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
     // Serial.print("Game Over! \n");
      
      }
    }

  //-----Motor-----------
  //Burde legge inn buffer som konstanter
  /*
  if( (can_receive.data[0]< 127) || (can_receive.data[0] > 131) ){
    if(can_receive.data[0] > 131){
        dir_val =( (can_receive.data[0]-130));  //Trekker fra offset og multipliserer med 2
        
        Serial.print("Hoyre: "); 
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
        /*
        Serial.print("Venstre: "); 
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

  error = can_receive.data[0] - map( encoder_data, 0, 10000, 255, 0);
  error_integral += error;

  pid_val = K_p* error + K_i* error_integral;
  /*
  Serial.print("\n\n Ref: ");
  Serial.println(can_receive.data[0]);
  Serial.print("error: ");
  Serial.println(error);
  Serial.print("Integral: ");
  Serial.println(error_integral);
  Serial.print("Output: ");
  Serial.print(pid_val);
*/
  if(pid_val < 0){
        digitalWrite(DIR_pin, LOW);
        digitalWrite(MOTOR_ENABLE, HIGH);

        Wire.beginTransmission(0b0101000);
        Wire.write(byte(0x00));
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

