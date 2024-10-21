// generate_map.cpp

#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;


/*
Tile costs:
f 3
g 1
G 2
h 4
m 7
r 5
*/

// Returns a random tile char from the list above
char randTile() {
    int r = rand()%6;
    switch(r) {
        case 0: return 'f';
        case 1: return 'g';
        case 2: return 'G';
        case 3: return 'h';
        case 4: return 'm';
        case 5: return 'r';   
    }
}

// Main Execution - given N (as a command argument), generates a .txt file containing an NxN board with random tiles
int main(int argc, char *argv[]) {
    // Error check for correct num of arguments
    if(argc != 3) {
        cerr << "Usage: ./generate_map n fileName" << endl;
        return 1;
    }

    // Create new file with name argv[1] and convert argv[0] to an int
    ofstream fout(argv[1]);
    int n = stoi(argv[0]);

    for(int i = 0; i < n; i++) { // Create n lines...
        for(int j = 0; j < n; j++) { // ... with n tiles in each line
            fout << randTile();
        }
        fout << endl; // end of line
    }
    fout.close(); // Close file stream
    return 0;
}