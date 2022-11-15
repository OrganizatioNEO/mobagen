#ifndef PRIMS_H
#define PRIMS_H

#include "../MazeGeneratorBase.h"
#include "Point2D.h"
#include <map>
#include <vector>

using namespace std;

class Prims : public MazeGeneratorBase {
private:
	map<int, std::map<int, bool>> inMaze;
	vector<Point2D> frontier;
	Point2D RandomStartPoint(World* world);
    int nodesInMaze = 0;
    void Draw(World* world);
    void AddNeighbors(World* world, Point2D point);
	
	int RandInt(int Max, int Min);
    bool InFrontier(Point2D wanted);

public:
	Prims() = default;
	string GetName() override { return "Prims"; };
	bool Step(World* world) override;
	void Clear(World* world) override;
};

#endif  // PRIMS_H