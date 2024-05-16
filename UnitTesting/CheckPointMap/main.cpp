#include "Map.h"
#include "Path.h"

int main() {
	Path checkpointPath;
	Map checkpointMap;
	Checkpoint checkPoint;
	CheckPointDirection direction;

	checkPoint.id = 1;
	checkPoint.front_id = 2;
	checkPoint.back_id = 0;
	checkPoint.left_id = 0;
	checkPoint.right_id = 0;
	checkpointMap.addCheckPoint(checkPoint);

	checkPoint.id = 2;
	checkPoint.front_id = 3;
	checkPoint.back_id = 1;
	checkPoint.left_id = 7;
	checkPoint.right_id = 0;
	checkpointMap.addCheckPoint(checkPoint);

	checkPoint.id = 3;
	checkPoint.front_id = 8;
	checkPoint.back_id = 2;
	checkPoint.left_id = 0;
	checkPoint.right_id = 5;
	checkpointMap.addCheckPoint(checkPoint);

	checkPoint.id = 5;
	checkPoint.front_id = 0;
	checkPoint.back_id = 0;
	checkPoint.left_id = 3;
	checkPoint.right_id = 0;
	checkpointMap.addCheckPoint(checkPoint);

	checkPoint.id = 7;
	checkPoint.front_id = 0;
	checkPoint.back_id = 0;
	checkPoint.left_id = 0;
	checkPoint.right_id = 2;
	checkpointMap.addCheckPoint(checkPoint);

	checkPoint.id = 8;
	checkPoint.front_id = 0;
	checkPoint.back_id = 3;
	checkPoint.left_id = 0;
	checkPoint.right_id = 0;
	checkpointMap.addCheckPoint(checkPoint);

	checkpointMap.setPreviousCheckPoint(1);
	checkpointMap.setNextCheckPoint(2);

	checkpointPath = checkpointMap.findPath(8);
	direction = checkpointPath.getNextDirection();
	checkpointPath.goNextCheckPoint();
	direction = checkpointPath.getNextDirection();
	checkpointPath.goNextCheckPoint();
	direction = checkpointPath.getNextDirection();
	
	return 0;
}