#ifndef CSEARCHRESULT_H
#define CSEARCHRESULT_H

#include <QColor>


class CSearchResult
{
public:
    CSearchResult();
    CSearchResult( QColor color );

    QColor  mNearestColor;
    float   mNearestDist;
    int     mNearestColrNum;
    QColor  mSecondColor;
    float   mSecondDist;
    int     mSecondColrNum;
};

#endif // CSEARCHRESULT_H
