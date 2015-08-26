#include "Get.h"


string getIMU_Y() {
    return IMU_Y;
}

string getIMU_P() {
    return IMU_P;
}

string getIMU_R() {
    return IMU_R;
}

string getGPS_Quality() {
    return GPS_Quality;
}

string getGPS_UTC() {
    return GPS_UTC;
}

string getGPS_Latitude() {
    return GPS_Latitude;
}

string getGPS_Longtitude() {
    return GPS_Longtitude;
}

string getGPS_Altitude() {
    return GPS_Altitude;
}

string getGPS_Num_Satellite() {
    return GPS_Num_Satellite;
}

string getGPS_HDOP() {
    return GPS_HDOP;
}

string getGPS_VDOP() {
    return GPS_VDOP;
}

string getGPS_PDOP() {
    return GPS_PDOP;
}

string getGPS_Date() {
    return GPS_Date;
}

string getGPS_VelocityKnot() {
    return GPS_VelocityKnot;
}

string getGPS_VelocityKph() {
    return GPS_VelocityKph;
}
    
string decodeCommand(string cmd) {
    if (cmd == "IMU_Y") {
        return(getIMU_Y());
    } else if (cmd == "IMU_P") {
        return(getIMU_P());
    } else if (cmd == "IMU_R") {
        return(getIMU_R());
    } else if (cmd == "GPS_Quality") {
        return(getGPS_Quality());
    } else if (cmd == "GPS_UTC") {
        return(getGPS_UTC());
    } else if (cmd == "GPS_Latitude") {
        return(getGPS_Latitude());
    } else if (cmd == "GPS_Longtitude") {
        return(getGPS_Longtitude());
    } else if (cmd == "GPS_Altitude") {
        return(getGPS_Altitude());
    } else if (cmd == "GPS_Num_Satellite") {
        return(getGPS_Num_Satellite());
    } else if (cmd == "GPS_HDOP") {
        return(getGPS_HDOP());
    } else if (cmd == "GPS_VDOP") {
        return(getGPS_VDOP());
    } else if (cmd == "GPS_PDOP") {
        return(getGPS_PDOP());
    } else if (cmd == "GPS_Date") {
        return(getGPS_Date());
    } else if (cmd == "GPS_VelocityKnot") {
        return(getGPS_VelocityKnot());
    } else if (cmd == "GPS_VelocityKph") {
        return(getGPS_VelocityKph());
    } else {
        return("Not acceptable command, example: @GET=GPS_Quality");
    }
}