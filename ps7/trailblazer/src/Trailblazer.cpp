// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: write comment header for this file; remove this comment

#include "queue.h"
#include "hashset.h"
#include "Trailblazer.h"
using namespace std;

static const double SUFFICIENT_DIFFERENCE = 0.2;

// Function prototypes (instead of adding to Trailblazer.h)
void visitNode(RoadNode* node, HashSet<string>& visitedNodes);
void enqueueNeighbors(Path& currPath, RoadNode* node, HashSet<string>& visitedNodes, const RoadGraph& graph);

void visitNode(RoadNode* node, HashSet<string>& visitedNodes){

    visitedNodes.add(node->nodeName());
    node->setColor(Color::GREEN);

}

void enqueueNeighbors(Path& currPath, RoadNode* node, HashSet<string>& visitedNodes, Queue<Path>& pathQ, const RoadGraph& graph){

    Path newPath;
    for (RoadNode* neighbor : graph.neighborsOf(node)){

        if (!visitedNodes.contains(neighbor->nodeName())){
            newPath = currPath;
            newPath.add(neighbor);
            neighbor->setColor(Color::YELLOW);
            pathQ.enqueue(newPath);
            newPath.clear();
        }
    }
}

Path breadthFirstSearch(const RoadGraph& graph, RoadNode* start, RoadNode* end) {


    Path pathVec;
    pathVec.add(start);

    Queue<Path> pathQ;
    pathQ.enqueue(pathVec);

    HashSet<string> visitedNodes;

    while (!pathQ.isEmpty() and !visitedNodes.contains(end->nodeName())){

        Path currPath = pathQ.dequeue();
        RoadNode* currLast = currPath[currPath.size() - 1];

        if (!visitedNodes.contains(currLast->nodeName())){

            visitNode(currLast, visitedNodes);

            if (currLast->nodeName() == end->nodeName()) {
                return currPath;
            }

            enqueueNeighbors(currPath, currLast, visitedNodes, pathQ, graph);

        }
    }

    return {};
}

Path dijkstrasAlgorithm(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    /* TODO: Delete the following lines and implement this function! */
    (void) graph;
    (void) start;
    (void) end;
    return {};
}

Path aStar(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    /* TODO: Delete the following lines and implement this function! */
    (void) graph;
    (void) start;
    (void) end;
    return {};
}

Path alternativeRoute(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    /* TODO: Delete the following lines and implement this function! */
    (void) graph;
    (void) start;
    (void) end;
    return {};
}

