#include "Config.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
 
Serial pc(USBTX, USBRX);
Serial IMU(p28, p27);  // tx, rx
Serial GPS(p13, p14);  // tx, rx
Servo rudderServo(p25);
Servo wingServo(p26);
SDFileSystem sd(p5, p6, p7, p8, "sd");// mosi, miso, sck, cs

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
string GPS_Altitude_Unit="N/A";
string GPS_Num_Satellite="N/A";
string GPS_HDOP="N/A";
string GPS_VDOP="N/A";
string GPS_PDOP="N/A";
string GPS_Date="N/A";
string GPS_VelocityKnot="N/A";
string GPS_VelocityKnot_Unit="N/A";
string GPS_VelocityKph="N/A";
string GPS_VelocityKph_Unit="N/A";


double D_IMU_Y=0;
double D_IMU_P=0;
double D_IMU_R=0;
double D_GPS_Quality=0;
double D_GPS_UTC=0;
double D_GPS_Latitude=0;
double D_GPS_Latitude_Direction=0;
double D_GPS_Longtitude=0;
double D_GPS_Longtitude_Direction=0;
double D_GPS_Altitude=0;
double D_GPS_Altitude_Unit=0;
double D_GPS_Num_Satellite=0;
double D_GPS_HDOP=0;
double D_GPS_VDOP=0;
double D_GPS_PDOP=0;
double D_GPS_Date=0;
double D_GPS_VelocityKnot=0;
double D_GPS_VelocityKnot_Unit=0;
double D_GPS_VelocityKph=0;
double D_GPS_VelocityKph_Unit=0;

int asterisk_idx;

double Longtitude_Path[MAX_TASK_SIZE];
double Latitude_Path[MAX_TASK_SIZE];

vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        if (item.empty()) {
            item = "N/A";
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
        D_IMU_Y = strtod(result.at(0).c_str(), NULL);
        IMU_P = result.at(1);
        D_IMU_P = strtod(result.at(1).c_str(), NULL);       
        IMU_R = result.at(2).substr(0, result.at(2).size()-1);
        D_IMU_R = strtod(result.at(2).substr(0, result.at(2).size()-1).c_str(), NULL);  
        }
}

