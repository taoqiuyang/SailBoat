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
Ticker ctrl_updt_timer; //timer to refresh controller

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
string GPS_Longitude="N/A";
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
string temp = "";

double D_IMU_Y=0;
double D_IMU_P=0;
double D_IMU_R=0;
double D_GPS_Quality=0;
double D_GPS_UTC=0;
double D_GPS_Latitude=0;
double D_GPS_Latitude_Direction=0;
double D_GPS_Longitude=0;
double D_GPS_Longitude_Direction=0;
double D_GPS_Altitude=0;
double D_GPS_Num_Satellite=0;
double D_GPS_HDOP=0;
double D_GPS_VDOP=0;
double D_GPS_PDOP=0;
double D_GPS_Date=0;
double D_GPS_VelocityKnot=0;
double D_GPS_VelocityKph=0;
double D_temp=0;
int asterisk_idx;

double Longitude_Path[MAX_TASK_SIZE];
double Latitude_Path[MAX_TASK_SIZE];

double angle_to_path_point,distance_to_path_point,desired_speed;
double rudder_ctrl_parameters[3];
double rudder_variables[5];//,,,prev,out
double T=0.2; //controller update period=0.2sec, 5Hz


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

double GPSdecimal(double coordinate) {
     //Convert a NMEA decimal-decimal value into decimal degree value
     //5144.3855 (ddmm.mmmm) = 51 44.3855 = 51 + 44.3855/60 = 51.7397583 degrees
     coordinate = coordinate/100;
     return ((int) coordinate) + ((coordinate-((int) coordinate))/60);    
}

void initialize_Path() {
    fill(Longitude_Path, Longitude_Path+MAX_TASK_SIZE, 181);
    fill(Latitude_Path, Latitude_Path+MAX_TASK_SIZE, 181);
}

void updateIMU(string IMU_data) {
    string IMU_data_string(IMU_data);
    if (IMU_data_string.substr(0,4) == "#YPR" and IMU_data_string.size() <= MAX_IMU_SIZE) {
        IMU_data_string = IMU_data_string.substr(5);
        vector<string> result = split(IMU_data_string, ',');
        IMU_Y = result.at(0);
        D_IMU_Y = strtod(IMU_Y.c_str(), NULL);
        IMU_P = result.at(1);
        D_IMU_P = strtod(IMU_P.c_str(), NULL);       
        IMU_R = result.at(2).substr(0, result.at(2).size()-1);
        D_IMU_R = strtod(IMU_R.c_str(), NULL);  
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
            D_GPS_UTC = strtod(GPS_UTC.c_str(), NULL);
            GPS_Latitude = result.at(1) + result.at(2);
            if (result.at(2) == "N") { // 0 means North
              D_GPS_Latitude_Direction = 0;
            } else if (result.at(2) == "S") { // 1 means South
              D_GPS_Latitude_Direction = 1;
              temp = "-" + result.at(1);  
            }
            D_temp = strtod(temp.c_str(), NULL);
            D_GPS_Latitude = GPSdecimal(D_temp);
            GPS_Longitude = result.at(3) + result.at(4);
            if (result.at(4) == "E") { // 0 means East
              D_GPS_Longitude_Direction = 0;
            } else if (result.at(4) == "W") { // 1 means West
              D_GPS_Longitude_Direction = 1; 
              temp = "-" + result.at(3); 
            }
            D_temp = strtod(temp.c_str(), NULL);
            D_GPS_Longitude = GPSdecimal(D_temp);
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
    Supported parameter: GPS_Quality, GPS_UTC, GPS_Latitude, GPS_Longitude, GPS_Altitude,
    GPS_Num_Satellite, GPS_HDOP, GPS_VDOP, GPS_PDOP, GPS_Date, GPS_VelocityKnot, GPS_VelocityKph
    Set path to SailBoat
    @SET=PATH, Latitude, Longitude, Task id
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
            pc.printf("Current Path: Longitude, Latitude\n");
            for (int i=0;i<MAX_TASK_SIZE;++i) {
                pc.printf("              %f, %f\n", Longitude_Path[i], Latitude_Path[i]);
            }
        }
        pc.printf("%s\n", claim.c_str());
    }
}



