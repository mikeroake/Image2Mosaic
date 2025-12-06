#ifndef CMATRIX_H
#define CMATRIX_H

#include <QVector>
#include <QList>

class CMatrix
{
public:
    CMatrix();
    bool            setSize(int numRows, int numCols);
    void            setZero();
    void            setElement(int row, int col, int value);
    int             getElement(int row, int col);
    int             getNumRows();
    int             getNumCols();

private:
    int             mNumRows;
    int             mNumCols;
    QVector<int>    mData;
};

#endif // CMATRIX_H
