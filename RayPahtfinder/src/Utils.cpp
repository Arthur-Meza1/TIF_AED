#include "Utils.h"


float GetDistance(raylib::Vector2 p1, raylib::Vector2 p2) {
    return p1.Distance(p2);
}


void ClearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}