void printStateIMU() {
     //pc.printf("IMU_Y: %s, IMU_P: %s, IMU_R: %s\n",IMU_Y.c_str(), IMU_P.c_str(), IMU_R.c_str());
     pc.printf("D_IMU_Y: %f, D_IMU_P: %f, D_IMU_R: %f\n",D_IMU_Y, D_IMU_P, D_IMU_R);                  
 }
 
void printStateGPS() {
   /*
   pc.printf("GPS_Quality: %s, GPS_UTC: %s, GPS_Latitude: %s, GPS_Longitude: %s, GPS_Altitude: %s, "
            "GPS_Num_Satellite: %s, GPS_HDOP: %s, GPS_VDOP: %s, GPS_PDOP: %s, GPS_Date: %s, GPS_VelocityKnot: %s, GPS_VelocityKph: %s\n",
            GPS_Quality.c_str(), GPS_UTC.c_str(), GPS_Latitude.c_str(), GPS_Longitude.c_str(), GPS_Altitude.c_str(), GPS_Num_Satellite.c_str(),
            GPS_HDOP.c_str(), GPS_VDOP.c_str(), GPS_PDOP.c_str(), GPS_Date.c_str(), GPS_VelocityKnot.c_str(), GPS_VelocityKph.c_str());
   */  
    
    pc.printf("D_GPS_Quality: %f, D_GPS_UTC: %f, D_GPS_Latitude: %f, D_GPS_Latitude_Direction: %f, D_GPS_Longitude: %f, D_GPS_Longitude_Direction: %f, D_GPS_Altitude: %f,\n"
            "D_GPS_Num_Satellite: %f, D_GPS_HDOP: %f, D_GPS_VDOP: %f, D_GPS_PDOP: %f, D_GPS_Date: %f, D_GPS_VelocityKnot: %f, D_GPS_VelocityKph: %f\n",
            D_GPS_Quality, D_GPS_UTC, D_GPS_Latitude, D_GPS_Latitude_Direction, D_GPS_Longitude, D_GPS_Longitude_Direction, D_GPS_Altitude, D_GPS_Num_Satellite,
            D_GPS_HDOP, D_GPS_VDOP, D_GPS_PDOP, D_GPS_Date, D_GPS_VelocityKnot, D_GPS_VelocityKph);      
          
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


void initialize_controller(){
    rudder_variables[0]=0;
    rudder_variables[1]=0;
    rudder_variables[2]=0;
    rudder_variables[3]=0;
    rudder_variables[4]=0;
    rudder_ctrl_parameters[0]=1;
    rudder_ctrl_parameters[1]=0;
    rudder_ctrl_parameters[2]=0;
}

void update_controller_tmr_ISR() {
    /*Input:  angle(deg) difference between heading from IMU and path point航向与目的点之间的夹角   
              distance(meter) to the next path point
      Global Variables: angle_to_path_point,distance_to_path_point;
      
      Function: drive two servos to navigate the sailboat to the desired path point
    */

    rudder_variables[0]=rudder_ctrl_parameters[0]*angle_to_path_point;
    rudder_variables[1]=rudder_variables[1]+rudder_ctrl_parameters[1]*angle_to_path_point*T;
    rudder_variables[2]=(rudder_variables[3]-angle_to_path_point)/T;
    rudder_variables[3]=angle_to_path_point;
    rudder_variables[4]=rudder_variables[0]+rudder_variables[1]+rudder_variables[2];
    
    
    //Drive servos
    set_servo_position(rudderServo,rudder_variables[4],-45,0,45,1);
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
    ctrl_updt_timer.attach(&update_controller_tmr_ISR, T); // Update controller at 1/T Hz
   
    initialize_controller();
    initialize_Path();
    //float angle=20;    
    while (1) {
    //    if (angle>160){angle=20;}               
    //    set_servo_position(rudderServo, angle, 0, 0, 180, 1);
    //    set_servo_position(wingServo, angle, 0, 0, 180, 1);        
    //    angle=angle+10;
    
    
        wait(0.4);
        printStateIMU();
        printStateGPS();
        led1 = !led1;
    }
}