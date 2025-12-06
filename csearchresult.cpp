#include "csearchresult.h"

CSearchResult::CSearchResult()
{

}

CSearchResult::CSearchResult( QColor color )
{
    mNearestColor = color;
    mNearestDist = 0;
    mNearestColrNum = 0;
    mSecondColor = color;
    mSecondDist = 0;
    mSecondColrNum = 0;
}
