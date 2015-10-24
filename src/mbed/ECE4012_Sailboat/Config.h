#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <math.h>
#include "mbed.h"
#include "Servo.h"
#include "SDFileSystem.h"

using namespace std;

#define MAX_IMU_SIZE 29
#define MAX_TASK_SIZE 5
#define DEG2RAD_RATIO 0.0174532925199433
#define RAD2DEG_RATIO 57.295779513082321
#define EARTH_RADIUS 6371000

extern string IMU_Y;
extern string IMU_P;
extern string IMU_R;
extern string GPS_Quality;
extern string GPS_UTC;
extern string GPS_Latitude;
extern string GPS_Longitude;
extern string GPS_Altitude;
extern string GPS_Num_Satellite;
extern string GPS_HDOP;
extern string GPS_VDOP;
extern string GPS_PDOP;
extern string GPS_Date;
extern string GPS_VelocityKnot;
extern string GPS_VelocityKph;

extern double D_IMU_Y;
extern double D_IMU_Y_north;
extern double D_IMU_P;
extern double D_IMU_R;
extern double D_GPS_Quality;
extern double D_GPS_UTC;
extern double D_GPS_Latitude;
extern double D_GPS_Longitude;
extern double D_GPS_Altitude;
extern double D_GPS_Num_Satellite;
extern double D_GPS_HDOP;
extern double D_GPS_VDOP;
extern double D_GPS_PDOP;
extern double D_GPS_Date;
extern double D_GPS_VelocityKnot;
extern double D_GPS_VelocityKph;

extern double Longitude_Path[MAX_TASK_SIZE];
extern double Latitude_Path[MAX_TASK_SIZE];
extern Serial pc;
extern Servo rudderServo;
extern Servo wingServo;

string decodeCommandGET(string cmd);
string decodeCommandSET(string cmd);
vector<string> split(const string &s, char delim);
double getDistance(int task_id);
double getAngle(int task_id);
double Deg2Rad(double degree);
string stringify(double x);
void printPath();
void printDistance();
void printAngle();