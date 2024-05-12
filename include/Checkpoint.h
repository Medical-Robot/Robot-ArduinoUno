#include "PurePursuitGeometry.h"
#include <vector>
#include<iostream>

class Checkpoint
{
private:
    float x;
    float y;
public:
    Checkpoint(float x, float y){
        this->x = x;
        this->y = y;
    }
};


class CheckpointHandler
{
private:
    std::vector<Checkpoint> checkpoints;
    int curentCheckpointIndex;
    std::string direction;

public:

    void AddCheckpoint(Checkpoint cp){
        checkpoints.push_back(cp);
    }

    void SetCurentCheckpoint(int index){
        this->curentCheckpointIndex = index;
    }

    void SetDirection(std::string direction){
        this->direction = direction;
    }

    void NextCheckpointHandler(){
        if (direction.compare("ID_next_front_waypoint")){
            direction = "ID_next_front_waypoint";
        }
        else if (direction.compare("ID_next_right_waypoint")){
            direction = "ID_next_right_waypoint";
        }
        else if (direction.compare("ID_next_left_waypoint")){
            direction = "ID_next_left_waypoint";
        }
        else if (direction.compare("ID_next_back_waypoint")){
            direction = "ID_next_back_waypoint";
        }
        else direction = "Error Direction: Not set yet.";
        
    }

};





