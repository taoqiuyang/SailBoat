#include "Config.h"



string decodeCommandSET(string cmd) {
    vector<string> result = split(cmd, ',');
    string head = result.at(0);
    if (head == "PATH") {
        double latitude = strtod(result.at(1).c_str(), NULL);
        //pc.printf("%f\n",longitude);
        double longitude = strtod(result.at(2).c_str(), NULL);
        //pc.printf("%f\n",latitude);
        int task_id = atoi(result.at(3).c_str());
        //pc.printf("%d\n",task_id);
        if (task_id < 1 or task_id > MAX_TASK_SIZE) {
            return "Task id Out of Range";
        } else {
            Longitude_Path[task_id-1] = longitude;
            Latitude_Path[task_id-1] = latitude;
            return "DONE";
        }
    } else if(head == "SERVO") {
        string servo = result.at(1);
        double angle = strtod(result.at(2).c_str(), NULL);
        if(servo == "RUDDER" or servo == " RUDDER" or servo == "WING" or servo == " WING"){
            if(angle<=45 and angle >= -45) {
                if(servo == "RUDDER" or servo == " RUDDER"){
                    set_servo_position(rudderServo, angle, 0, 0, 180, 1);
                } else {
                    set_servo_position(wingServo, angle, 0, 0, 180, 1);
                }
                return("angle set");
            } else {
                return("angle should be within range 0-180");
            }
        } else {
            return("SERVO type should be RUDDER or WING");
        }
    } else {
        return("Not valid command, example: @SET=PATH, 33.776318, -84.407590, 3 or @SET=SERVO, RUDDER, 30");
    }
}