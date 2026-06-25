#include "cpalette.h"

CPalette::CPalette( QWidget* parent)
{
    mpParent = parent;
}


bool
CPalette::load(QFile* pPaletteFile)
{
    QString line;
    QStringList list;

    QFileInfo csvFileInfo(*pPaletteFile);
    QString csvFileName(csvFileInfo.fileName());
    mPaletteName = csvFileName.section(".",0,0);
    mRgbColorList.clear();

    if ( pPaletteFile->open(QFile::ReadOnly | QFile::Text) )
    {
        mNumCols = 0;
        QTextStream in(pPaletteFile);
        // read data one line at a time
        while (!in.atEnd())
        {
            line = in.readLine();
            list = line.split(",");
            mDescriptionList.append(list.at(3));
            QColor rgbCol(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt());
            mRgbColorList.append(rgbCol);
            //qDebug() << list.at(0).toInt() << list.at(1).toInt() << list.at(2).toInt();
            mNumCols++;
        }
        //qDebug() << mNumCols;

        pPaletteFile->close();
        return true;
    }
    else // palette file fails to open
    {
        QString fileWarning = "Failed to Open File "+csvFileName;
        QMessageBox::warning(mpParent,"Open Palette File", fileWarning);
        return false;
    }
}

/*
void
CPalette::loadCSVImport(int numRows, QFile* pPaletteFile)
{
    QString line;
    QStringList list;

    //QFileInfo csvFileInfo(*pPaletteFile);
    //QString csvFileName(csvFileInfo.fileName());
    //mPaletteName = csvFileName.section(".",0,0);
    mRgbColorList.clear();

    //if ( pPaletteFile->open(QFile::ReadOnly | QFile::Text) )
    {
        mNumCols = 0;
        QTextStream in(pPaletteFile);
        // read data one line at a time
        // while (!in.atEnd())
        for( int row = 0; row < numRows; row++)
        {
            line = in.readLine();
            list = line.split(",");
            QColor rgbCol(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt());
            mRgbColorList.append(rgbCol);
            mDescriptionList.append(list.at(3));
            //qDebug() << list.at(0).toInt() << list.at(1) << list.at(2).toInt() << list.at(3).toInt() << list.at(4).toInt();
            mNumCols++;
        }
        //qDebug() << mNumCols;

        pPaletteFile->close();
 //       return true;
    }
 //   else // palette file fails to open
 //   {
 //       QString fileWarning = "Failed to Open File "+csvFileName;
 //       QMessageBox::warning(mpParent,"Open Palette File", fileWarning);
 //       return false;
 //   }

}
*/

QString
CPalette::getName()
{
    return mPaletteName;
}

QString
CPalette::getDescription(int index)
{
QString description;

    if( index >= 0 && index < mNumCols )
    {
        description = mDescriptionList.at(index);
    }
    return description;

}

int
CPalette::getNumCols()
{
    return mNumCols;
}


void
CPalette::setNumCols(int numCols)
{
    mNumCols = numCols;
}


QColor
CPalette::getRgb(int index)
{
QColor rgbValue;
    if( index >= 0 && index < mNumCols )
    {
        rgbValue = mRgbColorList.at(index);
    }
   return rgbValue;
}


int
CPalette::getRed(int index)
{
int redValue = 0;
QColor rgbValue;

    if( index >= 0 && index < mNumCols )
    {
        rgbValue = mRgbColorList.at(index);
        redValue = rgbValue.red();
    }
    return redValue;
}


int
CPalette::getGreen(int index)
{
int greenValue = 0;
QColor rgbValue;

    if( index >= 0 && index < mNumCols )
    {
        rgbValue = mRgbColorList.at(index);
        greenValue = rgbValue.green();
    }
    return greenValue;
}


int
CPalette::getBlue(int index)
{
int blueValue = 0;
QColor rgbValue;

    if( index >= 0 && index < mNumCols )
    {
        rgbValue = mRgbColorList.at(index);
        blueValue = rgbValue.blue();
    }
    return blueValue;
}
