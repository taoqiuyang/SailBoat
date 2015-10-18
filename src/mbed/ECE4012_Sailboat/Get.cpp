#include "Config.h"

double Initial_Bearing;
double Final_Bearing;
double D_IMU_Y_north;

string decodeCommandGET(string cmd) {
    if (cmd == "IMU_Y") {
        return(IMU_Y);
    } else if (cmd == "IMU_P") {
        return(IMU_P);
    } else if (cmd == "IMU_R") {
        return(IMU_R);
    } else if (cmd == "GPS_Quality") {
        return(GPS_Quality);
    } else if (cmd == "GPS_UTC") {
        return(GPS_UTC);
    } else if (cmd == "GPS_Latitude") {
        return(GPS_Latitude);
    } else if (cmd == "GPS_Longitude") {
        return(GPS_Longitude);
    } else if (cmd == "GPS_Altitude") {
        return(GPS_Altitude);
    } else if (cmd == "GPS_Num_Satellite") {
        return(GPS_Num_Satellite);
    } else if (cmd == "GPS_HDOP") {
        return(GPS_HDOP);
    } else if (cmd == "GPS_VDOP") {
        return(GPS_VDOP);
    } else if (cmd == "GPS_PDOP") {
        return(GPS_PDOP);
    } else if (cmd == "GPS_Date") {
        return(GPS_Date);
    } else if (cmd == "GPS_VelocityKnot") {
        return(GPS_VelocityKnot);
    } else if (cmd == "GPS_VelocityKph") {
        return(GPS_VelocityKph);
    } else if (cmd == "Path") {
        printPath();
        return NULL;
    } else if (cmd == "Distance") {
        printDistance();
        return NULL;
    } else if (cmd == "Angle") {
        printAngle();
        return NULL;
    } else {
        return("Not valid command, example: @GET=GPS_Quality");
    }
}



double Deg2Rad(double degree) {
    return degree*DEG2RAD_RATIO;
}

double Rad2Degree(double radian) {
    return radian*RAD2DEG_RATIO;
}

double getDistance(int task_id) {
    double cur_Latitude = Deg2Rad(D_GPS_Latitude);
    double cur_Logntitude = Deg2Rad(D_GPS_Longitude);
    double dest_Latitude = Deg2Rad(Latitude_Path[task_id-1]);
    double dest_Longitude = Deg2Rad(Longitude_Path[task_id-1]);
    if(Latitude_Path[task_id-1] == 181 or Longitude_Path[task_id-1] == 181) {
        return -1;
    } else {
        return acos(sin(cur_Latitude)*sin(dest_Latitude)+cos(cur_Latitude)*cos(dest_Latitude)*cos(dest_Longitude-cur_Logntitude))*EARTH_RADIUS;
    }
}

double getDistance_2dest(int task_id_start, int task_id_end) {
    double dest_Latitude_start = Deg2Rad(Latitude_Path[task_id_start-1]);
    double dest_Longitude_start = Deg2Rad(Longitude_Path[task_id_start-1]);
    double dest_Latitude_end = Deg2Rad(Latitude_Path[task_id_end-1]);
    double dest_Longitude_end = Deg2Rad(Longitude_Path[task_id_end-1]);
    if(Latitude_Path[task_id_start-1] == 181 or Longitude_Path[task_id_start-1] == 181 or Latitude_Path[task_id_end-1] == 181 or Longitude_Path[task_id_end-1] == 181) {
        return -1;
    } else {
        return acos(sin(dest_Latitude_start)*sin(dest_Latitude_end)+cos(dest_Latitude_start)*cos(dest_Latitude_end)*cos(dest_Longitude_end-dest_Longitude_start))*EARTH_RADIUS;
    }
}

double getAngle(int task_id) {
    double cur_Latitude = Deg2Rad(D_GPS_Latitude);
    double cur_Logntitude = Deg2Rad(D_GPS_Longitude);
    double dest_Latitude = Deg2Rad(Latitude_Path[task_id-1]);
    double dest_Longitude = Deg2Rad(Longitude_Path[task_id-1]);
    if(Latitude_Path[task_id-1] == 181 or Longitude_Path[task_id-1] == 181) {
        return -361;
    } else {
        Initial_Bearing = Rad2Degree(atan2(sin(dest_Longitude-cur_Logntitude)*cos(dest_Latitude),
                          cos(cur_Latitude)*sin(dest_Latitude)-sin(cur_Latitude)*cos(dest_Latitude)*cos(dest_Longitude-cur_Logntitude)));         
        Final_Bearing = ((int)(Initial_Bearing+180))%360;
        D_IMU_Y_north = (D_IMU_Y<=0)?(D_IMU_Y + 180):(D_IMU_Y - 180);
        double out =  (Initial_Bearing - D_IMU_Y_north< -180)?(Initial_Bearing - D_IMU_Y_north + 360):
                      (Initial_Bearing - D_IMU_Y_north>  180)?(Initial_Bearing - D_IMU_Y_north - 360):(Initial_Bearing - D_IMU_Y_north);
        return out;
    }
}

double getCTE(int task_id) {
    double dis_start = getDistance(task_id);
    double dis_end = getDistance(task_id+1);
    double dis_between = getDistance_2dest(task_id,task_id+1);
    if (dis_start == -1 or dis_end == -1 or dis_between == -1) {
        return -1;
    } else {
        double p = (dis_start+dis_end+dis_between)/2;
        double area = sqrt(p*(p-dis_start)*(p-dis_end)*(p-dis_between));
        return 2*area/dis_between;
    }
}
