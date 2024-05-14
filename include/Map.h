#pragma once
#include <vector>

typedef enum CheckPointDirection_e {LEFT, RIGHT, FRONT, BACK, NONE}CheckPointDirection;
typedef struct CheckPoint_s{
	int id;
	int next_Checkpoint_id;
	int left_id;
	int right_id;
	int front_id;
	int back_id;
}Checkpoint;

class Map
{
public:
	Map() {
		
	}
	~Map() {

	}

	void addCheckPoint(Checkpoint newCheckpoint) {
		checkPoints.push_back(newCheckpoint);
	}

	CheckPointDirection getNextDirection() {
		return Map::getCheckpointNextDirection(this->nextCheckPoint);
	}

	void goNextCheckPoint() {
		Checkpoint* temp_nextCheckPoint;

		previousCheckPoint = nextCheckPoint;
		temp_nextCheckPoint = this->findCheckPointById(previousCheckPoint.id);

		if (temp_nextCheckPoint == NULL) {
			nextCheckPoint = previousCheckPoint;
		}
		else {
			nextCheckPoint = *temp_nextCheckPoint;
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

	static CheckPointDirection getCheckpointNextDirection(Checkpoint checkpoint) {
		CheckPointDirection direction;
		if (checkpoint.next_Checkpoint_id == checkpoint.front_id) {
			direction = CheckPointDirection::FRONT;
		}
		else if (checkpoint.next_Checkpoint_id == checkpoint.back_id) {
			direction = CheckPointDirection::BACK;
		}
		else if (checkpoint.next_Checkpoint_id == checkpoint.left_id) {
			direction = CheckPointDirection::LEFT;
		}
		else if (checkpoint.next_Checkpoint_id == checkpoint.right_id) {
			direction = CheckPointDirection::RIGHT;
		}
		else {
			direction = CheckPointDirection::NONE;
		}
		return direction;
	}


private:
	std::vector<Checkpoint> checkPoints;
	Checkpoint previousCheckPoint;
	Checkpoint nextCheckPoint;	

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
};


