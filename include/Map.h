#pragma once
//#include <vector>
#include <Queue>
#include <iostream>
#include "Path.h"

#define MAX_CHECKPOINT_ID 15000

#define CHECKPOINT_SIZE 20
typedef struct Checkpoint_s {
	int id;
	int left_id;
	int right_id;
	int front_id;
	int back_id;
}Checkpoint;

struct bfs_checkpoint {
	int id;
	int parent_id;
	int distance;
};

int findIndex_bfs_checkpoint_vector(int id, std::vector<bfs_checkpoint>& vec) {
	for (size_t i = 0; i < vec.size(); i++) {
		if (vec[i].id == id) {
			return i;
		}
	}
	return -1;
}

int findIndex_checkpoint_vector(int id, std::vector<Checkpoint>& vec) {
	for (size_t i = 0; i < vec.size(); i++) {
		if (vec[i].id == id) {
			return i;
		}
	}
	return -1;
}

// https://www.geeksforgeeks.org/shortest-path-unweighted-graph/
// Modified bfs to store the parent of nodes along with the
// distance from source node
int bfs(std::vector<Checkpoint>& graph, int source_node_id, std::vector<int>& par, std::vector<int>& dist)
{
	int index;
	int node_index, neighbor_index;
	// queue to store the nodes in the order they are
	// visited

	std::queue<int> q;
	// Mark the distance of the source node as 0
	index = findIndex_checkpoint_vector(source_node_id, graph);
	if (index < 0) {
		return -1;
	}

	dist[index] = 0;
	// Push the source node to the queue
	q.push(source_node_id);

	// Iterate till the queue is not empty
	while (!q.empty()) {
		// Pop the node at the front of the queue
		int node_id = q.front();
		q.pop();
		Checkpoint checkPoint;

		// Explore all the neighbours of the current node
		node_index = findIndex_checkpoint_vector(node_id, graph);
		if (node_index < 0) {
			return -1;
		}

		checkPoint = graph[node_index];

		if (checkPoint.front_id != 0)
		{
			neighbor_index = findIndex_checkpoint_vector(checkPoint.front_id, graph);
			if (neighbor_index < 0) {
				return -1;
			}
			// Check if the neighbouring node is not visited
			if (dist[neighbor_index] == MAX_CHECKPOINT_ID) {
				// Mark the current node as the parent of
				// the neighbouring node
				par[neighbor_index] = node_index;
				// Mark the distance of the neighbouring
				// node as distance of the current node + 1
				dist[neighbor_index] = dist[node_index] + 1;
				// Insert the neighbouring node to the queue
				q.push(checkPoint.front_id);
			}
		}

		if (checkPoint.back_id != 0)
		{
			neighbor_index = findIndex_checkpoint_vector(checkPoint.back_id, graph);
			if (neighbor_index < 0) {
				return -1;
			}
			// Check if the neighbouring node is not visited
			if (dist[neighbor_index] == MAX_CHECKPOINT_ID) {
				// Mark the current node as the parent of
				// the neighbouring node
				par[neighbor_index] = node_index;
				// Mark the distance of the neighbouring
				// node as distance of the current node + 1
				dist[neighbor_index] = dist[node_index] + 1;
				// Insert the neighbouring node to the queue
				q.push(checkPoint.back_id);
			}
		}

		if (checkPoint.left_id != 0)
		{
			neighbor_index = findIndex_checkpoint_vector(checkPoint.left_id, graph);
			if (neighbor_index < 0) {
				return -1;
			}
			// Check if the neighbouring node is not visited
			if (dist[neighbor_index] == MAX_CHECKPOINT_ID) {
				// Mark the current node as the parent of
				// the neighbouring node
				par[neighbor_index] = node_index;
				// Mark the distance of the neighbouring
				// node as distance of the current node + 1
				dist[neighbor_index] = dist[node_index] + 1;
				// Insert the neighbouring node to the queue
				q.push(checkPoint.left_id);
			}
		}

		if (checkPoint.right_id != 0)
		{
			neighbor_index = findIndex_checkpoint_vector(checkPoint.right_id, graph);
			if (neighbor_index < 0) {
				return -1;
			}
			// Check if the neighbouring node is not visited
			if (dist[neighbor_index] == MAX_CHECKPOINT_ID) {
				// Mark the current node as the parent of
				// the neighbouring node
				par[neighbor_index] = node_index;
				// Mark the distance of the neighbouring
				// node as distance of the current node + 1
				dist[neighbor_index] = dist[node_index] + 1;
				// Insert the neighbouring node to the queue
				q.push(checkPoint.right_id);
			}
		}
	}
}

