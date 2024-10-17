// dijsktras.cpp

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>

using namespace std;

// Changes a cell's expression from coordinates to the ID format
int toID(vector<int> cellCoords, int n) {
  return (cellCoords[0]*n + cellCoords[1]);
}

// Changes a cell's expression from the ID format to coordinates
vector<int> toCoords(int cellID, int n) {
  return {cellID/n, cellID%n};
}

// COMPLETED?
// Returns a 3d matrix with containing the optimal paths to every coord [X][Y][step]
vector<vector<vector<int>>> djikstrasAlgorithm(vector<vector<int>> board, const int &srcID, const int &destID) {
  vector<vector<int>> visited = board; // Holds -1 When cell has been visited
  vector<vector<vector<int>>> paths;    // Each coord holds the cell IDs taken in the optimal path
  vector<vector<int>> distances;        // Distances from src
  multimap<int, int> frontier;          // {Distance from start, coordID}

  frontier.insert(0, srcID); // Insert starting coord into multimap

  while(!frontier.empty()) { // While frontier isn't empty...
    // Get next coord from multimap
    multimap<int,int>::iterator curr = frontier.begin();
    int currDist = curr->first;
    int currID   = curr->second;
    frontier.erase(curr);

    // Mark as visited
    int currX = toCoords(currID, board.size())[0];
    int currY = toCoords(currID, board.size())[1];
    visited[currX][currY] = -1;

    // Get curr's neighbors
    vector<vector<int>> edgesCoords; // TODO: clamp these
    edgesCoords.push_back({ currX+1, currY   });
    edgesCoords.push_back({ currX-1, currY   });
    edgesCoords.push_back({ currX  , currY+1 });
    edgesCoords.push_back({ currX  , currY-1 });

    // For each of curr's neighbors (up, down, left, right)...
    for(unsigned i = 0; i < edgesCoords.size(); i++) {
      int nextX = edgesCoords[i][0];
      int nextY = edgesCoords[i][1];
      if(visited[nextX][nextY] != -1) { // If the node hasn't been visited already...
        distances[nextX][nextY] = distances[currX][currY] + board[currX][currY]; // Add distance of current tile to next's total
        paths[nextX][nextY].push_back(currID); // Add current's ID to next's path
        frontier.insert({distances[nextX][nextY], toID({nextX,nextY},board.size())}); // Insert next into multimap
      }
    }
  }
  return paths; 
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
    
    int startRow, startCol;
    int endRow  , endCol;

    /* Convert board from char representation to the ints they represent */
    vector<vector<int>> boardConverted;


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
   int startID = toID({startRow, startCol}, boardConverted.size());
   int endID   = toID({endRow  , endCol  }, boardConverted.size());
   vector<vector<vector<int>>> optimalPaths = djikstrasAlgorithm(boardConverted, startID, endID);


    /* Output handling */
    // Total cost
    // Tiles travelled
    return 0;
}