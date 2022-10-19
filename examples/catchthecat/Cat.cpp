#include "Cat.h"
#include "World.h"
#include <stdexcept>
#include <map>
#include <queue>

using namespace std;

Point2D Cat::Move(World* world) {
	Point2D cat = world->getCat();

	queue<Point2D> todo;
    map<int, map<int, bool>> visited;
    map<int, map<int, bool>> inQueue;
    map<int, map<int, Point2D>> cameFrom;

    todo.push(cat);
    visited[cat.x][cat.y] = true;
    inQueue[cat.x][cat.y] = true;
    cameFrom[cat.x][cat.y] = cat;

    Point2D nearestWin = Point2D(0, 0);
    bool winnable = false;

	while ( !todo.empty()) {
		Point2D head = todo.front();
        visited[head.x][head.y] = true;
        todo.pop();
        inQueue[head.x][head.y] = false;

		Point2D neighbors[6] = {World::NE(head), World::E(head),
                                World::SE(head), World::SW(head),
                                World::W(head),  World::NW(head)};

        for (int i = 0; i < 6; i++) {

            if (world->isValidPosition(neighbors[i])) {
                bool neighborBlocked = world->getContent(neighbors[i]);
                bool neighborVisited = visited[neighbors[i].x][neighbors[i].y];
                bool neighborInQueue = inQueue[neighbors[i].x][neighbors[i].y];

                if (!neighborBlocked && !neighborVisited && !neighborInQueue) {
                    todo.push(neighbors[i]);
                    inQueue[neighbors[i].x][neighbors[i].y] = true;
                    cameFrom[neighbors[i].x][neighbors[i].y] = head;
                    if (world->catWinsOnSpace(neighbors[i]) && !winnable) {
                        nearestWin = neighbors[i];
                        winnable = true;
                    }
                }

            }

        }

	}

    Point2D finger = Point2D(0, 0);
    if (winnable) { //if the cat can win
        finger = nearestWin;
        // follow the trail back to the tile that is next to the cat
        while (!world->catCanMoveToPosition(finger)) {
            finger = cameFrom[finger.x][finger.y];
        }
    }
    else { //if the cat cannot win
        if (world->catCanMoveToPosition(World::NE(cat))) { //wander around aimlessly
            finger = World::NE(cat);
        }
        else if (world->catCanMoveToPosition(World::E(cat))) {
            finger = World::E(cat);
        }
        else if (world->catCanMoveToPosition(World::SE(cat))) {
            finger = World::SE(cat);
        }
        else if (world->catCanMoveToPosition(World::SW(cat))) {
            finger = World::SW(cat);
        }
        else if (world->catCanMoveToPosition(World::W(cat))) {
            finger = World::W(cat);
        }
        else if (world->catCanMoveToPosition(World::NW(cat))) {
            finger = World::NW(cat);
        }
    }

    return finger;
}
