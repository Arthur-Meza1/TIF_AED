#pragma once
#include "Graph.h"
#include "Estructuras_datos/list.h"
#include "Estructuras_datos/MyVector.h"

// DFS search for path between two nodes in a graph
SimpleList<int> findPathDFS(const Graph& graph, int startNodeId, int endNodeId);

// Helper for recursive DFS
bool dfsRecursive(const Graph& graph, int currentId, int endNodeId, SimpleList<int>& path, MyVector<bool>& visited);
