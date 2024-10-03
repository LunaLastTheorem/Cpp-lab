#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h>

using namespace std;

int height;
int width;
int maxVal;

int processHeader(ifstream &file) {
    string line;

    while (getline(file, line)) {
        if (line[0] != '#' && !line.empty()) {
            break;
        }
    }
    while (getline(file, line)) {
        if (line[0] != '#' && !line.empty()) {
            istringstream iss(line);
            iss >> width >> height;
            break;
        }
    }

    while (getline(file, line)) {
        if (line[0] != '#' && !line.empty()) {
            istringstream iss(line);
            iss >> maxVal;
            break;
        }
    }

    return 0;
}

void rewriteOriginal(ofstream &writer, int **red, int **green, int **blue)
{
    writer << "P3" << endl;
    writer << width << " " << height << endl;
    writer << maxVal << endl;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            // cout << red[row][col]  << " " << green[row][col] << " " << blue[row][col] << "  ";
            writer << red[row][col] << " "
                   << green[row][col] << " "
                   << blue[row][col] << " ";
        }
        writer << endl; // Add newline after each row
    }
}

void loadOriginal(ifstream &file, int **red, int **green, int **blue)
{
    for (int row = 0; row < height; row++)
    {
        string line;
        getline(file, line);
        istringstream iss(line);
        for (int col = 0; col < width; col++)
        {
            iss >> red[row][col];
            iss >> green[row][col];
            iss >> blue[row][col];

            // cout << red[row][col]  << " " << green[row][col] << " " << blue[row][col] << "  ";
        }
        // cout << endl;
    }
}

void negateImg(ofstream &writer, int **red, int **green, int **blue)
{
    writer << "P3" << endl;
    writer << width << " " << height << endl;
    writer << maxVal << endl;

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            writer << (maxVal - red[row][col]) << " "
                   << (maxVal - green[row][col]) << "  "
                   << (maxVal - blue[row][col]) << " ";
        }
        writer << endl;
    }
}

void quantize(ofstream &writer, int **red, int **green, int **blue)
{
    writer << "P3" << endl;
    writer << width << " " << height << endl;
    writer << maxVal << endl;

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            writer << ((red[row][col] > 127) ? 255 : 0) << " "
                   << ((green[row][col] > 127) ? 255 : 0) << "  "
                   << ((blue[row][col] > 127) ? 255 : 0) << " ";
        }
        writer << endl;
    }
}

void grayscale(ofstream &writer, int **red,  int **green, int **blue)
{
    writer << "P3" << endl;
    writer << width << " " << height << endl;
    writer << maxVal << endl;
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            int sum = red[row][col] + green[row][col] + blue[row][col];
            int avg = sum / 3;
            writer << avg << " "
                   << avg << " "
                   << avg << "  ";
        }
        writer << endl;
    }
}

void flipHorizontal(ofstream &writer, int **red, int **green,  int **blue)
{
    writer << "P3" << endl;
    writer << width << " " << height << endl;
    writer << maxVal << endl;

    for (int row = 0; row < height; row++)
    {
        for (int col = width - 1; col >= 0; col--)
        {
            writer << red[row][col] << " "
                   << green[row][col] << "  "
                   << blue[row][col] << " ";
        }
        writer << endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: ImageModify <filename>" << endl;
        return 1;
    }

    string filename = argv[1];

    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    processHeader(file);

    int **red = new int *[height];
    int **green = new int *[height];
    int **blue = new int *[height];

    for (int i = 0; i < height; i++)
    {
        red[i] = new int[width];
        green[i] = new int[width];
        blue[i] = new int[width];
    }

    loadOriginal(file, red, green, blue);

    ofstream output(filename + "_flipped.ppm");

    if (!output.is_open())
    {
        cerr << "error opening file" << endl;
        return 1;
    }

    // rewriteOriginal(output, red, green, blue);
    // negateImg(output, red, green, blue);
    // quantize(output, red, green, blue);
    // grayscale(output, red, green, blue);
    flipHorizontal(output, red, green, blue);

    output.close();
    file.close();

    // Free allocated memory
    for (int i = 0; i < height; i++)
    {
        delete[] red[i];
        delete[] green[i];
        delete[] blue[i];
    }
    delete[] red;
    delete[] green;
    delete[] blue;

    cout << "InputFile: " << filename << endl;
    cout << "height: " << height << ", width: " << width << ", " << maxVal << endl;

    return 0;
}
