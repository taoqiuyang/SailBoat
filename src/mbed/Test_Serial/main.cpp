#include "mbed.h"
#include <string>
#include <sstream>
#include <vector>
#include "Get.h"
using namespace std;

#define MAX_IMU_SIZE 29

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
char PC_message[256];
int  PC_message_counter=0;

string IMU_Y="N/A";
string IMU_P="N/A";
string IMU_R="N/A";
string GPS_Quality="N/A";
string GPS_UTC="N/A";
string GPS_Latitude="N/A";
string GPS_Longtitude="N/A";
string GPS_Altitude="N/A";
string GPS_Num_Satellite="N/A";
string GPS_HDOP="N/A";
string GPS_VDOP="N/A";
string GPS_PDOP="N/A";
string GPS_Date="N/A";
string GPS_VelocityKnot="N/A";
string GPS_VelocityKph="N/A";

int asterisk_idx;

vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        if (item.empty()) {
            item == "N/A";
        }
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
        IMU_R = result.at(2).substr(0, result.at(2).size()-1);
        }
}

void updateGPS(string GPS_data) {
    string GPS_data_string(GPS_data);
    if (GPS_data_string.substr(0,6) == "$GPGGA") {
        GPS_data_string = GPS_data_string.substr(7);
        vector<string> result = split(GPS_data_string, ',');
        GPS_Quality = result.at(5);
        if(GPS_Quality != "0" and GPS_Quality != "6") {
            GPS_UTC = result.at(0);
            GPS_Latitude = result.at(2) + result.at(1);
            GPS_Longtitude = result.at(4) + result.at(3);
            GPS_Num_Satellite = result.at(6);
            GPS_HDOP = result.at(7);
            GPS_Altitude = result.at(8) + result.at(9);
        }
    } else if (GPS_data_string.substr(0,6) == "$GPGSA" and GPS_Quality != "0" and GPS_Quality != "6") {
        GPS_data_string = GPS_data_string.substr(7);
        vector<string> result = split(GPS_data_string, ',');
        GPS_PDOP = result.at(14);
        asterisk_idx = result.at(16).find('*');
        GPS_VDOP = result.at(16).substr(0, asterisk_idx);
    } else if (GPS_data_string.substr(0,6) == "$GPRMC" and GPS_Quality != "0" and GPS_Quality != "6") {
        GPS_data_string = GPS_data_string.substr(7);
        vector<string> result = split(GPS_data_string, ',');
        GPS_Date = result.at(8);
    } else if (GPS_data_string.substr(0,6) == "$GPVTG" and GPS_Quality != "0" and GPS_Quality != "6") {
        GPS_data_string = GPS_data_string.substr(7);
        vector<string> result = split(GPS_data_string, ',');
        GPS_VelocityKnot = result.at(4) + result.at(5);
        asterisk_idx = result.at(7).find('*');
        GPS_VelocityKph = result.at(6) + result.at(7).substr(0, asterisk_idx);
    }
        
}

//@GET=GPS_Quality
void decodePC(string PC_data) {
    string PC_data_string(PC_data);
    if (PC_data_string.substr(0,4) == "@GET") {
        pc.printf("%s", PC_data_string.c_str());
        PC_data_string = PC_data_string.substr(5, PC_data_string.size()-6);
        pc.printf("%s\n", decodeCommand(PC_data_string).c_str());
        }
}


void printStateIMU() {
     pc.printf("IMU_Y: %s, IMU_P: %s, IMU_R: %s\n",IMU_Y.c_str(), IMU_P.c_str(), IMU_R.c_str());                  
 }
 
void printStateGPS() {
   pc.printf("GPS_Quality: %s, GPS_UTC: %s, GPS_Latitude: %s, GPS_Longtitude: %s, GPS_Altitude: %s, "
            "GPS_Num_Satellite: %s, GPS_HDOP: %s, GPS_VDOP: %s, GPS_PDOP: %s, GPS_Date: %s, GPS_VelocityKnot: %s, GPS_VelocityKph: %s\n",
            GPS_Quality.c_str(), GPS_UTC.c_str(), GPS_Latitude.c_str(), GPS_Longtitude.c_str(), GPS_Altitude.c_str(), GPS_Num_Satellite.c_str(),
            GPS_HDOP.c_str(), GPS_VDOP.c_str(), GPS_PDOP.c_str(), GPS_Date.c_str(), GPS_VelocityKnot.c_str(), GPS_VelocityKph.c_str());    
}

//#YPR=-183.74,-134.27,-114.39
void IMU_serial_ISR() {
    char buf;
    
     while (IMU.readable()) {
        buf = IMU.getc();
        
        //pc.putc(buf);
        IMU_message[IMU_message_counter]=buf;
        IMU_message_counter+=1;
        
        if (buf=='\n'){
            string IMU_copy(IMU_message, IMU_message_counter);
            //pc.printf("%s", IMU_copy.c_str());
            updateIMU(IMU_copy);
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
              
        //pc.putc(buf);  
        GPS_message[GPS_message_counter]=buf;
        GPS_message_counter+=1;
        
        if (buf=='\n'){
            string GPS_copy(GPS_message, GPS_message_counter);
            //pc.printf("%s", GPS_copy.c_str());
            updateGPS(GPS_copy);
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
        PC_message[PC_message_counter]=buf;
        PC_message_counter+=1;
        
        if (buf=='\n'){
            string PC_copy(PC_message, PC_message_counter);
            //pc.printf("%s", PC_copy.c_str());
            decodePC(PC_copy);
            PC_message_counter=0; 
            PC_copy[0] = '\0';
        }   
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
        wait(0.4);
        //printStateIMU();
        //printStateGPS();
    }
}