// dijsktras.cpp

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>

using namespace std;

// Returns the closest bound if val is out of bounds; otherwise returns val itself
int clamp(int val, int maxVal) {
  return min(max(val, 0), maxVal);
}

// Changes a cell's expression from coordinates to the ID format
int toID(vector<int> cellCoords, int n) {
  return (cellCoords[0]*n + cellCoords[1]);
}

// Changes a cell's expression from the ID format to coordinates
vector<int> toCoords(int cellID, int n) {
  return {cellID/n, cellID%n};
}

// Converts an int to a char (0=a, 1=b, etc.)
char itoc(int num) {
  return (num+65);
}

// COMPLETED?
// Returns a 3d matrix with containing the optimal paths to every coord [X][Y][step]
vector<vector<vector<int>>> djikstrasAlgorithm(vector<vector<int>> board, const int &srcID, const int &destID) {
  vector<vector<int>> visited = board;  // Holds -1 When cell has been visited
  vector<vector<vector<int>>> paths;    // Each coord holds the cell IDs taken in the optimal path
  vector<vector<int>> distances;        // Distances from src
  multimap<int, int> frontier;          // {Distance from start, coordID}

  int n = board.size();
  frontier.insert({0, srcID}); // Insert starting coord into multimap

  while(!frontier.empty()) { // While frontier isn't empty...
    // Get next coord from multimap
    multimap<int,int>::iterator curr = frontier.begin();
    int currDist = curr->first;
    int currID   = curr->second;
    frontier.erase(curr);

    // Mark as visited
    int currX = toCoords(currID, n)[0];
    int currY = toCoords(currID, n)[1];
    visited[currX][currY] = -1;

    // Get curr's neighbors
    // If the neighbor is out of bounds (ie (-1, 3)) curr will be pushed, which is handled below
    vector<vector<int>> edgesCoords;
    edgesCoords.push_back({ clamp(currX+1, n),      currY       });
    edgesCoords.push_back({ clamp(currX-1, n),      currY       });
    edgesCoords.push_back({       currX  ,    clamp(currY+1, n) });
    edgesCoords.push_back({       currX  ,    clamp(currY-1, n) });

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
    int totalCost = 0;
    for(unsigned i = 0; i < optimalPaths[startID][endID].size(); i++) {
      totalCost += optimalPaths[startID][endID][i];
    }
    cout << totalCost << endl;

    // Path travelled
    for(unsigned i = 0; i < optimalPaths[startID][endID].size(); i++) {
      int id = optimalPaths[startID][endID][i]; // get id
      vector<int> coords = toCoords(id, boardConverted.size()); // convert id to int coords
      cout << toupper(itoc(coords[0])) << toupper(itoc(coords[1])) << endl; // convert ints to chars and output to console
    }

    // // ASSUMES OUTPUT IS ADJLIST - useless right now but keeping in just in case
    // // Total cost
    // int totalCost = 0;
    // for(unsigned i = 0; i < optimalPaths.size(); i++) {
    //   for(unsigned j = i; j < optimalPaths[i].size(); j++) { // Splits 2d vec diagonally \ and only reads top-right half; avoids reading edges twice
    //     if(optimalPaths[i][j] != 0) totalCost += optimalPaths[i][j]; // Add edge cost to total if [i][j] is a valid edge
    //   }
    // }

    return 0;
}