#include "mbed.h"
#include <string>
 
//LED indicators
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
 
Serial pc(USBTX, USBRX);
Serial IMU(p28, p27);  // tx, rx
        if (buf=='\n'){
            int i;
            for(i=0;i<=IMU_message_counter;i++){
                pc.putc(IMU_message[i]);  
                //pc.putc(IMU_message[i]);  
            }  
        } 
    }
    led2 = !led2;
}
 
 
void GPS_serial_ISR() {
    char buf;
    
    led3 = !led3;
}
 
 
void PC_serial_ISR() {
    char buf;
    
     while (pc.readable()) {
        buf = pc.getc();
        pc.putc(buf);    
    }
    
    led4= !led4;
}
 
int main() {
    IMU.baud(57600);
    IMU.attach(&IMU_serial_ISR);
    GPS.baud(38400);
    GPS.attach(&GPS_serial_ISR);
    pc.baud(115200);
    pc.attach(&PC_serial_ISR);
    
    while (1) {
        
        led1 = !led1;
        wait(0.2);
    }