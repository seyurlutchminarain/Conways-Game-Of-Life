#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

int row, col;
int timeSteps;

void printGrid(vector<vector<int>> grid)
{
    for (int i = 1; i < grid.size() - 1; ++i)
    {
        for (int j = 1; j < grid[i].size() - 1; ++j)
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

    auto start = chrono::high_resolution_clock::now();
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

        // cout << "Generation: " << iTime << endl;
        // printGrid(a);

        for (int i = 1; i <= row; ++i)
        {
            for (int j = 1; j <= col; ++j)
            {
                int nAlive = 0;

                for (int r = i - 1; r <= i + 1; ++r)
                {
                    for (int c = j - 1; c <= j + 1; ++c)
                    {
                        // cout << "Cell (" << r << "," << c << ") is " << a[r][c] << endl;
                        if (a[r][c] == 1 && (r != i || c != j)) // count the neighbors
                        {
                            nAlive++;
                        }
                    }
                }

                if (nAlive < 2 || nAlive > 3)
                {
                    b[i][j] = 0;
                }
                if (a[i][j] == 1 && (nAlive == 2 || nAlive == 3))
                {
                    b[i][j] = 1;
                }
                if (a[i][j] == 0 && nAlive == 3)
                {
                    b[i][j] = 1;
                }
            }
        }

        // cout << "Generation " << iTime + 1 << ": " << endl;
        // printGrid(b);
        copyGrid(a, b);
    }
    auto finish = chrono::high_resolution_clock::now();
    long long timeTaken = chrono::duration_cast<chrono::microseconds>(finish - start).count();

    cout << "Time taken for execution: " << timeTaken << " microseconds" << endl;

    return 0;
}
