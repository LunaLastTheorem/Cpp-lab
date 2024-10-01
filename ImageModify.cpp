#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

int processHeader(ifstream &file, int *widthptr, int *heightptr, int *maxVal){
    string line;

    getline(file, line); //ignore the first line
    getline(file, line); //height and lenght

    istringstream iss(line);
    iss >> *widthptr;
    iss >> *heightptr;

    getline(file, line);

    istringstream iss2(line);
    iss2 >> *maxVal;

    return 0;
}

void processBody(ifstream &file, int **matrix, int row, int col){
    string line;
    int i = 0;
    int j = 0;

    while(getline(file, line)){
        istringstream iss(line);
        while(j < col && iss >> matrix[i][j % col]){
            cout << "meow" << endl;
            j++;
        }
        i++;
        j = 0;
        cout << line << endl;
    }

    file.close();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: cpp_scanner <filename>" << endl;
        return 1;
    }

    int height;
    int width;
    int maxVal;

    int **matrix = new int*[height];

    for(int i = 0; i < height; i++){
        matrix[i] = new int[width];
    }

    string filename = argv[1];

    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    processHeader(file, &width, &height, &maxVal);
    processBody(file, matrix, height, width);

    cout << "InputFile: " << filename << endl;
    cout << "height: " << height << ", width: " << width << ", " << maxVal << endl;

    return 0;
}
