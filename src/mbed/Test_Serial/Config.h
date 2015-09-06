#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include "mbed.h"
#include "Servo.h"
#include "SDFileSystem.h"

using namespace std;

#define MAX_IMU_SIZE 29
#define MAX_TASK_SIZE 5

extern string IMU_Y;
extern string IMU_P;
extern string IMU_R;
extern string GPS_Quality;
extern string GPS_UTC;
extern string GPS_Latitude;
extern string GPS_Longtitude;
extern string GPS_Altitude;
extern string GPS_Num_Satellite;
extern string GPS_HDOP;
extern string GPS_VDOP;
extern string GPS_PDOP;
extern string GPS_Date;
extern string GPS_VelocityKnot;
extern string GPS_VelocityKph;

extern double D_IMU_Y;
extern double D_IMU_P;
extern double D_IMU_R;
extern double D_GPS_Quality;
extern double D_GPS_UTC;
extern double D_GPS_Latitude;
extern double D_GPS_Longtitude;
extern double D_GPS_Altitude;
extern double D_GPS_Num_Satellite;
extern double D_GPS_HDOP;
extern double D_GPS_VDOP;
extern double D_GPS_PDOP;
extern double D_GPS_Date;
extern double D_GPS_VelocityKnot;
extern double D_GPS_VelocityKph;

extern double Longtitude_Path[MAX_TASK_SIZE];
extern double Latitude_Path[MAX_TASK_SIZE];
extern Serial pc;

string getIMU_Y();
string getIMU_P();
string getIMU_R();
string getGPS_Quality();
string getGPS_UTC();
string getGPS_quality();
string getGPS_Latitude();
string getGPS_Longtitude();
string getGPS_Altitude();
string getGPS_Num_Satellite();
string getGPS_HDOP();
string getGPS_VDOP();
string getGPS_PDOP();
string getGPS_Date();
string getGPS_VelocityKnot();
string getGPS_VelocityKph();
string decodeCommandGET(string cmd);
string decodeCommandSET(string cmd);
vector<string> split(const string &s, char delim);