void updateGPS(string GPS_data) {
    string GPS_data_string(GPS_data);
    if (GPS_data_string.substr(0,6) == "$GPGGA") {
        GPS_data_string = GPS_data_string.substr(7);
        vector<string> result = split(GPS_data_string, ',');
        GPS_Quality = result.at(5);
        D_GPS_Quality = strtod(result.at(5).c_str(), NULL);
        if(GPS_Quality != "0" and GPS_Quality != "6") {
            GPS_UTC = result.at(0);
            D_GPS_UTC = strtod(result.at(0).c_str(), NULL);
            GPS_Latitude = result.at(2) + result.at(1);
            D_GPS_Latitude = strtod(result.at(1).c_str(), NULL);
            if (result.at(2) == "N") {
              D_GPS_Latitude_Direction = 0;
            } else if (result.at(2) == "S") {
              D_GPS_Latitude_Direction = 1;  
            }
            GPS_Longtitude = result.at(4) + result.at(3);
            D_GPS_Longtitude = strtod(result.at(3).c_str(), NULL);
            if (result.at(4) == "W") {
              D_GPS_Latitude_Direction = 0;
            } else if (result.at(4) == "E") {
              D_GPS_Latitude_Direction = 1;  
            }
            GPS_Num_Satellite = result.at(6);
            D_GPS_Num_Satellite = strtod(result.at(6).c_str(), NULL);
            GPS_HDOP = result.at(7);
            D_GPS_HDOP = strtod(result.at(7).c_str(), NULL);
            GPS_Altitude = result.at(8) + result.at(9);
            D_GPS_Altitude = strtod(result.at(8).c_str(), NULL);
            GPS_Altitude_Unit = result.at(9);
        }
    } else if (GPS_data_string.substr(0,6) == "$GPGSA" and GPS_Quality != "0" and GPS_Quality != "6") {
        GPS_data_string = GPS_data_string.substr(7);
        vector<string> result = split(GPS_data_string, ',');
        GPS_PDOP = result.at(14);
        D_GPS_PDOP = strtod(result.at(14).c_str(), NULL);
        asterisk_idx = result.at(16).find('*');
        GPS_VDOP = result.at(16).substr(0, asterisk_idx);
        D_GPS_VDOP = strtod(result.at(16).substr(0, asterisk_idx).c_str(), NULL);
    } else if (GPS_data_string.substr(0,6) == "$GPRMC" and GPS_Quality != "0" and GPS_Quality != "6") {
        GPS_data_string = GPS_data_string.substr(7);
        vector<string> result = split(GPS_data_string, ',');
        GPS_Date = result.at(8);
        D_GPS_Date = strtod(result.at(8).c_str(), NULL);
    } else if (GPS_data_string.substr(0,6) == "$GPVTG" and GPS_Quality != "0" and GPS_Quality != "6") {
        GPS_data_string = GPS_data_string.substr(7);
        vector<string> result = split(GPS_data_string, ',');
        GPS_VelocityKnot = result.at(4) + result.at(5);
        D_GPS_VelocityKnot = strtod(result.at(4).c_str(), NULL);
        GPS_VelocityKnot_Unit = result.at(5);
        asterisk_idx = result.at(7).find('*');
        GPS_VelocityKph = result.at(6) + result.at(7).substr(0, asterisk_idx);
        D_GPS_VelocityKph = strtod(result.at(6).c_str(), NULL);
        GPS_VelocityKph_Unit = result.at(7).substr(0, asterisk_idx);
    }
        
}
/*  Get data from SailBoat
    @GET=parameter
    Ex: @GET=GPS_Quality
    Supported parameter: GPS_Quality, GPS_UTC, GPS_Latitude, GPS_Longtitude, GPS_Altitude,
    GPS_Num_Satellite, GPS_HDOP, GPS_VDOP, GPS_PDOP, GPS_Date, GPS_VelocityKnot, GPS_VelocityKph
    Set path to SailBoat
    @SET=PATH, Latitude, Longtitude, Task id
    @SET=PATH, 33.776318, -84.407590, 3 
*/
void decodePC(string PC_data) {
    string PC_data_string(PC_data);
    if (PC_data_string.substr(0,4) == "@GET") {
        pc.printf("%s", PC_data_string.c_str());
        PC_data_string = PC_data_string.substr(5, PC_data_string.size()-6);
        pc.printf("%s\n", decodeCommandGET(PC_data_string).c_str());
    } else if (PC_data_string.substr(0,4) == "@SET") {
        pc.printf("%s", PC_data_string.c_str());
        PC_data_string = PC_data_string.substr(5, PC_data_string.size()-6);
        string claim = decodeCommandSET(PC_data_string);
        if (claim == "DONE") {
            pc.printf("Current Path: Longtitude, Latitude\n");
            for (int i=0;i<MAX_TASK_SIZE;++i) {
                pc.printf("              %f, %f\n", Longtitude_Path[i], Latitude_Path[i]);
            }
        }
        pc.printf("%s\n", claim.c_str());
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

void set_servo_position(Servo targetServo, float angleDeg, float minDeg, float minNorVal, float maxDeg, float maxNorVal){
    /*angleDeg:  desired angle
      minDeg:    minimum angle in degrees
      minNorVal: normalized value[0,1] when servo is at its minimum angle
      maxDeg:    maximum angle in degrees
      maxNorVal: normalized value[0,1] when servo is at its maximum angle
    */
    
    float pos; //normalized angle, [0,1]
    float scale; //scale factor for servo calibration
    
    //extreme conditions
    if (angleDeg<minDeg){
        pos=minNorVal;
    }   
    if (angleDeg>maxDeg){
        pos=maxNorVal;
    }
    
    //Calculate scale factor for servo calibration
    scale = (angleDeg-minDeg)/(maxDeg-minDeg);
    //Calculate servo normalized value
    pos = minNorVal + scale*(maxNorVal-minNorVal);
    
    //send signal to servo
    targetServo=pos;
}

int log_data_SD(){   
    FILE *fp = fopen("/sd/dataLog/dataLog.txt", "w");
    if(fp == NULL) {
        return 0;
    }else{
        //Write all the useful data to the SD card        
        fprintf(fp, "Nya Pass~");
        fclose(fp); 
        return 1;
    }
}


int main() {
    IMU.baud(57600);
    IMU.attach(&IMU_serial_ISR);
    GPS.baud(38400);
    GPS.attach(&GPS_serial_ISR);
    pc.baud(115200);
    pc.attach(&PC_serial_ISR);
    
float angle=20;    
    while (1) {
        if (angle>160){angle=20;}               
        set_servo_position(rudderServo, angle, 0, 0, 180, 1);
        set_servo_position(wingServo, angle, 0, 0, 180, 1);        
        angle=angle+10;
        wait(0.4);
        //printStateIMU();
        //printStateGPS();
        led1 = !led1;
    }
}