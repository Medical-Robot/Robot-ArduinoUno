#pragma once
#include <vector>
#include <Queue>
#include "Path.h"

typedef struct Checkpoint_s {
	int id;
	int left_id;
	int right_id;
	int front_id;
	int back_id;
}Checkpoint;

// https://www.geeksforgeeks.org/shortest-path-unweighted-graph/
// Modified bfs to store the parent of nodes along with the
// distance from source node
void bfs(std::vector<std::vector<int> >& graph, int S,
	std::vector<int>& par, std::vector<int>& dist)
{
	// queue to store the nodes in the order they are
	// visited
	std::queue<int> q;
	// Mark the distance of the source node as 0
	dist[S] = 0;
	// Push the source node to the queue
	q.push(S);

	// Iterate till the queue is not empty
	while (!q.empty()) {
		// Pop the node at the front of the queue
		int node = q.front();
		q.pop();

		// Explore all the neighbours of the current node
		for (int neighbour : graph[node]) {
			// Check if the neighbouring node is not visited
			if (dist[neighbour] == 1e9) {
				// Mark the current node as the parent of
				// the neighbouring node
				par[neighbour] = node;
				// Mark the distance of the neighbouring
				// node as distance of the current node + 1
				dist[neighbour] = dist[node] + 1;
				// Insert the neighbouring node to the queue
				q.push(neighbour);
			}
		}
	}
}


class Map
{
private:
	std::vector<Checkpoint> checkPoints;
	Checkpoint previousCheckPoint;
	Checkpoint nextCheckPoint;

public:
	Map(/* args */) {

	}
	~Map() {

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

	void setNextCheckPoint(Checkpoint newCheckpoint) {
		this->nextCheckPoint = newCheckpoint;
	}

	void setPreviousCheckPoint(Checkpoint newCheckpoint) {
		this->previousCheckPoint = newCheckpoint;
	}

	static Path findPath(Checkpoint _prevCheckpoint, Checkpoint nexCheckpoint_, Checkpoint destinationCheckpoint_) {
		Path path;
	}
};


