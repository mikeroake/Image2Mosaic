 #include "cmosaic.h"

CMosaic::CMosaic(Ui::MainWindow* ui, CMosaicParams* pParams, QList<CPalette>* pPaletteList, \
                 QVector<int>* pPaletteCount, CMatrix* pMosaicMap, QList<QImage>* pTileList)
{
    mpParams = pParams;
    mpPaletteList = pPaletteList;
    mpPaletteCount = pPaletteCount;
    mpMosaicMap = pMosaicMap;
    mpTileList = pTileList;
    gui = ui;
}

QSize
CMosaic::getSize( QImage sourceImage )
{
QSize   sourceSize = sourceImage.size();
QSize   mosaicSize;
int     minSize;


    if(mpParams->getMaxSize() == SIZE_EQ_SOURCE && sourceSize.width() <= SIZE_EQ_SOURCE && sourceSize.height() <= SIZE_EQ_SOURCE)
    {
        mosaicSize.setWidth(sourceSize.width());
        mosaicSize.setHeight(sourceSize.height());
    }
    else // all brick mosaic sizes and 'Source' larger dimension not exceeding 1024
    {
        if(sourceSize.width() > sourceSize.height())
        {
            mosaicSize.setWidth(mpParams->getMaxSize());
            minSize = (unsigned int) mpParams->getMaxSize()*((float) sourceSize.height()/(float) sourceSize.width());
            mosaicSize.setHeight(minSize);
        }
        else
        {
            mosaicSize.setHeight(mpParams->getMaxSize());
            minSize = (unsigned int) mpParams->getMaxSize()*((float) sourceSize.width()/(float) sourceSize.height());
            mosaicSize.setWidth(minSize);
        }
    }
    return mosaicSize;
}

