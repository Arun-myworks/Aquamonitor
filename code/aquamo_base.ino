#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <EEPROM.h>
#include "TimerOne.h"

#define PIN_TX    9                 /* rx of Arduino (connect tx of gprs to this pin) */
#define PIN_RX    10                 /* tx of Arduino (connect rx of gprs to this pin) */
#define BAUDRATE  9600
#define PHONE_NUMBER "9497485980"
#define MESSAGE_LENGTH 160
#define aerator_1 1
#define aerator_2 2
#define PIN_AERATOR1 2
#define PIN_AERATOR2 3

int currentSense = A0;

struct stat{
            int8_t count ;
            int8_t aerator_status;
            int16_t sense_Current_aerator_1;
            int8_t sense_Current_aerator_2;
            int8_t timer_count;
  };

struct datastruct{
            int16_t time_Data;
            int16_t EEP_base_addr;
            char message[MESSAGE_LENGTH];
            char phone[16];
            char datetime[24];
            int8_t messageIndex ;
  };  

/* Create an object named Sim900_test of the class GPRS */
GPRS aquamo_Sim900(PIN_TX,PIN_RX,BAUDRATE); 
struct stat Status;
struct datastruct datas;



void callback()
{
  Status.timer_count++;
  if(Status.timer_count == 60)
  {
    Status.timer_count = 0;
    if(Status.aerator_status == aerator_1)
    {
      Status.aerator_status = aerator_2;
     
    }
    else
    {
      Status.aerator_status = aerator_1;
      
    }
  }
}
void setup() {
  Timer1.initialize(1000000);         // initialize timer1, and set a 1/2 second period  
  Timer1.attachInterrupt(callback);
  Serial.begin(9600); /* Define baud rate for serial communication */
  pinMode(4, OUTPUT);
  pinMode(PIN_AERATOR1,OUTPUT);
  pinMode(PIN_AERATOR2,OUTPUT);  
  Status.aerator_status= aerator_1;   
  while(!aquamo_Sim900.init())                            /* Sim card and signal check, also check if module connected */ 
  {
      delay(1000);
      Serial.println("SIM900 initialization error");
  }  
  Serial.println("SIM900 initialization success");
//  memset(datas.message, 0, 160);
//  datas.time_Data = EEPROM.read(datas.EEP_base_addr);
//  if( datas.time_Data == 0x00 || datas.time_Data == 0xFF )
//  {
//    aquamo_Sim900.sendSMS( PHONE_NUMBER , "sent_time_data" );  
//  }
//  do
//  {
//    datas.messageIndex = aquamo_Sim900.isSMSunread(); 
//    delay(1000);
//                     /* need to add a led here to indicate waiting for sms */
//  }while(datas.messageIndex > 0 ) ;
//                     /* need to add a led here to indicate LED turn off */
//    aquamo_Sim900.readSMS(datas.messageIndex, datas.message, MESSAGE_LENGTH, datas.phone, datas.datetime);  /* Read message */
//    memset(datas.message, 0, 160);
//    //time_Data = messageConvertedtoIntofReceivedMsg /* Convertion of read string into interger value for setting time */
//    datas.messageIndex = aquamo_Sim900.isSMSunread();
//    
}

void loop() 
        {  
          Status.sense_Current_aerator_1 =analogRead(currentSense);
          Serial.println(Status.sense_Current_aerator_1);
          switch(Status.aerator_status)  /* aerator selection is done on timer interrupt */
          {
            case aerator_1:
                  
                     digitalWrite(PIN_AERATOR1,HIGH) ;               //Switch relay of  aerator 1    ON 
                     digitalWrite(PIN_AERATOR2,LOW) ;              //Switch relay of  aerator 2    OFF
                     //Switch LED   of  aerator 1    ON
                     //Turn   LED   of  aerator 2    OFF
                     
                  
            break;
            case aerator_2:  
                   
                     digitalWrite(PIN_AERATOR1,LOW) ;               //Switch relay of  aerator 1    ON 
                     digitalWrite(PIN_AERATOR2,HIGH) ;              //Switch relay of  aerator 2    OFF
                     //Switch LED   of  aerator 1    OFF
                     //Turn   LED   of  aerator 2    ON
                      
                  
            break;
          }

           if(Status.sense_Current_aerator_1 <=450)
            {
              if(Status.aerator_status != aerator_1 )  
              {
                Serial.println("Aerator 1 is in Normal OFF state");
              }
              else 
              {
                aquamo_Sim900.sendSMS(PHONE_NUMBER ,"CAUTION !: Aerator 1 is disconnected");
                //aquamo_Sim900.callUp(PHONE_NUMBER);                             /* Call */
                delay(2500);
//                aquamo_Sim900.hangup();                                       // aLARM  
             }
            }
//          if(Status.sense_Current_aerator_2 <=0)
//            {
//              if(Status.aerator_status != aerator_2 )  
//              {
//                Serial.println("Aerator 2 is in Normal OFF state");
//              }
//              else 
//              {
//                aquamo_Sim900.sendSMS(PHONE_NUMBER ,"RISK ALERT !");
//                aquamo_Sim900.callUp(PHONE_NUMBER);                             /* Call */
//                delay(25000);
//                aquamo_Sim900.hangup();                                       // aLARM  
//              }
//            }
            
        }


     
 
