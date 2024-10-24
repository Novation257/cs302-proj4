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
    return 'X';
}

// Main Execution - given N (as a command argument), generates a .txt file containing an NxN board with random tiles
int main(int argc, char *argv[]) {
    // Error check for correct num of arguments
    if(argc != 3) {
        cerr << "Usage: ./generate_map n fileName" << endl;
        return 1;
    }

    // Seed rand with time
    srand(time(0));

    // Create new file with name argv[1] and convert argv[0] to an int
    ofstream fout(argv[2]);
    int n = stoi(argv[1]);

    // Fout tile types
    fout << "6" << endl;
    fout << "f 3" << endl;
    fout << "g 1" << endl;
    fout << "G 2" << endl;
    fout << "h 4" << endl;
    fout << "m 7" << endl;
    fout << "r 5" << endl;

    // Fout board size
    fout << n << " " << n << endl;

    // Fout randomised board
    for(int i = 0; i < n; i++) { // Create n lines...
        for(int j = 0; j < n; j++) { // ... with n tiles in each line
            fout << randTile();
        }
        fout << endl; // end of line
    }

    // Fout start (0,0) and end (n-1,n-1) coordinates
    fout << "0 0" << endl;
    fout << (n-1) << " " << (n-1) << endl;


    fout.close(); // Close file stream
    return 0;
}