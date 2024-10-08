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

enum Modification
{
    NEGATE = 0,
    QUANTIZE = 1,
    GRAY_SCALE = 2,
    FLIP_HORIZONTAL = 3
};

string modificationToString(Modification mod)
{
    switch (mod)
    {
    case NEGATE:
        return "-negate";
    case QUANTIZE:
        return "-quantize";
    case GRAY_SCALE:
        return "-gray-scale";
    case FLIP_HORIZONTAL:
        return "-flip-horizontal";
    default:
        return "-negate";
    }
}

Modification getModification(char ch)
{
    switch (ch)
    {
    case 'a':
        return NEGATE;
    case 'b':
        return QUANTIZE;
    case 'c':
        return GRAY_SCALE;
    case 'd':
        return FLIP_HORIZONTAL;
    default:
        return NEGATE;
    }
}

int processHeader(ifstream &file)
{
    string line;

    while (getline(file, line))
    {
        if (line[0] != '#' && !line.empty())
        {
            break;
        }
    }
    while (getline(file, line))
    {
        if (line[0] != '#' && !line.empty())
        {
            istringstream iss(line);
            iss >> width >> height;
            break;
        }
    }

    while (getline(file, line))
    {
        if (line[0] != '#' && !line.empty())
        {
            istringstream iss(line);
            iss >> maxVal;
            break;
        }
    }

    return 0;
}

// void rewriteOriginal(ofstream &writer, int **red, int **green, int **blue)
// {
//     writer << "P3" << endl;
//     writer << width << " " << height << endl;
//     writer << maxVal << endl;

//     for (int row = 0; row < height; row++) {
//         for (int col = 0; col < width; col++) {
//             // cout << "row: " << row << " col: " << col  << " " << red[row][col] << " " << green[row][col] << " " << blue[row][col]  << endl;
//             writer << red[row][col] << " "
//                    << green[row][col] << " "
//                    << blue[row][col] << " ";
//         }
//         writer << endl; // Add newline after each row
//     }
// }

void loadOriginal(ifstream &file, int **red, int **green, int **blue)
{
    string line;
    int r, g, b;
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            file >> r >> g >> b;
            red[row][col] = r;
            green[row][col] = g;
            blue[row][col] = b;
        }
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

void grayscale(ofstream &writer, int **red, int **green, int **blue)
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

void flipHorizontal(ofstream &writer, int **red, int **green, int **blue)
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

void processBody(ifstream &file, ofstream &writer, Modification mod)
{
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

    switch (mod)
    {
    case NEGATE:
        negateImg(writer, red, green, blue);
        break;
    case QUANTIZE:
        quantize(writer, red, green, blue);
        break;
    case GRAY_SCALE:
        grayscale(writer, red, green, blue);
        break;
    case FLIP_HORIZONTAL:
        flipHorizontal(writer, red, green, blue);
        break;
    default:
        negateImg(writer, red, green, blue);
    }

    for (int i = 0; i < height; i++)
    {
        delete[] red[i];
        delete[] green[i];
        delete[] blue[i];
    }
    delete[] red;
    delete[] green;
    delete[] blue;
}

int main(int argc, char *argv[])
{
    string filename;
    char input = 'f';

    cout << "Input file: ";
    cin >> filename;
    string fileNoExtension = filename.substr(0, filename.find(".ppm"));
    string fileExtesion = filename.substr(filename.find_last_of(".") + 1);
    if (fileExtesion != "ppm")
    {
        cerr << "Error file not supported: " << fileExtesion << endl;
        return 1;
    }

    while (input < 'a' || input > 'd')
    {
        cout << "Select modification: " << endl;
        cout << "(a) negate" << endl;
        cout << "(b) quantize" << endl;
        cout << "(c) gray scale" << endl;
        cout << "(d) flip horizontal" << endl;
        cin >> input;
    }

    Modification mod = getModification(input);
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    ofstream output(fileNoExtension + modificationToString(mod) + ".ppm");
    if (!output.is_open())
    {
        cerr << "Error opening file" << endl;
        return 1;
    }
    processBody(file, output, mod);

    cout << "Output file: " << fileNoExtension << modificationToString(mod) << ".ppm";

    output.close();
    file.close();

    return 0;
}