// Function to print the shortest distance between source
// vertex and destination vertex
Path printShortestDistance(std::vector<Checkpoint>& graph, int source_node, int destination_node, int n_nodes)
{
	int destination_node_index;
	Path checkpointPath;
	PathCheckpoint pathCheckpoint;
	// par[] array stores the parent of nodes
	std::vector<int> par(n_nodes, -1);

	// dist[] array stores distance of nodes from S
	std::vector<int> dist(n_nodes, MAX_CHECKPOINT_ID);

	// function call to find the distance of all nodes and
	// their parent nodes
	bfs(graph, source_node, par, dist);

	destination_node_index = findIndex_checkpoint_vector(destination_node, graph);
	if (destination_node_index < 0) {
		return checkpointPath;
	}

	if (dist[destination_node_index] == MAX_CHECKPOINT_ID) {
		//std::cout << "Source and Destination are not connected";
		return checkpointPath;
	}

	// vector path stores the shortest path
	std::vector<int> path;
	int currentNode = destination_node_index;
	path.push_back(destination_node_index);
	while (par[currentNode] != -1) {
		path.push_back(par[currentNode]);
		currentNode = par[currentNode];
	}

	std::reverse(path.begin(), path.end());
	for (size_t i = 0; i < path.size(); i++) {

		pathCheckpoint.id = graph[path[i]].id;
		if ((i+1) < path.size()) {
			pathCheckpoint.next_checkpoint_id = graph[path[i + 1]].id;
		}
		else {
			pathCheckpoint.next_checkpoint_id = 0;
		}
		
		pathCheckpoint.front_id = graph[path[i]].front_id;
		pathCheckpoint.back_id = graph[path[i]].back_id;
		pathCheckpoint.left_id = graph[path[i]].left_id;
		pathCheckpoint.right_id = graph[path[i]].right_id;
		
		checkpointPath.addCheckPoint(pathCheckpoint);
		//std::cout << graph[path[i]].id << " ";
	}
	return checkpointPath;
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

	void setNextCheckPoint(int newCheckpoint_id) {
		this->nextCheckPoint = *(this->findCheckPointById(newCheckpoint_id));
	}

	void setPreviousCheckPoint(int newCheckpoint_id) {
		this->previousCheckPoint = *(this->findCheckPointById(newCheckpoint_id));
	}

	Path findPath(int destinationCheckpoint_id) {
		Path temp_path;
		PathCheckpoint pathCheckpoint;

		temp_path = printShortestDistance(this->checkPoints, this->nextCheckPoint.id, destinationCheckpoint_id, this->checkPoints.size());
		if (temp_path.getCheckPoints().size() <= 0) {
			return temp_path;
		}
		pathCheckpoint.id = this->previousCheckPoint.id;
		pathCheckpoint.next_checkpoint_id = this->nextCheckPoint.id;
		pathCheckpoint.front_id = this->previousCheckPoint.front_id;
		pathCheckpoint.back_id = this->previousCheckPoint.back_id;
		pathCheckpoint.left_id = this->previousCheckPoint.left_id;
		pathCheckpoint.right_id = this->previousCheckPoint.right_id;

		temp_path.setPreviousCheckPoint(pathCheckpoint);
		temp_path.setNextCheckPoint(*(temp_path.findCheckPointById(this->nextCheckPoint.id)));
		return temp_path;
	}
};


