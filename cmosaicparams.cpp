#include "cmosaicparams.h"

CMosaicParams::CMosaicParams() { }

void
CMosaicParams::setBrightness(int position)
{
    mBrightness = position;
}

void
CMosaicParams::setContrast(int position)
{
    mContrast = position;
}

void
CMosaicParams::setRedWeight(int position)
{
    mRedWeight = 100 - position;
}

void
CMosaicParams::setGreenWeight(int position)
{
    mGreenWeight = 100 - position;
}

void
CMosaicParams::setBlueWeight(int position)
{
    mBlueWeight = 100 - position;
}

void
CMosaicParams::setMixWeight(int position)
{
    mMixWeight = position;
}

void
CMosaicParams::setRandomise(int position)
{
    mRandomise = position;
}

void
CMosaicParams::setPaletteIndex(int index)
{
    mPaletteIndex = index;
}

void
CMosaicParams::setAlgorithm(int index)
{
    // this indexes into the algorithm enum type
    mAlgorithm = (algorithm) index;
}

void
CMosaicParams::setMaxSize(int index)
{
    mMaxSize = MAX_SIZE[index];
}

void
CMosaicParams::setTileType(int index)
{
    // this indexes into the tile enum type
    mTileType = (tile) index;
}


int
CMosaicParams::getBrightness()
{
    return mBrightness;
}

int
CMosaicParams::getContrast()
{
    return mContrast;
}

int
CMosaicParams::getRedWeight()
{
    return mRedWeight;
}

int
CMosaicParams::getGreenWeight()
{
    return mGreenWeight;
}

int
CMosaicParams::getBlueWeight()
{
    return mBlueWeight;
}

int
CMosaicParams::getMixWeight()
{
    return mMixWeight;
}

int
CMosaicParams::getRandomise()
{
    return mRandomise;
}

int
CMosaicParams::getPaletteIndex()
{
    return mPaletteIndex;
}

algorithm
CMosaicParams::getAlgorithm()
{
    return mAlgorithm;
}

int
CMosaicParams::getMaxSize()
{
    return mMaxSize;
}

tile
CMosaicParams::getTileType()
{
    return mTileType;
}











