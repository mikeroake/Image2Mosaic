// resizable matrix of integers
// indices are ZERO-based
#include "cmatrix.h"
#include <QDebug>

CMatrix::CMatrix()
{
    // constructs null matrix
    // must call setSize() before use
}

bool
CMatrix::setSize(int numRows, int numCols)
{

    if( numRows > 0 && numCols > 0 )
    {
        mData.resize( (numRows*numCols));
        mData.fill(99);
        mNumRows = numRows;
        mNumCols = numCols;
        return true;
    }
    else
    {
        return false;
    }
}

void
CMatrix::setZero()
{
    mData.fill(0);
}

void
CMatrix::setElement(int row, int col, int value)
{
    if( row >= 0 && col >= 0 )
    {
        mData.insert((row*mNumCols + col), value);
    }
}


int
CMatrix::getElement(int row, int col)
{
int value = 0;

    if( row >= 0 && col >= 0 )
    {
        value = mData.at(row*mNumCols + col);
    }
    return value;
}

int
CMatrix::getNumRows()
{
    return mNumRows;
}

int
CMatrix::getNumCols()
{
    return mNumCols;
}

