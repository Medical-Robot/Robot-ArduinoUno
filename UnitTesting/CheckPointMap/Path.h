#pragma once
#include <vector>

typedef enum CheckPointDirection_e { LEFT, RIGHT, FRONT, BACK, NONE }CheckPointDirection;
typedef struct PathCheckpoint_s {
	int id;
	int next_checkpoint_id;
	int left_id;
	int right_id;
	int front_id;
	int back_id;
}PathCheckpoint;

class Path
{
public:
	Path() {

	}
	~Path() {

	}

	void addCheckPoint(PathCheckpoint newCheckpoint) {
		checkPoints.push_back(newCheckpoint);
	}

	CheckPointDirection getNextDirection() {
		return Path::getCheckpointNextDirection(this->nextCheckPoint);
	}

	void setNextCheckPoint(PathCheckpoint newCheckpoint) {
		this->nextCheckPoint = newCheckpoint;
	}

	void setPreviousCheckPoint(PathCheckpoint newCheckpoint) {
		this->previousCheckPoint = newCheckpoint;
	}

	void goNextCheckPoint() {
		PathCheckpoint* temp_nextCheckPoint;

		previousCheckPoint = nextCheckPoint;
		temp_nextCheckPoint = this->findCheckPointById(previousCheckPoint.id);

		if (temp_nextCheckPoint == NULL) {
			nextCheckPoint = previousCheckPoint;
		}
		else {
			nextCheckPoint = *temp_nextCheckPoint;
		}
	}

	std::vector<PathCheckpoint>& getCheckPoints() {
		return this->checkPoints;
	}

	PathCheckpoint getPreviousCheckpoint() {
		return this->previousCheckPoint;
	}

	PathCheckpoint getNextCheckPoint() {
		return this->nextCheckPoint;
	}

	static CheckPointDirection getCheckpointNextDirection(PathCheckpoint checkpoint) {
		CheckPointDirection direction;
		if (checkpoint.next_checkpoint_id == checkpoint.front_id) {
			direction = CheckPointDirection::FRONT;
		}
		else if (checkpoint.next_checkpoint_id == checkpoint.back_id) {
			direction = CheckPointDirection::BACK;
		}
		else if (checkpoint.next_checkpoint_id == checkpoint.left_id) {
			direction = CheckPointDirection::LEFT;
		}
		else if (checkpoint.next_checkpoint_id == checkpoint.right_id) {
			direction = CheckPointDirection::RIGHT;
		}
		else {
			direction = CheckPointDirection::NONE;
		}
		return direction;
	}

	PathCheckpoint* findCheckPointById(int checkPointId) {
		for (size_t i = 0; i < checkPoints.size(); i++)
		{
			if (checkPoints[i].id == checkPointId) {
				return &(checkPoints[i]);
			}
		}
		return NULL;
	}

	void removeCheckPointById(int checkPointId) {
		std::vector<PathCheckpoint>::iterator nth;
		for (size_t i = 0; i < checkPoints.size(); i++)
		{
			if (checkPoints[i].id == checkPointId) {
				nth = checkPoints.begin() + i;
				checkPoints.erase(nth);
			}
		}
	}

private:
	std::vector<PathCheckpoint> checkPoints;
	PathCheckpoint previousCheckPoint;
	PathCheckpoint nextCheckPoint;


};


