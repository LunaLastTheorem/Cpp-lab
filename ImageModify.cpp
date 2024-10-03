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

int processHeader(ifstream &file)
{
    string line;

    getline(file, line); // ignore the first line
    getline(file, line); // height and lenght

    istringstream iss(line);
    iss >> width;
    iss >> height;

    getline(file, line);

    istringstream iss2(line);
    iss2 >> maxVal;

    return 0;
}

void loadOriginal(ifstream &file, int **red, int **blue, int **green)
{
    for (int row = 0; row < height; row++)
    {
        string line;
        getline(file, line);
        istringstream iss(line);
        for (int col = 0; col < width; col++)
        {
            iss >> red[row][col];
            iss >> blue[row][col];
            iss >> green[row][col];
            // cout << red[row][col]  << blue[row][col] << green[row][col] << endl;
        }
    }
}

void negateImg(ofstream &writer, int **red, int **blue, int **green)
{
    writer << "P3" << endl;
    writer << width << " " << height << endl;
    writer << maxVal << endl;

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            writer << maxVal - red[row][col] << " "
                   << maxVal - blue[row][col] << " "
                   << maxVal - green[row][col] << "  ";
        }
        writer << endl;
    }
}

void quantize(ofstream &writer, int **red, int **blue, int **green)
{
    writer << "P3" << endl;
    writer << width << " " << height << endl;
    writer << maxVal << endl;

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            writer << ((red[row][col] > 127) ? 255 : 0) << " "
                   << ((blue[row][col] > 127) ? 255 : 0) << " "
                   << ((green[row][col] > 127) ? 255 : 0) << "  ";
        }
        writer << endl;
    }
}

void grayscale(ofstream &writer, int **red, int **blue, int **green)
{
    writer << "P3" << endl;
    writer << width << " " << height << endl;
    writer << maxVal << endl;
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            int sum = red[row][col] + blue[row][col] + green[row][col];
            int avg = sum / 3;
            writer << avg << " "
                   << avg << " "
                   << avg << "  ";
        }
        writer << endl;
    }
}

void flipHorizontal(ofstream &writer, int **red, int **blue, int **green)
{
    writer << "P3" << endl;
    writer << width << " " << height << endl;
    writer << maxVal << endl;

    for (int row = 0; row < height; row++)
    {
        for (int col = width - 1; col >= 0; col--)
        {
            writer << red[row][col] << " "
                   << blue[row][col] << " "
                   << green[row][col] << "  ";
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
    int **blue = new int *[height];
    int **green = new int *[height];

    for (int i = 0; i < height; i++)
    {
        red[i] = new int[width];
        blue[i] = new int[width];
        green[i] = new int[width];
    }

    loadOriginal(file, red, blue, green);

    ofstream output(filename + "_flipped.ppm");

    if (!output.is_open())
    {
        cerr << "error opening file" << endl;
        return 1;
    }

    // negateImg(output, red, blue, green);
    // quantize(output, red, blue, green);
    // grayscale(output, red, blue, green);
    // flipHorizontal(output, red, blue, green);

    output.close();
    file.close();

    // Free allocated memory
    for (int i = 0; i < height; i++)
    {
        delete[] red[i];
        delete[] blue[i];
        delete[] green[i];
    }
    delete[] red;
    delete[] blue;
    delete[] green;

    cout << "InputFile: " << filename << endl;
    cout << "height: " << height << ", width: " << width << ", " << maxVal << endl;

    return 0;
}
