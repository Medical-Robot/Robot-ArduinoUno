#pragma once
#include <vector>

typedef enum CheckPointDirection_e { LEFT, RIGHT, FRONT, BACK, NONE }CheckPointDirection;
typedef struct PathCheckpoint_s {
	int id;
	int next_checkpoint_id;
	int left_id;	// in
	int right_id;	// out
	int front_id;	// out
	int back_id;	// in
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

	void setNextCheckPoint(PathCheckpoint newCheckpoint) {
		this->nextCheckPoint = newCheckpoint;
	}

	void setPreviousCheckPoint(PathCheckpoint newCheckpoint) {
		this->previousCheckPoint = newCheckpoint;
	}

	void goNextCheckPoint() {
		PathCheckpoint* temp_nextCheckPoint;

		if (this->nextCheckPoint.id <= 0) {
			return;
		}

		this->previousCheckPoint = this->nextCheckPoint;
		temp_nextCheckPoint = this->findCheckPointById(this->previousCheckPoint.next_checkpoint_id);

		if (temp_nextCheckPoint == NULL) {
			this->nextCheckPoint = this->previousCheckPoint;
		}
		else {
			this->nextCheckPoint = *temp_nextCheckPoint;
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


	CheckPointDirection getNextDirection() {
		return Path::getCheckpointNextDirection(this->nextCheckPoint);
	}

	CheckPointDirection getCheckpointNextDirection(PathCheckpoint checkpoint) {
		CheckPointDirection direction;
		PathCheckpoint nextCheckpoint_temp, *temp_ptr;
		int temp;

		temp_ptr = findCheckPointById(checkpoint.next_checkpoint_id);
		if (temp_ptr == NULL) {
			return CheckPointDirection::NONE;
		}
		nextCheckpoint_temp = *temp_ptr;

		if (nextCheckpoint_temp.back_id == checkpoint.id) {

		}
		else if (nextCheckpoint_temp.front_id == checkpoint.id) {
			temp = checkpoint.left_id;
			checkpoint.left_id = checkpoint.right_id;
			checkpoint.right_id = temp;

			temp = checkpoint.front_id;
			checkpoint.front_id = checkpoint.back_id;
			checkpoint.back_id = temp;
		}
		else if (nextCheckpoint_temp.left_id == checkpoint.id) {

		}
		else if (nextCheckpoint_temp.right_id == checkpoint.id) {
			temp = checkpoint.front_id;
			checkpoint.front_id = checkpoint.back_id;
			checkpoint.back_id = temp;

			temp = checkpoint.left_id;
			checkpoint.left_id = checkpoint.right_id;
			checkpoint.right_id = temp;
		}

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