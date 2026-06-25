#ifndef CMOSAIC_H
#define CMOSAIC_H

#include <QObject>
#include <QRandomGenerator>
#include "cmatrix.h"
#include "cmosaicparams.h"
#include "cpalette.h"
#include "csearchresult.h"
#include "./ui_mainwindow.h"

namespace Ui {
class QMosaic;
}


class CMosaic : QObject
{
    Q_OBJECT

public:
    CMosaic(Ui::MainWindow* ui, CMosaicParams* pParams, QList<CPalette>* pPaletteList, \
            QVector<int>* pPaletteCount, CMatrix* pMosaicMap, QList<QImage>* pTileList);

    QSize           getSize( QImage sourceImage );
    QImage          render( QImage sourceImage, QSize mosaicSize );
    QImage          renderCSV( CPalette palette, QSize mosaicSize );
    void            applyBrightnessContrast( QImage* image );
    void            renderTile( QImage* mosaicImage, QColor mosicRgb, int xPos, int yPos );
    CSearchResult   searchAlgorithm( QColor sourceColor, CPalette palette );
    CSearchResult   searchRgb( QColor sourceColor, CPalette palette );
    CSearchResult   searchHsv( QColor sourceColor, CPalette palette );
    CSearchResult   searchGrey( QColor sourceColor, CPalette palette );
    CSearchResult   searchHybrid( QColor sourceColor, CPalette palette );
    void            applyRandom( CSearchResult searchResult, QColor* pMosaicRgb, int* pColrNum );

private:
    Ui::MainWindow*     gui;
    CMosaicParams*      mpParams;
    QList<CPalette>*    mpPaletteList;
    QVector<int>*       mpPaletteCount;
    CMatrix*            mpMosaicMap;
    QList<QImage>*      mpTileList;
};

#endif // CMOSAIC_H
