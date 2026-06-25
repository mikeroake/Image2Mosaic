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
//    void    loadCSVImport(int numRows, QFile* pPaletteFile);
    QString getName();
    QString getDescription(int index);
    int     getNumCols();
    void    setNumCols(int numCols);
    QColor  getRgb(int index);
    int     getRed(int index);
    int     getGreen(int index);
    int     getBlue(int index);

    QList<QString>  mDescriptionList;
    QList<QColor>   mRgbColorList;
    int             mNumCols;


private:
    QWidget*        mpParent;
    QString         mPaletteName;
};

#endif // CPALETTE_H
