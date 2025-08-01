#include "SearchAlgorithm.hpp"
#include "MyVector.hpp"
#include "SimpleMap.hpp"
#include "Utils.hpp"
#include "pair.hpp"
#include "queue.hpp"
#include <algorithm>
#include <limits>
#include <iostream>

SimpleList<int> AStar_Algorithm::findPath(const Graph& graph, int startNodeId, int endNodeId) const {
    int numNodes = graph.getNumNodes();

    if (startNodeId < 0 || startNodeId >= numNodes || endNodeId < 0 || endNodeId >= numNodes) {
        return {};
    }

    MyVector<float> gScore(numNodes, std::numeric_limits<float>::infinity());
    MyVector<float> fScore(numNodes, std::numeric_limits<float>::infinity());
    MyVector<bool> closedSet(numNodes, false);
    SimpleMap<int, int> cameFrom;

    PriorityQueue<Pair<float, int>> openSet;

    gScore[startNodeId] = 0;
    fScore[startNodeId] = calculateHeuristic(graph, startNodeId, endNodeId);
    openSet.push({fScore[startNodeId], startNodeId});

    while (!openSet.empty()) {
        int currentId = openSet.top().second;
        openSet.pop();

        if (currentId == endNodeId) {
            return reconstructPath(cameFrom, currentId);
        }

        if (closedSet[currentId]) {
            continue;
        }
        closedSet[currentId] = true;

        const raylib::Vector2 currentPos = graph.getNode(currentId).position;
        const auto& neighbors = graph.getAdjacentNodes(currentId);

        for (const auto& neighborPair : neighbors) {
            int neighborId = neighborPair.first;
            float edgeCost = neighborPair.second;

            if (closedSet[neighborId]) {
                continue;
            }

            const raylib::Vector2 neighborPos = graph.getNode(neighborId).position;
            bool isBlocked = false;
            for (const auto& obstacle : graph.getObstacles()) {
                if (Utils::CheckLineRectangleCollision(currentPos, neighborPos, obstacle.rect)) {
                    isBlocked = true;
                    break;
                }
            }
            if (isBlocked) continue;

            float tentative_gScore = gScore[currentId] + edgeCost;

            if (tentative_gScore < gScore[neighborId]) {
                cameFrom.set(neighborId, currentId);
                gScore[neighborId] = tentative_gScore;
                fScore[neighborId] = tentative_gScore + calculateHeuristic(graph, neighborId, endNodeId);
                openSet.push({fScore[neighborId], neighborId});
            }
        }
    }
    return {};
}

float AStar_Algorithm::calculateHeuristic(const Graph& graph, int nodeId1, int nodeId2) const {
    const Node& n1 = graph.getNode(nodeId1);
    const Node& n2 = graph.getNode(nodeId2);
    return Vector2Distance(n1.position, n2.position);
}

SimpleList<int> AStar_Algorithm::reconstructPath(const SimpleMap<int, int>& cameFrom, int currentId) const {
    MyVector<int> temp;
    while (true) {
        temp.push_back(currentId);
        if (!cameFrom.contains(currentId)) break;
        int prev = cameFrom.get(currentId);
        if (prev == -1) break;
        currentId = prev;
    }

    SimpleList<int> totalPath;
    for (int i = temp.size() - 1; i >= 0; --i) {
        totalPath.push_back(temp[i]);
    }

    return totalPath;
}
