#include "Config.h"


string head;
double longtitude;
double latitude;
int task_id;

string setIMU_Y() {
    return IMU_Y;
}

string setIMU_P() {
    return IMU_P;
}

string setIMU_R() {
    return IMU_R;
}

string setGPS_Quality() {
    return GPS_Quality;
}

string setGPS_UTC() {
    return GPS_UTC;
}

string setGPS_Latitude() {
    return GPS_Latitude;
}

string setGPS_Longtitude() {
    return GPS_Longtitude;
}

string setGPS_Altitude() {
    return GPS_Altitude;
}

string setGPS_Num_Satellite() {
    return GPS_Num_Satellite;
}

string setGPS_HDOP() {
    return GPS_HDOP;
}

string setGPS_VDOP() {
    return GPS_VDOP;
}

string setGPS_PDOP() {
    return GPS_PDOP;
}

string setGPS_Date() {
    return GPS_Date;
}

string setGPS_VelocityKnot() {
    return GPS_VelocityKnot;
}

string setGPS_VelocityKph() {
    return GPS_VelocityKph;
}

string decodeCommandSET(string cmd) {
    vector<string> result = split(cmd, ',');
    head = result.at(0);
    if (head == "PATH") {
        longtitude = strtod(result.at(1).c_str(), NULL);
        //pc.printf("%f\n",longtitude);
        latitude = strtod(result.at(2).c_str(), NULL);
        //pc.printf("%f\n",latitude);
        task_id = atoi(result.at(3).c_str());
        //pc.printf("%d\n",task_id);
        if (task_id < 1 or task_id > MAX_TASK_SIZE) {
            return "Task id Out of Range";
        } else {
            Longtitude_Path[task_id-1] = longtitude;
            Latitude_Path[task_id-1] = latitude;
            return "DONE";
        }
    } else {
        return("Not valid command, example: @SET=PATH, 33.776318, -84.407590, 3");
    }
}