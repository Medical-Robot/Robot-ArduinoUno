#pragma once
#include <vector>
#include <Path.h>

typedef struct Checkpoint_s{
	int id;
	int left_id;
	int right_id;
	int front_id;
	int back_id;
}Checkpoint;

class Map
{
private:
    std::vector<Checkpoint> checkPoints;
    Checkpoint previousCheckPoint;
	Checkpoint nextCheckPoint;
    
public:
    Map(/* args */){

    }
    ~Map(){

    }

    void addCheckPoint(Checkpoint newCheckpoint) {
		checkPoints.push_back(newCheckpoint);
	}

    Checkpoint* findCheckPointById(int checkPointId) {
		for (size_t i = 0; i < checkPoints.size(); i++)
		{
			if (checkPoints[i].id == checkPointId) {
				return &(checkPoints[i]);
			}
		}
		return NULL;
	}

	void removeCheckPointById(int checkPointId) {
		std::vector<Checkpoint>::iterator nth;
		for (size_t i = 0; i < checkPoints.size(); i++)
		{
			if (checkPoints[i].id == checkPointId) {
				nth = checkPoints.begin() + i;
				checkPoints.erase(nth);
			}
		}
	}

    std::vector<Checkpoint>& getCheckPoints() {
		return this->checkPoints;
	}

	Checkpoint getPreviousCheckpoint() {
		return this->previousCheckPoint;
	}

	Checkpoint getNextCheckPoint() {
		return this->nextCheckPoint;
	}

    void setNextCheckPoint(Checkpoint newCheckpoint){
		this->nextCheckPoint = newCheckpoint;
	}

	void setPreviousCheckPoint(Checkpoint newCheckpoint){
		this->previousCheckPoint = newCheckpoint;
	}

	static Path findPath(Checkpoint _prevCheckpoint, Checkpoint nexCheckpoint_, Checkpoint destinationCheckpoint_){
		Path path;
		


	}
};


