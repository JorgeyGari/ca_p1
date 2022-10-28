/* Source file containing functions common to SOA and AOS gauss methods */

#include "common_gauss.hpp"

int getm(int s, int t) {
    int m[5][5] = {{1, 4, 7, 4, 1}, {4, 16, 26, 16, 4}, {7, 26, 41, 26, 7}, {4, 16, 26, 16, 4}, {1, 4, 7, 4, 1}};
    return m[s+3][t+3];
}
