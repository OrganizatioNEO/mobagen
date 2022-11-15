#include "../World.h"
#include "Prims.h"
#include <climits>

using namespace std;

Point2D Prims::RandomStartPoint(World* world) {
    int sideOver2 = world->GetSize() / 2;
    Point2D randomStartPoint = Point2D( RandInt(sideOver2, sideOver2 * -1), RandInt(sideOver2, sideOver2 * -1) );
    cout << "random start point (" << randomStartPoint.x << "," << randomStartPoint.y << ")" << endl;
    return randomStartPoint;
}

void Prims::Draw(World* world) {
    auto sideOver2 = world->GetSize() / 2;
    for (int i = -sideOver2; i <= sideOver2; i++) {
        for (int j = -sideOver2; j <= sideOver2; j++) {
            if (inMaze[i][j]) {
                world->SetNodeColor(Point2D(i,j), Color::Black);
            }
        }
    }
    for (int i = 0; i < frontier.size(); i++) {
        world->SetNodeColor(frontier[i], Color::Red.Dark());
    }
}

void Prims::AddNeighbors(World* world, Point2D point) {
    auto sideOver2 = world->GetSize() / 2;
    if (point.y > -sideOver2 && !inMaze[point.x][point.y - 1] && !InFrontier(Point2D(point.x, point.y - 1))) {  // north
        frontier.push_back(Point2D(point.x, point.y - 1));
    }
    if (point.x < sideOver2 && !inMaze[point.x + 1][point.y] && !InFrontier(Point2D(point.x + 1, point.y))) {  // east
        frontier.push_back(Point2D(point.x + 1, point.y));
    }
    if (point.y < sideOver2 && !inMaze[point.x][point.y + 1] && !InFrontier(Point2D(point.x, point.y + 1))) {  // south
        frontier.push_back(Point2D(point.x, point.y + 1));
    }
    if (point.x > -sideOver2 && !inMaze[point.x - 1][point.y] && !InFrontier(Point2D(point.x - 1, point.y))) {  // west
        frontier.push_back(Point2D(point.x - 1, point.y));
    }
}

int Prims::RandInt(int max, int min) {
    int randInt = rand();
    randInt = randInt % (max - min + 1);
    randInt += min;
    return randInt;
}

bool Prims::InFrontier(Point2D wanted) {
    return find(frontier.begin(), frontier.end(), wanted) != frontier.end();
}

bool Prims::Step(World* world) {
    auto sideOver2 = world->GetSize() / 2;
    //if the maze is empty
    if (nodesInMaze == 0) {
        srand(time(0));
        Point2D start = RandomStartPoint(world);
        inMaze[start.x][start.y] = true;
        nodesInMaze++;
        AddNeighbors(world, start);
    }
    //if not empty
    else if (nodesInMaze < world->GetSize() * world->GetSize()) {
        //pick random node from frontier
        int luckyAdd = RandInt(frontier.size()-1, 0);
        Point2D luckyNode = frontier[luckyAdd];
        frontier.erase(frontier.begin() + luckyAdd);
        nodesInMaze++;
        cout << "Adding Node (" << luckyNode.x << "," << luckyNode.y << ")" << endl;

        //break wall to connect random node
        bool notConnected = true;
        int randDir;
        while (notConnected) {
            randDir = RandInt(4, 1);
            switch (randDir) {
                case 1: //north
                    if (inMaze[luckyNode.x][luckyNode.y - 1] && luckyNode.y > -sideOver2) {
                        world->SetNorth(luckyNode, false);
                        notConnected = false;
                        cout << "Connecting from North" << endl;
                    }
                    break;
                case 2:  // east
                    if (inMaze[luckyNode.x + 1][luckyNode.y] && luckyNode.x < sideOver2) {
                        world->SetEast(luckyNode, false);
                        notConnected = false;
                        cout << "Connecting from East" << endl;
                    }
                    break;
                case 3:  // south
                    if (inMaze[luckyNode.x][luckyNode.y + 1] && luckyNode.y < sideOver2) {
                        world->SetSouth(luckyNode, false);
                        notConnected = false;
                        cout << "Connecting from South" << endl;
                    }
                    break;
                case 4:  // west
                    if (inMaze[luckyNode.x - 1][luckyNode.y] && luckyNode.x > -sideOver2) {
                        world->SetWest(luckyNode, false);
                        notConnected = false;
                        cout << "Connecting from West" << endl;
                    }
                    break;
                default:
                    cout << "Error connecting Node" << endl;
                    break;
          }
        }

        //consider random node in maze
        inMaze[luckyNode.x][luckyNode.y] = true;
        AddNeighbors(world, luckyNode);
    } else if (nodesInMaze == world->GetSize() * world->GetSize()) {
        
    }

    Draw(world);
    return true;
}

void Prims::Clear(World* world) {
    frontier.clear();
    nodesInMaze = 0;
    auto sideOver2 = world->GetSize() / 2;
    for (int i = -sideOver2; i <= sideOver2; i++) {
        for (int j = -sideOver2; j <= sideOver2; j++) {
            inMaze[i][j] = false;
        }
    }
}