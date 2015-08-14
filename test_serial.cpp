#include "mbed.h"
#include "stdlib.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
 
Serial pc(USBTX, USBRX);
Serial IMU(p28, p27);  // tx, rx

char IMU_message[256];
int  IMU_message_counter=0;
float IMU_data[3];
 
void callback() {
    char buf;
    
     while (IMU.readable()) {
        buf = IMU.getc();
        
        IMU_message_counter+=1;
        IMU_message[IMU_message_counter]=buf;
        
        if (buf=='#'){
            IMU_message_counter=0;
        }
        
        if (buf=='\n'){ //if got a complete message
            int i=0;
            int IMU_data_counter=0; 
            
            buf
            while(IMU_message[i]!=','){
                if ((IMU_message[i]<58 && IMU_message[i]>47) || IMU_message[i]==46){ //if it is a number
                       
                }
            } 
        } 
    }
       
    led2 = !led2;
}
 
 
 
 
int main() {
    IMU.baud(57600);
    IMU.attach(&callback);
    
    while (1) {
        led1 = !led1;
        wait(0.5);
    }
}