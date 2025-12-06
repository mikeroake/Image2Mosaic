#ifndef CPALETTE_H
#define CPALETTE_H

#include <QWidget>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

class CPalette
{    
public:
    CPalette( QWidget* parent);

    bool    load(QFile* pPaletteFile);
    QString getName();
    QString getDescription(int index);
    int     getNumCols();
    QColor  getRgb(int index);
    int     getRed(int index);
    int     getGreen(int index);
    int     getBlue(int index);

private:
    QWidget*        mpParent;
    QString         mPaletteName;
    int             mNumCols;
    QList<QString>  mDescriptionList;
    QList<QColor>   mRgbColorList;
};

#endif // CPALETTE_H
