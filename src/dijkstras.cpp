// dijsktras.cpp

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>

using namespace std;


// TODO: this code is an augmented form of BFS, which should be able to be modified into djikstras
// Code copied from challenge 4 - Returns true if there is a valid path from src to dest
bool isPath(unordered_map<char, vector<char>> &adjMap, const char &src, const char &dest) {
  vector<char> visited;
  queue<char> queue;
  queue.push(src);

  while(queue.size() != 0) {
    char curr = queue.front(); // Pop next node off queue
    queue.pop();

    visited.push_back(curr);      // Mark node as visited
    if(curr == dest) return true; // Return true if dest if found

    // Push curr's adjacent nodes to queue if they haven't been already visited
    unordered_map<char, vector<char>>::iterator currPos = adjMap.find(curr);
    for(unsigned i = 0; i < currPos->second.size(); i++) { // For every adjacent node...
      bool isVisited = false;
      for(unsigned j = 0; j < visited.size(); j++) { // Check against every node in visited vec
        if(currPos->second[i] == visited[j]) isVisited = true; // Don't push if they are equal
      }
      if(isVisited == false) queue.push(currPos->second[i]);
    }  
  }

  return false; // All possible nodes visited, dest is unreachable
}


// Main Execution
int main(int argc, char *argv[]) {
    /* Input handling */
    // nTiles - number of different tiles on board
    // tileChar1 tileCost1 (repeated nTiles times)
    // mapX mapY - size of board
    // Board matrix itself
    // startRow startCol - coordinate where pathfinding will start
    // endRow endCol - coordinate where pathfinding will end

    /* Perform dijkstras algorithm and get optimal path from the start coord to the end coord */
    /*
    From notes: https://web.eecs.utk.edu/~semrich/cs302-24/notes/11+12/
    For all nodes, set their backedges to NULL, their distances to -1, and their "visited" field to be false.
    Set node 0's distance to zero and put it on the multimap.
    While multimap isn't empty...
        Remove a node n from the front of the multimap and set its visited field to true.
        For each edge e from n to n2 such that n2 has not been visited. Let d be n's distance plus the weight of edge e. If n2's distance is -1, or if d is less than node n2's current distance:
            If n2 was in the multimap, remove it.
            Set n2's distance to d.
            Set n2's backedge to e.
            Insert n2 into the multimap, keyed on distance.
    */


    /* Output handling */
    // Total cost
    // Tiles travelled
    return 0;
}