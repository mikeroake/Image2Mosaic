#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cpalette.h"
#include "cmatrix.h"
#include "cmosaic.h"
#include "cmosaicparams.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSize               mMinWinSize;
    QSize               mMosaicWinSize;
    QPoint              mRenderPBPos;
    QPoint              mProgressBarPos;
    QPoint              mMosaicLabelPos;
    QPoint              mCountLabelPos;

    QIcon               mIcon;
    QPixmap             mIconPixmap;
    QImage              mSourceImage;
    QImage              mMosaicImage;
    QList<QImage>       mTileList;
    QString             mSourceFile;
    QString             mSourceDir;
    QString             mPaletteDir;
    QString             mPrefix;
    QList<CPalette>     mPaletteList;
    QVector<int>        mPaletteCount;
    CMosaicParams       mParams;
    CMosaic*            mpMosaic;
    CMatrix             mMosaicMap;

    bool                mImageLoaded;
    bool                mImageRendered;

private slots:
    // slots for menu items
    void on_actionLoad_Image_triggered();
    void on_actionSave_Mosaic_triggered();
    void on_actionExport_CSV_triggered();
    void on_actionImport_CSV_triggered();
    void on_actionImport_Palette_triggered();
    void on_actionAbout_triggered();
    void on_actionAboutQt_triggered();
    void on_actionPreferences_2_triggered();

    // slots for combo boxes and buttons
    void on_paletteComboBox_currentIndexChanged(int index);
    void on_sizeComboBox_currentIndexChanged(int index);
    void on_algoComboBox_currentIndexChanged(int index);
    void on_tileComboBox_currentIndexChanged(int index);
    void on_resetButton_clicked();
    void renderButtonClicked();

    // slots for custom sliders (QSliderPlus)
    void brightSliderChanged( int newPosition );
    void contrastSliderChanged( int newPosition );
    void redSliderChanged( int newPosition );
    void greenSliderChanged( int newPosition );
    void blueSliderChanged( int newPosition );
    void mixSliderChanged( int newPosition );
    void randomSliderChanged( int newPosition );

    void resetRender();



public slots:
    void updateProgressBar(float percentComplete);


private:
    Ui::MainWindow *ui;
    void resizeEvent(QResizeEvent*);

    void loadSettings();
    void saveSettings();
    bool importPalette(QString csvFileName);
    bool importPalette(QString directory, QString filename);
    void setSliders();
    void sourceLightnessContrast();

    // from https://stackoverflow.com/questions/27318631/parsing-through-a-csv-file-in-qt
    QStringList parseCSV(const QString &string);

    const QString DEFAULT_DIR = "/Users/michaelroake/Documents/MATLAB/Image";
    const QString DEFAULT_PREFIX = "brick_";
};
#endif // MAINWINDOW_H
