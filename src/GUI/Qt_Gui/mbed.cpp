#include "mbed.h"
#include "uLCD_4DGL.h"

Serial pc(USBTX, USBRX); // tx, rx
uLCD_4DGL lcd(p28, p27, p30);

int main() {
    pc.baud(9600);
    lcd.baudrate(3000000);
    lcd.cls();
    
    int r = 1;
    while(1){
        if(pc.readable()){
            char command = pc.getc();
            lcd.printf("%c", command);
            switch (command) {
           
                default:
                    r++;
                    pc.printf("number is %d", r);
                    break;
            }
        }   
    } 
}