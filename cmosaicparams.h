#ifndef CMOSAICPARAMS_H
#define CMOSAICPARAMS_H

#include <QDebug>

typedef enum { none, rgb, hsv, greyscale, hybrid } algorithm;

typedef enum { stud, square, circle } tile;

const QString KEYSTRING = "@image2mosaic";
const QString VERSTRING = "1.0";
const QString PALETTESIZE = "Palette size";
const int NUM_SIZES = 9;
const int SIZE_EQ_SOURCE = 1024;
const int MAX_SIZE[NUM_SIZES] = {16, 32, 48, 64, 96, 128, 192, 256, SIZE_EQ_SOURCE };

class CMosaicParams
{

public:
    CMosaicParams();

    void        setBrightness(int position);
    void        setContrast(int position);
    void        setRedWeight(int position);
    void        setGreenWeight(int position);
    void        setBlueWeight(int position);
    void        setMixWeight(int position);
    void        setRandomise(int index);
    void        setPaletteIndex(int index);
    void        setAlgorithm(int index);
    void        setMaxSize(int index);
    void        setTileType(int index);

    int         getBrightness();
    int         getContrast();
    int         getRedWeight();
    int         getGreenWeight();
    int         getBlueWeight();
    int         getMixWeight();
    int         getRandomise();

    int         getPaletteIndex();
    algorithm   getAlgorithm();
    int         getMaxSize();
    tile        getTileType();

private:
    int         mBrightness = 50;
    int         mContrast = 50;
    int         mRedWeight = 50;
    int         mGreenWeight = 50;
    int         mBlueWeight = 50;
    int         mMixWeight = 50;
    int         mRandomise = 0;
    int         mPaletteIndex;
    algorithm   mAlgorithm;
    int         mMaxSize;
    tile        mTileType;
};

#endif // CMOSAICPARAMS_H
