/*
 * This sourcecode file implements three different graph search algorithms:
 * [1] Breadth-First Search (BFS) using a standard queue data structure.
 * [2] Dijkstra's algorithm using a priority queue data structure.
 * [3] A* algorithm using a priority queue data structure and a heuristic metric
 *     composed of the minimum feasible time to traverse the straight-line distance
 *     between the current path endpoint and the desired final endpoint.
*/

#include <cfloat>

#include "pqueue.h"
#include "queue.h"
#include "hashset.h"
#include "Trailblazer.h"
using namespace std;

static const double SUFFICIENT_DIFFERENCE = 0.2;

// Function prototypes (instead of adding to Trailblazer.h)
void visitNode(RoadNode* node, HashSet<string>& visitedNodes);
void enqueueNeighbors(Queue<Path>& pathQ, const Path& currPath, RoadNode* node,
                      const HashSet<string>& visitedNodes, const RoadGraph& graph);
void enqueueNeighbors(PriorityQueue<Path>& pathQ, double& currCost, RoadNode* end, const bool useAstar, const Path& currPath,
                      RoadNode* node, const HashSet<string>& visitedNodes, const RoadGraph& graph);
double computeCost(const Path& currPath, const RoadGraph& graph);
double computeHeuristic(RoadNode* start, RoadNode* end, const RoadGraph& graph);
bool aStarHelper(const RoadGraph& graph, RoadNode* start, RoadNode* end, Path& solnPath);
HashSet<RoadNode*> createNodeHashSet(Path& path);
double computePathCost(Path& path, const RoadGraph& graph);


/*
 * Helper function for visiting a RoadNode in the trailblazing algorithms.
*/
void visitNode(RoadNode* node, HashSet<string>& visitedNodes){

    visitedNodes.add(node->nodeName());
    node->setColor(Color::GREEN);

}

/*
 * Helper function for enqueueing neighbors of a node in preparation for path exploration.
 * Note that this function is overloaded to be used with a Queue data structure for BFS, or
 * a PriorityQueue data structure for Dijkstra and A*.
*/
void enqueueNeighbors(Queue<Path>& pathQ, const Path& currPath, RoadNode* node,
                      const HashSet<string>& visitedNodes, const RoadGraph& graph){

    Path newPath;
    for (RoadNode* neighbor : graph.neighborsOf(node)){

        if (!visitedNodes.contains(neighbor->nodeName())){
            newPath = currPath;
            newPath.add(neighbor);
            pathQ.enqueue(newPath);
            neighbor->setColor(Color::YELLOW);
            newPath.clear();
        }
    }
}

void enqueueNeighbors(PriorityQueue<Path>& pathQ, double& currCost, RoadNode* end, const bool useAstar,const Path& currPath,
                      RoadNode* node, RoadEdge* neglectEdge, const HashSet<string>& visitedNodes, const RoadGraph& graph){

    Path newPath;
    double updatedCost;
    double heuristicCost;
    // Computing the prior heuristic cost so that we can remove it from the new cost computations
    double priorHeuristicCost = useAstar * computeHeuristic(node, end, graph);

    for (RoadNode* neighbor : graph.neighborsOf(node)){

        if (neglectEdge != nullptr and node->nodeName() == neglectEdge->from()->nodeName() and
            neighbor->nodeName() == neglectEdge->to()->nodeName()){
            continue;
        }

        if (!visitedNodes.contains(neighbor->nodeName())){

            newPath = currPath;
            newPath.add(neighbor);

            // Taking current cost, removing the prior heuristic cost and then adding the new edge cost
            updatedCost = currCost - priorHeuristicCost + computeCost(newPath, graph);
            heuristicCost = useAstar * computeHeuristic(newPath[newPath.size() - 1], end, graph);

            // Enqueuing with updated cost and the new hueristic cost
            pathQ.enqueue(newPath, updatedCost + heuristicCost);
            neighbor->setColor(Color::YELLOW);

            newPath.clear();
        }
    }
}

/*
 * Helper function for computing the edge cost assoicated with the newest to entries to a path.
*/
double computeCost(const Path& currPath, const RoadGraph& graph){

    size_t endIdx = currPath.size() - 1;
    double cost = graph.edgeBetween(currPath[endIdx - 1], currPath[endIdx])->cost();

    return cost;
}

/*
 * Helper function for computing the heuristic cost from the last node in the current path
 * to the desired end node.
*/
double computeHeuristic(RoadNode* start, RoadNode* end, const RoadGraph& graph){

    return graph.crowFlyDistanceBetween(start, end) / graph.maxRoadSpeed();
}

/*
 * Implementing a Breadth-First Search (BFS) using a standard queue data structure.
*/
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

            enqueueNeighbors(pathQ, currPath, currLast, visitedNodes, graph);

        }
    }

    return {};
}

