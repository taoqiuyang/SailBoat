#include "mbed.h"
#include <string>
#include <sstream>
#include <vector>
#define MAX_IMU_SIZE 28
#define MAX_GPS_GPGGA_SIZE 72
#define MAX_GPS_GPGSV_SIZE 210
#define MAX_GPS_GPRMC_SIZE 70

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
 
Serial pc(USBTX, USBRX);
Serial IMU(p28, p27);  // tx, rx
Serial GPS(p13, p14);  // tx, rx

char IMU_message[256];
int  IMU_message_counter=0;
char GPS_message[256];
int  GPS_message_counter=0;
string IMU_Y="Not ready";
string IMU_P="Not ready";
string IMU_R="Not ready";

vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

void updateIMU(string IMU_data) {
    string IMU_data_string(IMU_data);
    if (IMU_data_string.substr(0,4) == "#YPR" and IMU_data_string.size() <= MAX_IMU_SIZE) {
        IMU_data_string = IMU_data_string.substr(5);
        vector<string> result = split(IMU_data_string, ',');
        IMU_Y = result.at(0);
        IMU_P = result.at(1);
        IMU_R = result.at(2).substr(0, result.at(2).size()-1);;
        }
}

void updateGPS(string GPS_data) {
    string GPS_data_string(GPS_data);
    if (GPS_data_string.substr(0,6) == "$GPGGA" and GPS_data_string.size() <= MAX_GPS_GPGGA_SIZE) {
        GPS_data_string = IMU_data_string.substr(5);
        vector<string> result = split(GPS_data_string, ',');
        IMU_Y = result.at(0);
        IMU_P = result.at(1);
        IMU_R = result.at(2).substr(0, result.at(2).size()-1);
    } else if (GPS_data_string.substr(0,6) == "%GPGSV" and GPS_data_string.size() <= MAX_GPS_GPGSV_SIZE) {
        GPS_data_string = IMU_data_string.substr(5);
        vector<string> result = split(GPS_data_string, ',');
        IMU_Y = result.at(0);
        IMU_P = result.at(1);
        IMU_R = result.at(2).substr(0, result.at(2).size()-1);
    } else if (GPS_data_string.substr(0,6) == "%GPRMC" and GPS_data_string.size() <= MAX_GPS_GPRMC_SIZE) {
        GPS_data_string = IMU_data_string.substr(5);
        vector<string> result = split(GPS_data_string, ',');
        IMU_Y = result.at(0);
        IMU_P = result.at(1);
        IMU_R = result.at(2).substr(0, result.at(2).size()-1);
    }
}

void printState() {
    pc.printf("IMU_Y: %s, IMU_P: %s, IMU_R: %s\n",IMU_Y,IMU_P,IMU_R);
 }
 
 
 //#YPR=-183.-174,-134.27,-114.39
void IMU_serial_ISR() {
    char buf;
    
     while (IMU.readable()) {
        buf = IMU.getc();
        

        IMU_message[IMU_message_counter]=buf;
        IMU_message_counter+=1;
        
        if (buf=='\n'){
            string IMU_copy(IMU_message, IMU_message_counter);
            //pc.printf("%s\n", IMU_copy);
            updateIMU(IMU_copy);
            printState();
            IMU_message_counter=0; 
            IMU_copy[0] = '\0';
        }

    }
    led2 = !led2;
}


 
void GPS_serial_ISR() {
    char buf;
    
     while (GPS.readable()) {
        buf = GPS.getc();
        

        GPS_message[GPS_message_counter]=buf;
        GPS_message_counter+=1;
        
        if (buf=='\n'){
            string GPS_copy(GPS_message, GPS_message_counter);
            //pc.printf("%s\n", IMU_copy);
            updateGPS(GPS_copy);
            printState();
            GPS_message_counter=0; 
            GPS_copy[0] = '\0';
        }  
    }
    
    led3 = !led3;
}
 
void PC_serial_ISR() {
    char buf;
    
     while (pc.readable()) {
        buf = pc.getc();
        //pc.putc(buf);    
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
}