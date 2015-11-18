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
#define Bounce_delay 1000
uint16_t highscore_time = 0;
uint16_t game_start_time = 0;
uint8_t device = 0;
//--- Motor------------
#define DIR_pin A3
#define MOTOR_ENABLE A4
#define MOTOR_UPPER_TRESHOLD 131
#define MOTOR_LOWER_TRESHOLD 127

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

#define SAMPLE_TIME 20
#define TIME_TRESHOLD 200
uint32_t current_time = 0;
uint32_t last_sample_time = 0;
uint32_t time_difference = 0;

//---- HC-SR04 (Other Arduino) ----------
float K_p_u = 0.8;
float K_i_u = 0.002;
float K_d_u = 3;

uint8_t distance[4];
uint8_t index = 0;
uint32_t sum_distance;



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
  
      //**** CAN - Receive
  if(digitalRead(RX0BF_pin) == 0){
   
    can_receive= CAN_data_receive();
    
    if (can_receive.id == 1){
      
      game_over = 0;
      lives = 3;
      device = can_receive.data[0];
      Serial.println("Starting Game!");
      game_start_time = millis();
      
      can_message_t start_msg;
      start_msg.id = 5;
      start_msg.len = 3;
      start_msg.data[0] = lives;
      start_msg.data[1] = 0;
      start_msg.data[2] = 0;
      CAN_message_send(&start_msg);
    }
    
  }

  //------ In Game------------
 if(!game_over){
  myservo.write( map(can_receive.data[1], 0, 255, 0, 180) ); 
  
  IR_val = analogRead(IR_pin);    // read the input pin
  //Serial.println(IR_val);
  
  
  //-------- IR Detection--------------
  if( ( (IR_val) < 20) && (millis() > (start_time + Bounce_delay) ) ){   
    lives--;
    Serial.println(lives);
    start_time = millis();
    can_message_t can_msg;
  
    can_msg.id = 3;
    can_msg.len = 3;
    highscore_time = (millis()-game_start_time)/100;
    Serial.println(highscore_time);
    can_msg.data[0]= lives;
    can_msg.data[1] = (highscore_time & 0xFF);           //low_highscore
    can_msg.data[2] = ((highscore_time >> 8) & 0xFF);    //high_highscore
    CAN_message_send(&can_msg);
  }
  else{
    if (!(lives)){
      
      game_over = 1;
      highscore_time = (millis()-game_start_time)/100;
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
  if(device == 1){
    
    
    if( (can_receive.data[2]< MOTOR_LOWER_TRESHOLD) || (can_receive.data[2] > MOTOR_UPPER_TRESHOLD) ){
      if(can_receive.data[2] > MOTOR_UPPER_TRESHOLD){
          dir_val =( (can_receive.data[2]-MOTOR_UPPER_TRESHOLD));  //Subtracting offset and multiply by 2
          
          digitalWrite(DIR_pin, HIGH);
          digitalWrite(MOTOR_ENABLE, HIGH);
  
          Wire.beginTransmission(0b0101000);
          Wire.write(byte(0x00));
          Wire.write(dir_val);
          Wire.endTransmission();
        }
        else{
          dir_val =( (MOTOR_LOWER_TRESHOLD - can_receive.data[2]));  //Inverterer verdi og multipliserer med 2
          
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
      
  }
  if((device==2) ||  (device==3) ){
	//-----Encoder--------------
  
    digitalWrite(A6, HIGH); // !Reset encoder
    digitalWrite(A7, LOW); // !OE low
    digitalWrite(A5, LOW); // get high byte
    
    delayMicroseconds(20); 
    msb = PINK ;
    
    digitalWrite(A5, HIGH); // get low byte
    delayMicroseconds(20); 
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
    Serial.println(encoder_data);
	  
  }
  
  if(device == 2){
      
    

    //---- PID-regulator -------
    
    
   
    current_time = millis();
    time_difference = current_time -last_sample_time;
    if(time_difference >= SAMPLE_TIME){
      Serial.print(time_difference);
      error = can_receive.data[2] - map( encoder_data, 0, 10000, 255, 0);
      
      if(time_difference > TIME_TRESHOLD){    //To make sure the loop is runing properly 
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
    
    if(time_difference < TIME_TRESHOLD){
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

  //-----Ultrasonic
    if(device == 3){
      //---- HC-SR04 (Other Arduino) ----------
      Wire.requestFrom(1,4); //request 4 bytes from slave device #1
  
        while(Wire.available()){
        distance[index++] = Wire.read();
        
        if(index >= 4){
          index =0;
        }
        
        sum_distance = distance[3] + (distance[2] << 8) + (distance[1] << 16) + (distance[0] << 24);
        //Serial.println(sum_distance);
        }
    

    //---- PID-regulator -------
    
    
   
    current_time = millis();
    time_difference = current_time -last_sample_time;
    if(time_difference >= SAMPLE_TIME){
      //Serial.println(time_difference);
      if(sum_distance < 3000){
        error = map(sum_distance,300,2300,0,255) - map( encoder_data, 0, 10000, 0, 255);
        
        if(time_difference > TIME_TRESHOLD){    //for å fjerne feil tid på første måling 
          Serial.println("TIME FAIL!!!!!!!!");
           
        }
        else{
        error_integral += (error*time_difference);
        d_error = (float)(error - last_error)/time_difference;
        pid_val = K_p_u* error + K_i_u* error_integral + (int16_t)(K_d_u*d_error) ;    
        }
        
        last_error = error;
        last_sample_time = current_time;
      }
      else {
        Serial.println("Measurment ERROR");
      }
       
    }
    
  /*
    Serial.print("Encoder:\t");
    Serial.print(encoder_data);
    Serial.print("\t Encoder_map:\t");
    Serial.print(map( encoder_data, 0, 10000, 255, 0));
    */
    /*
    Serial.print("\tRef: \t");
    Serial.print(sum_distance);
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
    */
    
    if(time_difference < TIME_TRESHOLD){
      if(pid_val > 0){
          digitalWrite(DIR_pin, LOW);
          digitalWrite(MOTOR_ENABLE, HIGH);
  
          Wire.beginTransmission(0b0101000);
          Wire.write( byte(0x00));
          Wire.write( pid_val);
          Wire.endTransmission();
    }
    else{
          digitalWrite(DIR_pin, HIGH);
          digitalWrite(MOTOR_ENABLE, HIGH);
  
          Wire.beginTransmission(0b0101000);
          Wire.write(byte(0x00));
          Wire.write( -pid_val );
          Wire.endTransmission();
    }
  }
  
  }
  //-----Solenoid-----------
  
  if(can_receive.data[0]){
    digitalWrite(SOLENOID_pin, LOW);
    
  }
  else{
    digitalWrite(SOLENOID_pin, HIGH);
  }
  
 } 

 
  
  

}
