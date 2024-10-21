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

// COMPLETED?
// Returns a 3d matrix with containing the optimal paths to every coord [X][Y][step]
vector<vector<vector<int>>> djikstrasAlgorithm(vector<vector<int>> board, const int &srcID, const int &destID) {
  vector<vector<int>> visited = board;  // Holds -1 When cell has been visited
  vector<vector<vector<int>>> paths;    // Each coord holds the cell IDs taken in the optimal path
  vector<vector<int>> distances;        // Distances from src
  multimap<int, int> frontier;          // {Distance from start, coordID}

  int n = board.size();
  frontier.insert(0, srcID); // Insert starting coord into multimap

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


int main(int argc, char *argv[]) {
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
    int startID = toID({startRow, startCol}, boardConverted.size());
    int endID = toID({endRow, endCol}, boardConverted.size());

    // Initialize distances and backedges for each node
    vector<vector<int>> distances(mapX, vector<int>(mapY, -1));  // Distance initialized to -1
    vector<vector<int>> backedges(mapX, vector<int>(mapY, -1));  // Backedges initialized to -1
    distances[startRow][startCol] = 0;

    // Multimap to manage the frontier of nodes (distance from start, node ID)
    multimap<int, int> frontier;
    frontier.insert(pair<int, int>(0, startID));  // Insert starting node with distance 0

    // While multimap isn't empty...
    while (!frontier.empty()) {
        // Remove a node n from the front of the multimap
        multimap<int, int>::iterator curr = frontier.begin();
        int currDist = curr->first;
        int currID = curr->second;
        frontier.erase(curr);

        // Get current coordinates from node ID
        vector<int> currCoords = toCoords(currID, boardConverted.size());
        int currX = currCoords[0];
        int currY = currCoords[1];

        // Set visited field to true (i.e., mark with a -1 in distances)
        distances[currX][currY] = -1;

        // For each edge e from n to n2 (neighbors: up, down, left, right)
        vector<vector<int>> edgesCoords(4, vector<int>(2));
        edgesCoords[0] = vector<int>{clamp(currX + 1, 0, mapX - 1), currY};
        edgesCoords[1] = vector<int>{clamp(currX - 1, 0, mapX - 1), currY};
        edgesCoords[2] = vector<int>{currX, clamp(currY + 1, 0, mapY - 1)};
        edgesCoords[3] = vector<int>{currX, clamp(currY - 1, 0, mapY - 1)};

        for (int i = 0; i < 4; ++i) {
            int nextX = edgesCoords[i][0];
            int nextY = edgesCoords[i][1];
            int nextID = toID({nextX, nextY}, boardConverted.size());

            // Let d be n's distance plus the weight of edge e
            int newDist = currDist + boardConverted[nextX][nextY];

            //if n2's distance is -1, or if d is less than node n2's current distance
            if (distances[nextX][nextY] == -1 || newDist < distances[nextX][nextY]) {
                //if n2 in the multimap, remove it
                multimap<int, int>::iterator it = frontier.find(distances[nextX][nextY]);
                if (it != frontier.end()) {
                    frontier.erase(it);
                }

                //sets n2's distance to d
                distances[nextX][nextY] = newDist;

                //sets n2's backedge to e
                backedges[nextX][nextY] = currID;

                //inserts n2 into the multimap, keyed on distance
                frontier.insert(pair<int, int>(newDist, nextID));
            }
        }
    }

    //traces back the path using backedges and print cost
    vector<int> path;
    int currID = endID;
    int totalCost = 0;

    //traces path back to the start using backedges
    while (currID != -1) {
        vector<int> coords = toCoords(currID, boardConverted.size());
        path.push_back(currID);
        currID = backedges[coords[0]][coords[1]];
    }

    reverse(path.begin(), path.end());
    for (int i = 0; i < path.size(); ++i) {
        vector<int> coords = toCoords(path[i], boardConverted.size());
        cout << coords[0] << " " << coords[1] << endl;
        totalCost += boardConverted[coords[0]][coords[1]];
    }

    cout << totalCost << endl;

    return 0;
}
