#pragma once
#include <utility> 
struct RiverConnection {
    int nodeId1; 
    int nodeId2; 

    RiverConnection(int id1, int id2) : nodeId1(id1), nodeId2(id2) {}
    bool connects(int n1, int n2) const {
        return ( (nodeId1 == n1 && nodeId2 == n2) || (nodeId1 == n2 && nodeId2 == n1) );
    }
};