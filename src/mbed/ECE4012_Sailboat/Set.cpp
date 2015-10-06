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
    } else {
        return("Not valid command, example: @SET=PATH, 33.776318, -84.407590, 3");
    }
}