/*
 * Implementing Dijkstra's algorithm using a priority queue data structure.
*/
Path dijkstrasAlgorithm(const RoadGraph& graph, RoadNode* start, RoadNode* end) {

    Path pathVec;
    pathVec.add(start);

    PriorityQueue<Path> pathQ;
    pathQ.enqueue(pathVec, 0.0);

    HashSet<string> visitedNodes;

    while (!pathQ.isEmpty() and !visitedNodes.contains(end->nodeName())){

        double currCost = pathQ.peekPriority();
        Path currPath = pathQ.dequeue();
        RoadNode* currLast = currPath[currPath.size() - 1];

        if (!visitedNodes.contains(currLast->nodeName())){

            visitNode(currLast, visitedNodes);

            if (currLast->nodeName() == end->nodeName()) {
                return currPath;
            }

            bool useAstar = false;
            enqueueNeighbors(pathQ, currCost, end, useAstar, currPath, currLast, {}, visitedNodes, graph);
        }
    }

    return {};
}

/*
 * Implementing the A* algorithm using a priority queue data structure with a heuristic for
 * underestimating the time-to-traverse cost computed using the straightline distance between
 * two nodes divided by the maximum allowable speed on the graph.  Note that there is the
 * option for ignoring a specified edge in this implementation.
*/
bool aStarHelper(const RoadGraph& graph, RoadNode* start, RoadNode* end, Path& solnPath, RoadEdge* neglectEdge) {

    Path pathVec;
    pathVec.add(start);

    PriorityQueue<Path> pathQ;
    pathQ.enqueue(pathVec, computeHeuristic(start, end, graph));

    HashSet<string> visitedNodes;

    while (!pathQ.isEmpty() and !visitedNodes.contains(end->nodeName())){

        double currCost = pathQ.peekPriority();
        Path currPath = pathQ.dequeue();
        size_t endIdx = currPath.size() - 1;

        RoadNode* currLast = currPath[endIdx];

        if (!visitedNodes.contains(currLast->nodeName())){

            visitNode(currLast, visitedNodes);

            if (currLast->nodeName() == end->nodeName()) {
                solnPath = currPath;
                return true;
            }

            bool useAstar = true;
            enqueueNeighbors(pathQ, currCost, end, useAstar, currPath, currLast, neglectEdge, visitedNodes, graph);
        }
    }

    return false;
}

/*
 * Nominal A* function, which calls the A* helper function with no neglected edges
*/
Path aStar(const RoadGraph&graph, RoadNode* start, RoadNode* end) {

    Path solnPath;
    bool solnFound = aStarHelper(graph, start, end, solnPath, {});

    if (solnFound){
        return solnPath;
    }
    else{
        return {};
    }
}

/*
 * Helper function for creating a Hashset of nodes contained in a path
*/
HashSet<RoadNode*> createNodeHashSet(Path& path){

    HashSet<RoadNode*> nodeSet;
    for (int ii = 0; ii < path.size() - 1; ii++){
        nodeSet.add(path[ii]);
    }
    return nodeSet;
}

/*
 * Helper function for computing the cost of a path
*/
double computePathCost(Path& path, const RoadGraph& graph){

    double cost = 0.0;
    for (int ii = 0; ii < path.size() - 2; ii++){
        cost += graph.edgeBetween(path[ii], path[ii + 1])->cost();
    }
    return cost;
}

/*
 * Function for computing an alterantive route that differs from the best route by a
 * fixed threshold.  The alternative route is the minimum cost of all candidates.
*/

Path alternativeRoute(const RoadGraph& graph, RoadNode* start, RoadNode* end) {

    Path bestPath = aStar(graph, start, end);

    HashSet<RoadNode*> bestNodeSet = createNodeHashSet(bestPath);

    RoadEdge* currNeglectEdge;

    Path altSolnPath;
    HashSet<RoadNode*> altNodeSet;

    double currAltSolnPathCost = DBL_MAX;

    Path bestAltSolnPath = {};

    for (int ii = 0; ii < bestPath.size() - 2; ii++){
        currNeglectEdge = graph.edgeBetween(bestPath[ii], bestPath[ii + 1]);
        bool foundAltSoln = aStarHelper(graph, start, end, altSolnPath, currNeglectEdge);
        if (foundAltSoln){
            altNodeSet = createNodeHashSet(altSolnPath);
            double diffScore = (double)(bestNodeSet - altNodeSet).size() / bestNodeSet.size();

            if (diffScore > SUFFICIENT_DIFFERENCE){
                double altPathSolnCost = computePathCost(altSolnPath, graph);
                if (altPathSolnCost < currAltSolnPathCost){
                    currAltSolnPathCost = altPathSolnCost;
                    bestAltSolnPath = altSolnPath;
                }
            }
        }
        altSolnPath.clear();
    }

    return bestAltSolnPath;
}

