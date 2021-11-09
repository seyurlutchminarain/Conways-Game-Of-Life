#include <iostream>
#include <mpi.h>
#include <vector>
#include <random>
#include <chrono>

using namespace std;

int main(int argc, char *argv[])
{

  MPI_Init(&argc, &argv);
  int mpisize;
  int mpirank;
  MPI_Comm_size(MPI_COMM_WORLD, &mpisize);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);

  auto mpiroot = 0;

  int nRows;
  int nCols;
  int nTime;

  if (mpirank == mpiroot)
  {
    if (argc != 4)
    {
      cerr << "incorrect number of cmd arguments, 4 expected" << endl;
      exit(1);
    }
    nRows = atoi(argv[1]);
    nCols = atoi(argv[2]);
    nTime = atoi(argv[3]);
    if (nRows < 1 || nCols < 1 || nTime < 1)
    {
      cerr << "invalid cmd arguments" << endl;
      exit(1);
    }
  }

  MPI_Bcast(&nRows, 1, MPI_INT, mpiroot, MPI_COMM_WORLD);
  MPI_Bcast(&nCols, 1, MPI_INT, mpiroot, MPI_COMM_WORLD);
  MPI_Bcast(&nTime, 1, MPI_INT, mpiroot, MPI_COMM_WORLD);

  auto nRowsLocal = nRows / mpisize;
  if (mpirank == mpisize - 1)
  {
    nRowsLocal += nRows % mpisize;
  }

  auto nRowsLocalWithGhost = nRowsLocal + 2;
  auto nColsWithGhost = nCols + 2;

  vector<vector<int>> currGrid(nRowsLocalWithGhost, vector<int>(nColsWithGhost, 0));
  vector<vector<int>> nextGrid(nRowsLocalWithGhost, vector<int>(nColsWithGhost, 0));

  for (auto iRow = 1; iRow <= nRowsLocal; iRow++) // account for ghost rows
  {
    for (auto iCol = 1; iCol <= nCols; iCol++)
    {
      currGrid[iRow][iCol] = rand() % 2; // setting DOA (0 or 1)
    }
  }

  auto upperNeighbor = (mpirank == 0) ? mpisize - 1 : mpirank - 1; // Basically an if else statement
  auto lowerNeighbor = (mpirank == mpisize - 1) ? 0 : mpirank + 1;

  const int ALIVE = 1;
  const int DEAD = 0;

  auto start = chrono::high_resolution_clock::now();
  // Time loop
  for (auto iTime = 0; iTime < nTime; ++iTime)
  {
    // send top row above
    MPI_Send(&currGrid[1][0], nColsWithGhost, MPI_INT, upperNeighbor, 0, MPI_COMM_WORLD);

    //send bottom row
    MPI_Send(&currGrid[nRowsLocal][0], nColsWithGhost, MPI_INT, lowerNeighbor, 0, MPI_COMM_WORLD);

    // receive bottom
    MPI_Recv(&currGrid[nRowsLocal + 1][0], nColsWithGhost, MPI_INT, lowerNeighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    //receive top
    MPI_Recv(&currGrid[0][0], nColsWithGhost, MPI_INT, upperNeighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // ghost columns
    for (auto iRow = 0; iRow < nRowsLocalWithGhost; ++iRow)
    {
      currGrid[iRow][0] = currGrid[iRow][nCols];
      currGrid[iRow][nCols + 1] = currGrid[iRow][1];
    }

    // display current grid on screen
    if (mpirank != mpiroot)
    {
      for (int iRow = 1; iRow <= nRowsLocal; ++iRow)
      {
        //MPI::COMM_WORLD.Send(&currGrid[iRow][1], nCols, MPI::INT, mpiroot, 0);
        MPI_Send(&currGrid[iRow][1], nCols, MPI_INT, mpiroot, 0, MPI_COMM_WORLD);
      }
    }

    if (mpirank == mpiroot)
    {

      for (auto sourceRank = 1; sourceRank < mpisize; ++sourceRank)
      {
        auto nRecv = nRows / mpisize;
        if (sourceRank == mpisize - 1)
        {
          nRecv += nRows % mpisize;
        }
        vector<int> buff(nCols, 0);
        for (auto iRecv = 0; iRecv < nRecv; ++iRecv)
        {
          //MPI::COMM_WORLD.Recv(&buff[0], nCols, MPI::INT, sourceRank, 0);
          MPI_Recv(&buff[0], nCols, MPI_INT, sourceRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
      }
    }

    // update grid
    for (auto iRow = 1; iRow <= nRowsLocal; ++iRow)
    {
      for (auto iCol = 1; iCol <= nCols; ++iCol)
      {
        auto nAliveNeighbors = 0;

        for (auto jRow = iRow - 1; jRow <= iRow + 1; ++jRow)
        {
          for (auto jCol = iCol - 1; jCol <= iCol + 1; ++jCol)
          {
            if ((jRow != iRow || jCol != iCol) && currGrid[jRow][jCol] == ALIVE)
            {
              ++nAliveNeighbors;
            }
          }
        }

        if (nAliveNeighbors < 2)
        {
          nextGrid[iRow][iCol] = DEAD;
        }
        if (currGrid[iRow][iCol] == ALIVE && (nAliveNeighbors == 2 || nAliveNeighbors == 3))
        {
          nextGrid[iRow][iCol] = ALIVE;
        }
        if (nAliveNeighbors > 3)
        {
          nextGrid[iRow][iCol] = DEAD;
        }
        if (currGrid[iRow][iCol] == DEAD && nAliveNeighbors == 3)
        {
          nextGrid[iRow][iCol] = ALIVE;
        }
      }
    }

    for (auto iRow = 1; iRow <= nRowsLocal; ++iRow)
    {
      for (auto iCol = 1; iCol <= nCols; ++iCol)
      {
        currGrid[iRow][iCol] = nextGrid[iRow][iCol];
      }
    }

  } // for iTime

  if (mpirank == 0)
  {
    auto finish = chrono::high_resolution_clock::now();
    long long timeTaken = chrono::duration_cast<chrono::microseconds>(finish - start).count();
    cout << "Time taken for execution: " << timeTaken << " microseconds" << endl;
  }
  MPI_Finalize();

  return 0;
}