QImage
CMosaic::render( QImage sourceImage, QSize mosaicSize )
{
long            totalTiles;
long            renderedTiles = 0;
int             percentComplete = 0;
QColor          sourceRgb;
QColor          mosaicRgb;
int             colrNum = 0;
int             numCols;
int             colCount;
CSearchResult   searchResult;
QImage          mosaicImage;

    // rescale source image to have same dimensions in bricks as mosaic
    QImage sourceImageScaled = sourceImage.scaled(mosaicSize.width(),mosaicSize.height(),Qt::IgnoreAspectRatio);
    if( mpParams->getMaxSize() == SIZE_EQ_SOURCE )
    {
      mosaicImage = sourceImageScaled.scaled(mosaicSize.width(),mosaicSize.height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    }
    else
    {
      mosaicImage = sourceImageScaled.scaled(mosaicSize.width()*20,mosaicSize.height()*20,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    }
    CPalette palette = mpPaletteList->at(mpParams->getPaletteIndex());
    numCols = palette.getNumCols();
    mpPaletteCount->clear();
    mpPaletteCount->fill(0,numCols);
    mpMosaicMap->setSize(mosaicSize.height(),mosaicSize.width());

    applyBrightnessContrast(&sourceImageScaled);

    totalTiles = mosaicSize.width()*mosaicSize.height();
    for(int row = 0; row < mosaicSize.height(); row++)
    {
        for(int col = 0; col < mosaicSize.width(); col++)
        {
            sourceRgb = sourceImageScaled.pixelColor(col,row);
            searchResult = searchAlgorithm( sourceRgb, palette );
            applyRandom( searchResult, &mosaicRgb, &colrNum );

            colCount = mpPaletteCount->at(colrNum);
            colCount++;
            mpPaletteCount->replace(colrNum,colCount);
            mpMosaicMap->setElement(row, col, colrNum);

            if( mpParams->getMaxSize() == SIZE_EQ_SOURCE )
            {
              mosaicImage.setPixelColor(col, row, mosaicRgb);
            }
            else
            {
              renderTile( &mosaicImage, mosaicRgb, col, row );
            }
            renderedTiles++;
            percentComplete = (int) (100.0*((float) renderedTiles / (float) totalTiles));
            gui->progressBar->setValue(percentComplete);
            // force continual update of progress bar
            QApplication::processEvents();
        }

    }
    return mosaicImage;
}


void
CMosaic::applyBrightnessContrast( QImage* image )
{
int     x,y;
QColor  sourceRgb;
QColor  modifiedRgb;
int     hue, saturation, lightness;
int     newLightness;
float   gain;
float   meanLightness;
float   sumLightness;

    QSize imageSize = image->size();

    // apply lightness and calculate average lightness
    sumLightness = 0.0;
    for(x = 0; x < imageSize.width(); x++)
    {
        for(y = 0; y < imageSize.height(); y++)
        {
            sourceRgb = image->pixelColor(x,y);
            hue = sourceRgb.hue();
            saturation = sourceRgb.saturation();
            lightness = sourceRgb.value();
            gain = 0.01*((float) mpParams->getBrightness() - 50.0) + 1.0;
            newLightness = (int) (gain*lightness);
            if (newLightness < 0)
                newLightness = 0;
            if (newLightness > 255)
                newLightness = 255;

            modifiedRgb.setHsv(hue,saturation,newLightness);
            image->setPixelColor(x,y,modifiedRgb);
            sumLightness = sumLightness + newLightness;
        }
    }
    meanLightness = sumLightness / (imageSize.width() * imageSize.height());

    // apply contrast
    for(x = 0; x < imageSize.width(); x++)
    {
        for(y = 0; y < imageSize.height(); y++)
        {
            sourceRgb = image->pixelColor(x,y);
            hue = sourceRgb.hue();
            saturation = sourceRgb.saturation();
            lightness = sourceRgb.value();
            gain = 0.02*((float) mpParams->getContrast() - 50.0) + 1.0;
            newLightness = (int) (gain*(lightness - meanLightness)) + meanLightness;
            if (newLightness < 0)
                newLightness = 0;
            if (newLightness > 255)
                newLightness = 255;

            modifiedRgb.setHsv(hue,saturation,newLightness);
            image->setPixelColor(x,y,modifiedRgb);
        }
    }
}

CSearchResult
CMosaic::searchAlgorithm( QColor sourceColor, CPalette palette )
{
//int pColNum = 0;
CSearchResult result;
CSearchResult searchResult;

    switch(mpParams->getAlgorithm())
    {
        case none:
        searchResult = CSearchResult( sourceColor );
        break;

        case rgb:
        searchResult = searchRgb( sourceColor, palette );
        break;

        case hsv:
        searchResult = searchHsv( sourceColor, palette );
        break;

        case greyscale:
        searchResult = searchGrey( sourceColor, palette );
        break;

        case hybrid:
        searchResult = searchHybrid( sourceColor, palette );
        break;
    }
    return searchResult;
}

CSearchResult
CMosaic::searchRgb( QColor sourceColor, CPalette palette )
{
CSearchResult   searchResult;
QColor          palColor;
QColor          nearestColor;
QColor          secondColor;
int             colr;
int             nearestColrNum = 0;
int             secondColrNum = 0;
int             palRed, palGreen, palBlue;
long            distanceSquared;
long            minDistanceSquared = 0;
long            secDistanceSquared = 0 ;
int             srcRed, srcGreen, srcBlue;
bool            firstIsNearest = true;

    srcRed = sourceColor.red();
    srcGreen = sourceColor.green();
    srcBlue = sourceColor.blue();

    for( colr = 0; colr < palette.getNumCols(); colr++)
    {
        palColor = palette.getRgb(colr);
        palRed = palColor.red();
        palGreen = palColor.green();
        palBlue = palColor.blue();

        distanceSquared = (mpParams->getRedWeight()*(srcRed - palRed)*(srcRed - palRed) +\
                           mpParams->getGreenWeight()*(srcGreen - palGreen)*(srcGreen - palGreen) +\
                           mpParams->getBlueWeight()*(srcBlue - palBlue)*(srcBlue - palBlue));
        if( colr == 0)
        {
            minDistanceSquared = distanceSquared;
            secDistanceSquared = distanceSquared;
            nearestColor = palColor;
            secondColor = palColor;
            nearestColrNum = colr;
            secondColrNum = colr;
        }
        else
        {
            if( distanceSquared < minDistanceSquared )
            {
                firstIsNearest = false;
                secDistanceSquared = minDistanceSquared;
                minDistanceSquared = distanceSquared;
                secondColor = nearestColor;
                nearestColor = palColor;
                secondColrNum = nearestColrNum;
                nearestColrNum = colr;
            }
            // the following logic is required to obtain correctly the second
            // closest color in the case that the first color tested is closest
            else if( firstIsNearest == true)
            {
                if( distanceSquared < secDistanceSquared )
                {
                    secDistanceSquared = distanceSquared;
                    secondColor = palColor;
                    secondColrNum = colr;
                }
            }
        }
    }
    searchResult.mNearestColor = nearestColor;
    searchResult.mSecondColor = secondColor;
    searchResult.mNearestDist = sqrt( minDistanceSquared );
    searchResult.mSecondDist = sqrt( secDistanceSquared );
    searchResult.mNearestColrNum = nearestColrNum;
    searchResult.mSecondColrNum = secondColrNum;
    return searchResult;
}

CSearchResult
CMosaic::searchHsv( QColor sourceColor, CPalette palette )
{
CSearchResult   searchResult;
QColor          palColor;
QColor          nearestColor;
QColor          secondColor;
int             colr;
int             nearestColrNum = 0;
int             secondColrNum = 0;
int             palHue, palSaturation, palValue;
long            distanceSquared;
long            minDistanceSquared = 0;
long            secDistanceSquared = 0 ;
int             srcHue, srcSaturation, srcValue;
bool            firstIsNearest = true;

    srcHue = sourceColor.hue();
    srcSaturation = sourceColor.saturation();
    srcValue = sourceColor.value();

    for( colr = 0; colr < palette.getNumCols(); colr++)
    {
        palColor = palette.getRgb(colr);
        palHue = palColor.hue();
        palSaturation = palColor.saturation();
        palValue = palColor.value();

        distanceSquared = (mpParams->getRedWeight()*(srcHue - palHue)*(srcHue - palHue) +\
                           mpParams->getGreenWeight()*(srcSaturation - palSaturation)*(srcSaturation - palSaturation) +\
                           mpParams->getBlueWeight()*(srcValue - palValue)*(srcValue - palValue));
        if( colr == 0)
        {
            minDistanceSquared = distanceSquared;
            secDistanceSquared = distanceSquared;
            nearestColor = palColor;
            secondColor = palColor;
            nearestColrNum = colr;
            secondColrNum = colr;
        }
        else
        {
            if( distanceSquared < minDistanceSquared )
            {
                firstIsNearest = false;
                secDistanceSquared = minDistanceSquared;
                minDistanceSquared = distanceSquared;
                secondColor = nearestColor;
                nearestColor = palColor;
                secondColrNum = nearestColrNum;
                nearestColrNum = colr;
            }
            // the following logic is required to obtain correctly the second
            // closest color in the case that the first color tested is closest
            else if( firstIsNearest == true)
            {
                if( distanceSquared < secDistanceSquared )
                {
                    secDistanceSquared = distanceSquared;
                    secondColor = palColor;
                    secondColrNum = colr;
                }
            }
        }
    }
    searchResult.mNearestColor = nearestColor;
    searchResult.mSecondColor = secondColor;
    searchResult.mNearestDist = sqrt( minDistanceSquared );
    searchResult.mSecondDist = sqrt( secDistanceSquared );
    searchResult.mNearestColrNum = nearestColrNum;
    searchResult.mSecondColrNum = secondColrNum;
    return searchResult;
}

CSearchResult
CMosaic::searchGrey( QColor sourceColor, CPalette palette )
{
CSearchResult   searchResult;
QColor          palColor;
QColor          nearestColor;
QColor          secondColor;
int             colr;
int             nearestColrNum = 0;
int             secondColrNum = 0;
int             palGrey;
long            distanceSquared;
long            minDistanceSquared = 0;
long            secDistanceSquared = 0 ;
bool            firstIsNearest = true;
int             srcGrey;

        srcGrey = qGray(sourceColor.rgb());

        for( colr = 0; colr < palette.getNumCols(); colr++)
        {
            palColor = palette.getRgb(colr);
            palGrey = qGray(palColor.rgb());

            distanceSquared = (srcGrey - palGrey)*(srcGrey - palGrey);
            if( colr == 0)
            {
                minDistanceSquared = distanceSquared;
                secDistanceSquared = distanceSquared;
                nearestColor = palColor;
                secondColor = palColor;
                nearestColrNum = colr;
                secondColrNum = colr;
            }
            else
            {
                if( distanceSquared < minDistanceSquared )
                {
                    firstIsNearest = false;
                    secDistanceSquared = minDistanceSquared;
                    minDistanceSquared = distanceSquared;
                    secondColor = nearestColor;
                    nearestColor = palColor;
                    secondColrNum = nearestColrNum;
                    nearestColrNum = colr;
                }
                // the following logic is required to obtain correctly the second
                // closest color in the case that the first color tested is closest
                else if( firstIsNearest == true)
                {
                    if( distanceSquared < secDistanceSquared )
                    {
                        secDistanceSquared = distanceSquared;
                        secondColor = palColor;
                        secondColrNum = colr;
                    }
                }
            }
        }
        searchResult.mNearestColor = nearestColor;
        searchResult.mSecondColor = secondColor;
        searchResult.mNearestDist = sqrt( minDistanceSquared );
        searchResult.mSecondDist = sqrt( secDistanceSquared );
        searchResult.mNearestColrNum = nearestColrNum;
        searchResult.mSecondColrNum = secondColrNum;
        return searchResult;
 }

CSearchResult
CMosaic::searchHybrid( QColor sourceColor, CPalette palette )
{
CSearchResult   searchResult;
CSearchResult   rgbSearch;
CSearchResult   bwSearch;
long    rgbDistance;
long    bwDistance;

    rgbSearch = searchRgb( sourceColor, palette );
    bwSearch = searchGrey( sourceColor, palette );

    // apply scaling to allow sensible comparison with rgbDistance
    bwDistance = 2000*bwSearch.mNearestDist;
    rgbDistance = rgbSearch.mNearestDist;
    mpParams->getMixWeight();
    if( mpParams->getMixWeight()*bwDistance < (100 - mpParams->getMixWeight())*rgbDistance)
    {
        searchResult = bwSearch;
    }
    else
    {
        searchResult = rgbSearch;
    }   
    return searchResult;
}


void
CMosaic::applyRandom( CSearchResult searchResult, QColor* pMosaicRgb, int* pColrNum )
{
int randSlider;
float random;
float randomSet;
float prob2;

    randSlider =  mpParams->getRandomise();
    randomSet = ((float) randSlider) / 50.0;

    if( mpParams->getAlgorithm() == hybrid )
    {
        *pMosaicRgb = searchResult.mNearestColor;
        *pColrNum = searchResult.mNearestColrNum;
    }
    else // all other algorithms
    {
        if( randSlider == 0 )
        {
           *pMosaicRgb = searchResult.mNearestColor;
           *pColrNum = searchResult.mNearestColrNum;
        }
        else // 0 < randSlider <= 100
        {
            random = (float) QRandomGenerator::global()->generateDouble();
            prob2 = 0.5*randomSet*( searchResult.mNearestDist / searchResult.mSecondDist );
            // clamp probabilty of second nearest could to no more than 50%
            if( prob2 > 0.5 )
            {
                prob2 = 0.5;
            }
            if( random > prob2 )
            {
                *pMosaicRgb = searchResult.mNearestColor;
                *pColrNum = searchResult.mNearestColrNum;
            }
            else
            {
                *pMosaicRgb = searchResult.mSecondColor;
                *pColrNum = searchResult.mSecondColrNum;
            }
        }
    }
}


void
CMosaic::renderTile( QImage* mosaicImage, QColor mosaicRgb, int xPos, int yPos )
{
int     x,y;
int     brickRed,brickGreen, brickBlue;
int     tileRed, tileGreen, tileBlue;
int     mosaicRed, mosaicGreen, mosaicBlue;
QColor  pixelColor;
QColor  background;
QImage  tileImage;

    switch(mpParams->getTileType())
    {
        case stud:
        tileImage = mpTileList->at(0);
        break;

        case square:
        tileImage = mpTileList->at(1);
        break;

        case circle:
        tileImage = mpTileList->at(2);
        break;
    }

    brickRed = mosaicRgb.red();
    brickGreen = mosaicRgb.green();
    brickBlue = mosaicRgb.blue();

    // render tile
    for(x = 0; x < 20; x++)
    {
        for(y = 0; y < 20; y++)
        {
            tileRed = tileImage.pixelColor(x,y).red();
            tileGreen = tileImage.pixelColor(x,y).green();
            tileBlue = tileImage.pixelColor(x,y).blue();

            mosaicRed = (int)( (float)(tileRed*brickRed)/255.0);
            mosaicGreen = (int)( (float)(tileGreen*brickGreen)/255.0);
            mosaicBlue = (int)( (float)(tileBlue*brickBlue)/255.0);

            pixelColor.setRgb(mosaicRed, mosaicGreen, mosaicBlue);
            mosaicImage->setPixelColor(xPos*20 + x, yPos*20 + y, pixelColor);
        }
    }
    // render background of circular tile
    if(mpParams->getTileType() == circle)
    {
        background.setRgb(150, 150, 150);
        for(x = 0; x < 20; x++)
        {
            for(y = 0; y < 20; y++)
            {
                tileRed = tileImage.pixelColor(x,y).red();
                tileGreen = tileImage.pixelColor(x,y).green();
                tileBlue = tileImage.pixelColor(x,y).blue();
                // black area of circular tile indicates background
                if( tileRed == 0 && tileGreen == 0 && tileBlue == 0 )
                {
                    mosaicImage->setPixelColor(xPos*20 + x, yPos*20 + y, background);
                }
            }
        }
    }
}
