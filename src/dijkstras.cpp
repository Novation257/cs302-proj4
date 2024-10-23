// dijsktras.cpp

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <climits>

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
  vector<vector<bool>> visited;       // Holds true when coord has been visited
  vector<vector<vector<int>>> paths;  // Each coord holds the cell IDs taken in the optimal path
  vector<vector<int>> distances;      // Distances from src
  multimap<int, int> frontier;        // {Distance from start, coordID}

  int n = board.size();

  // Initialize 2D vectors (paths and distances)
  distances.resize(n);
  paths.resize(n);
  visited.resize(n);
  for(int i = 0; i < n; i++) {
    distances[i].resize(n, 999999);
    paths[i].resize(n);
    visited[i].resize(n);
  }
  
  vector<int> startCoords = toCoords(srcID, n);
  distances[startCoords[0]][startCoords[1]] = 0;
  frontier.insert({0, srcID}); // Insert starting coord into multimap

  while(!frontier.empty()) { // While frontier isn't empty...
    // Get next coord from multimap
    multimap<int,int>::iterator curr = frontier.begin();
    // int currDist = curr->first;
    int currID   = curr->second;
    frontier.erase(curr);

    // Mark as visited
    int currX = toCoords(currID, n)[0];
    int currY = toCoords(currID, n)[1];
    vector<int> currPath = paths[currX][currY];

    if(visited[currX][currY] == false) {
      // Get curr's neighbors
      vector<vector<int>> edgesCoords;
      if(currX+1 < n  && visited[currX+1][currY] == false) edgesCoords.push_back({ currX+1,      currY   }); // Down
      if(currX-1 >= 0 && visited[currX-1][currY] == false) edgesCoords.push_back({ currX-1,      currY   }); // Up
      if(currY+1 < n  && visited[currX][currY+1] == false) edgesCoords.push_back({ currX  ,      currY+1 }); // Right
      if(currY-1 >= 0 && visited[currX][currY-1] == false) edgesCoords.push_back({ currX  ,      currY-1 }); // Left

      // cout << "Node " << currX << " " << currY << endl; // DEBUG - print curr

      // For each of curr's neighbors (up, down, left, right)...
      for(unsigned i = 0; i < edgesCoords.size(); i++) {
        int nextX = edgesCoords[i][0];
        int nextY = edgesCoords[i][1];
        if(visited[nextX][nextY] == false) { // If the node hasn't been visited already...
          if(distances[currX][currY] + board[currX][currY] < distances[nextX][nextY]) { // If current path is "cheaper"...

            // // DEBUG - printing changes to path
            // cout << "At " << nextX << " " << nextY << " from " << currX << " " << currY << "... ";
            // cout << "weight " << distances[currX][currY] + board[currX][currY] << " is less than " << distances[nextX][nextY] << endl;

            // Update distance (current distance from start + current node's weight)
            distances[nextX][nextY] = distances[currX][currY] + board[currX][currY];

            // Update path (current path + current node)
            paths[nextX][nextY] = currPath;
            paths[nextX][nextY].push_back(currID);
          }
          frontier.insert({distances[nextX][nextY], toID({nextX,nextY},board.size())}); // Insert next into multimap
        }
      }
    }
    visited[currX][currY] = true;
  }
  return paths;
}


int main(int argc, char *argv[]) {
  /* Input handling */
  // nTiles - number of different tiles on board
  int nTiles;
  cin >> nTiles;

  // tileChar1 tileCost1 (repeated nTiles times)
  map<char, int> tileCosts;
  for (int i = 0; i < nTiles; ++i) {
      char tileChar;
      int tileCost;
      cin >> tileChar >> tileCost;
      tileCosts[tileChar] = tileCost;
  }

  // mapX mapY - size of board
  int mapX, mapY;
  cin >> mapX >> mapY;

  //board matrix itself (convert board from char representation to the ints they represent)
  vector<vector<int>> boardConverted(mapX, vector<int>(mapY));
  for (int i = 0; i < mapX; ++i) {
      for (int j = 0; j < mapY; ++j) {
          char tileChar;
          cin >> tileChar;
          boardConverted[i][j] = tileCosts[tileChar];
      }
  }

  // startRow startCol - coordinate where pathfinding will start
  int startRow, startCol;
  cin >> startRow >> startCol;

  // endRow endCol - coordinate where pathfinding will end
  int endRow, endCol;
  cin >> endRow >> endCol;

  // Convert start and end positions to ID format
  int startID = toID({startRow, startCol}, mapX);
  int endID = toID({endRow, endCol}, mapX);

  /* Perform dijkstras algorithm and get optimal path from the start coord to the end coord */
  vector<vector<vector<int>>> optimalPaths = djikstrasAlgorithm(boardConverted, startID, endID);

  /* Output handling */
  // Total cost
  int totalCost = 0;
  for(unsigned i = 0; i < optimalPaths[endCol][endRow].size(); i++) {
    vector<int> coords = toCoords(optimalPaths[endCol][endRow][i], mapX);

    totalCost += boardConverted[coords[0]][coords[1]];
  }
  cout << totalCost << endl;

  // Path travelled
  for(unsigned i = 0; i < optimalPaths[endCol][endRow].size(); i++) {
    vector<int> coords = toCoords(optimalPaths[endCol][endRow][i], mapX);
    cout << coords[0] << " " << coords[1] << endl;
  }
  cout << endRow << " " << endCol << endl;
  return 0;
}
