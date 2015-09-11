#include "Config.h"


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
    } else {
        return("Not valid command, example: @GET=GPS_Quality");
    }
}

double getDistance(int task_id) {
    double cur_Latitude = Deg2Rad(D_GPS_Latitude);
    double cur_Logntitude = D_GPS_Longitude;
    double dest_Latitude = Deg2Rad(Latitude_Path[task_id-1]);
    double dest_Longitude = Longitude_Path[task_id-1];   
    return acos(sin(cur_Latitude)*sin(dest_Latitude)+cos(cur_Latitude)*cos(dest_Latitude)*cos(Deg2Rad(dest_Longitude-cur_Logntitude)))*EARTH_RADIUS;
}