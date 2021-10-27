#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int row, col;
int timeSteps;

//returns the count of alive neighbours
int countNeighbors(vector<vector<int>> a, int r, int c)
{
    int count = 0;

    for (int i = r - 1; i <= r + 1; i++) // for rows: from left to right
    {
        for (int j = c - 1; j <= c + 1; j++) // for cols: from up to down -> so all 8 neighbors explored?
        {

            if ((i == r && j == c) || (i < 0 || j < 0) || (i >= row || j >= col)) // if we are in valid spaces check the cell if alive or dead
            {                                                                     // but dont count cell (r,c) -> it should not affect our state
                continue;
            }
            if (a[i][j] == 1)
            {
                count++;
            }
        }
    }

    return count;
}

void printGrid(vector<vector<int>> grid)
{
    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[i].size(); ++j)
        {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}

void copyGrid(vector<vector<int>> &a, vector<vector<int>> b) // this is to copy the nextGen grid to the currGen grid for the next iteration
{
    for (int i = 0; i < a.size(); ++i)
    {
        for (int j = 0; j < a[i].size(); ++j)
        {
            a[i][j] = b[i][j];
        }
    }
}
int main(int argc, char *argv[])
{
    row = atoi(argv[1]);
    col = atoi(argv[2]);
    timeSteps = atoi(argv[3]);

    vector<vector<int>> a(row + 2, vector<int>(col + 2, 0));
    vector<vector<int>> b(row + 2, vector<int>(col + 2, 0));

    // int a[row][col], b[row][col];
    int i, j;

    //generate matrix canvas with random values (live and dead cells)
    for (i = 1; i <= row; i++)
    {
        for (j = 1; j <= col; j++)
        {
            a[i][j] = (rand() % 2);
        }
    }

    for (int iTime = 0; iTime < timeSteps; ++iTime)
    {
        for (int r = 0; r <= row; ++r) // adding the ghost cols
        {
            a[r][0] = a[r][col];
            a[r][col + 1] = a[r][1];
        }

        for (int c = 1; c <= col; ++c) // add ghost rows
        {
            a[0][c] = a[row][c];
            a[row + 1][c] = a[1][c];
        }

        a[0][0] = a[row][col]; // setting the edge elements
        a[0][col + 1] = a[row][1];
        a[row + 1][0] = a[1][col];
        a[row + 1][col + 1] = a[1][1];

        cout << "Generation: " << iTime << endl;
        printGrid(a);

        for (int i = 1; i <= row; ++i)
        {
            for (int j = 1; j <= col; ++j)
            {
                if (a[i][j] == 1 && (countNeighbors(a, i, j) == 3 || countNeighbors(a, i, j) == 2)) // first rule
                {

                    cout << "Cell (" << i << "," << j << ") has " << countNeighbors(a, i, j) << " neighbors alive and is alive -> should be 1" << endl;
                    b[i][j] = 1; // cell is alive next gen
                }
                else if (a[i][j] == 1 && (countNeighbors(a, i, j) > 3 || countNeighbors(a, i, j) < 2)) // cell must die
                {

                    cout << "Cell (" << i << "," << j << ") has " << countNeighbors(a, i, j) << " neighbors alive and is alive -> should be 0" << endl;
                    b[i][j] = 0;
                }
                else if (a[i][j] == 0 && countNeighbors(a, i, j) == 3)
                {

                    cout << "Cell (" << i << "," << j << ") has " << countNeighbors(a, i, j) << " neighbors alive and is dead -> should be 1" << endl;
                    b[i][j] = 1;
                }
            }
        }

        // cout << "Generation " << iTime + 1 << ": " << endl;
        // printGrid(b);
        copyGrid(a, b);
    }

    return 0;
}
