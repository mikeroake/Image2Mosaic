#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include "aboutdialog.h"
#include "preferencesdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // store widget sizes and positions with min window size
    mMinWinSize = this->size();
    mMosaicWinSize = ui->mosaicView->size();
    mRenderPBPos = ui->renderButton->pos();
    mProgressBarPos = ui->progressBar->pos();
    mMosaicLabelPos = ui->mosaicLabel->pos();
    mCountLabelPos = ui->countLabel->pos();


    //this->setStyleSheet("background-color: red;");

    //open qss file
    QFile file(":/resources/Stylesheet/Combinear.qss");
    file.open(QFile::ReadOnly);

    QString styleSheet { QString(file.readAll()) };

    //setup stylesheet
    qApp->setStyleSheet(styleSheet);

    //run
    show();

    //qApp->setStyleSheet("MainWindow { background-image: url(:/resources/Images/brick_dark grey.jpg) }");

    mIcon = QIcon(":/resources/Icons/icon-256.png");
    mIconPixmap = mIcon.pixmap(100,100);
    // for Windows implementation
    setWindowIcon(mIcon);

    mpMosaic = new CMosaic(ui, &mParams, &mPaletteList, &mPaletteCount, &mMosaicMap, &mTileList);

    QString mainTitle = "Image2Mosaic";
    this->setWindowTitle(mainTitle);

    ui->sizeComboBox->addItems({"16","32","48","64","96","128","192","256","Source"});
    ui->sizeComboBox->setCurrentIndex(3);

    ui->algoComboBox->addItems({"None","RGB","HSV","Greyscale","Hybrid"});
    ui->algoComboBox->setCurrentIndex(1);
    mParams.setAlgorithm(1);

    ui->tileComboBox->addItems({"1x1 Stud","Square","Round"});
    ui->tileComboBox->setCurrentIndex(0);

    QImage tileImage;
    QFile tileFileName(":/resources/Images/LegoBrick20.jpg");
    tileImage.load(tileFileName.fileName(), "JPG");
    mTileList.append(tileImage);

    tileFileName.setFileName(":/resources/Images/SquareBrick20.jpg");
    tileImage.load(tileFileName.fileName(), "JPG");
    mTileList.append(tileImage);

    tileFileName.setFileName(":/resources/Images/CircleBrick20.jpg");
    tileImage.load(tileFileName.fileName(), "JPG");
    mTileList.append(tileImage);

    QGraphicsScene* sourceGraphics = new QGraphicsScene( this );
    ui->sourceImageView->setBackgroundBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
    ui->sourceImageView->setScene( sourceGraphics);

    //ui->sourceImageView->setAcceptDrops(true);
    //ui->sourceImageView->dropEvent();

    QGraphicsScene* mosaicGraphics = new QGraphicsScene( this );
    ui->mosaicView->setBackgroundBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
    ui->mosaicView->setScene( mosaicGraphics );

    loadSettings();

    importPalette(":/resources/Palettes/Lego Full.csv");
    importPalette(":/resources/Palettes/Lego Store 1x1.csv");
    importPalette(":/resources/Palettes/Lego Store 1x1 B&W.csv");
    importPalette(":/resources/Palettes/Smartable Full.csv");
    importPalette(":/resources/Palettes/Smartable Reduced.csv");
    importPalette(":/resources/Palettes/Smartable Pastel 12.csv");
    importPalette(":/resources/Palettes/Smartable Skin 12.csv");
    importPalette(":/resources/Palettes/Acrylic Full.csv");
    mParams.setPaletteIndex(0);
    ui->paletteComboBox->setCurrentIndex(mParams.getPaletteIndex());

    ui->brightSlider->setup("Brightness",0.5,1.5);
    ui->contrastSlider->setup("Contrast",0.5,1.5);
    ui->redSlider->setup("Red",0.0,2.0);
    ui->greenSlider->setup("Green",0.0,2.0);
    ui->blueSlider->setup("Blue",0.0,2.0);
    ui->mixSlider->setup("Hybrid","Gr","RGB");
    ui->randomSlider->setup("Random",0.0,2.0);

    ui->brightSlider->enable(false);
    ui->contrastSlider->enable(false);
    ui->redSlider->enable(false);
    ui->greenSlider->enable(false);
    ui->blueSlider->enable(false);
    ui->mixSlider->enable(false);
    ui->randomSlider->enable(false);

    ui->randomSlider->setValue(0.0);

    // connect signals from QSliderPlus objects to slots in MainWindow
    connect(ui->brightSlider, &QSliderPlus::valueChanged, this, &MainWindow::brightSliderChanged);
    connect(ui->contrastSlider, &QSliderPlus::valueChanged, this, &MainWindow::contrastSliderChanged);
    connect(ui->redSlider, &QSliderPlus::valueChanged, this, &MainWindow::redSliderChanged);
    connect(ui->greenSlider, &QSliderPlus::valueChanged, this, &MainWindow::greenSliderChanged);
    connect(ui->blueSlider, &QSliderPlus::valueChanged, this, &MainWindow::blueSliderChanged);
    connect(ui->mixSlider, &QSliderPlus::valueChanged, this, &MainWindow::mixSliderChanged);
    connect(ui->randomSlider, &QSliderPlus::valueChanged, this, &MainWindow::randomSliderChanged);

    connect(ui->renderButton, &QPushButton::clicked, this, &MainWindow::renderButtonClicked);

    ui->progressBar->setRange(0,100);
    ui->progressBar->reset();
    ui->renderButton->setDisabled(true);

    mImageLoaded = false;
    mImageRendered = false;
    ui->actionSave_Mosaic->setEnabled(false);
    ui->actionExport_CSV->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete mpMosaic;
    delete ui;
}

void
MainWindow::resizeEvent(QResizeEvent*)
{
   // handle resizing of main window
   QSize mainWinSize = this->size();
   int xInc = mainWinSize.width() - mMinWinSize.width();
   int yInc = mainWinSize.height() - mMinWinSize.height();
   if (xInc < 0) {xInc = 0;}
   if (yInc < 0) {yInc = 0;}

   ui->renderButton->move( mRenderPBPos.x(), mRenderPBPos.y() + yInc );
   ui->progressBar->move( mProgressBarPos.x(), mProgressBarPos.y() + yInc );
   ui->mosaicLabel->move( mMosaicLabelPos.x() + xInc, mMosaicLabelPos.y() + yInc );
   ui->countLabel->move( mCountLabelPos.x() + xInc, mCountLabelPos.y() + yInc );

   ui->mosaicView->resize(mMosaicWinSize.width() + xInc, mMosaicWinSize.height() + yInc);
   QSize mosaicWinSize = ui->mosaicView->size();

   if(!mMosaicImage.isNull())
   {
      QImage mosaicImageWin = mMosaicImage.scaled(mosaicWinSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
      QGraphicsScene* mosaicGraphics = new QGraphicsScene( this );
      mosaicGraphics->addPixmap( QPixmap::fromImage( mosaicImageWin ));
      ui->mosaicView->setBackgroundBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
      ui->mosaicView->setScene( mosaicGraphics );
   }
}

void
MainWindow::loadSettings()
{
    QSettings settings("mike roake","Image2Mosaic");
    settings.beginGroup("preferences");
    mSourceDir = settings.value("sourceDir",DEFAULT_DIR).toString();
    mPaletteDir = settings.value("paletteDir",DEFAULT_DIR).toString();
    mPrefix = settings.value("prefix",DEFAULT_PREFIX).toString();
    settings.endGroup();
}

void
MainWindow::saveSettings()
{
    QSettings settings("mike roake","Image2Mosaic");
    settings.beginGroup("preferences");
    settings.setValue("sourceDir", this->mSourceDir);
    settings.setValue("paletteDir", this->mPaletteDir);
    settings.setValue("prefix", this->mPrefix);
    settings.endGroup();
}

void
MainWindow::on_actionLoad_Image_triggered()
{
    QString filters("*.jpg;;*.png");
    QString sourceFileName = QFileDialog::getOpenFileName(this, "Select Image File",mSourceDir, filters );
    if (!sourceFileName.isNull())
    {
        // open image file & display title
        QFileInfo sourceFileInfo(sourceFileName);
        mSourceFile = sourceFileInfo.fileName();
        QString mainTitle = "Image2Mosaic ("+mSourceFile+")";
        this->setWindowTitle(mainTitle);

        // load source image file and resize for display in source window
        QSize sourceWinSize = ui->sourceImageView->size();
        mSourceImage.load(sourceFileName, "JPG | PNG");
        QSize sourceSize = mSourceImage.size();
        QImage sourceImageWin = mSourceImage.scaled(sourceWinSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // display source image in source window
        QGraphicsScene* sourceGraphics = new QGraphicsScene( this );
        sourceGraphics->addPixmap( QPixmap::fromImage( sourceImageWin ));
        ui->sourceImageView->setBackgroundBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
        ui->sourceImageView->setScene(sourceGraphics);

        // display size of source image
        QString pixelText = QString("%1 x %2 pixels").arg(sourceSize.width()).arg(sourceSize.height());
        ui->pixelLabel->setText(pixelText);

        // clear & prepare mosaic screen to render from fresh image
        QGraphicsScene* mosaicGraphics = new QGraphicsScene( this );
        ui->mosaicView->setBackgroundBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
        ui->mosaicView->setScene( mosaicGraphics);
        mMosaicImage = QImage();
        mImageLoaded = true;
        setSliders();
        resetRender();
        ui->actionSave_Mosaic->setEnabled(false);
        ui->actionExport_CSV->setEnabled(false);

        // clear info text under mosaic
        QString mosaicText = QString();
        ui->mosaicLabel->setText(mosaicText);
    }
}


void MainWindow::on_actionSave_Mosaic_triggered()
{
    if(!mMosaicImage.isNull())
    {
        // this will apply default directory and filename
        QString defaultFile = mSourceDir+"/"+mPrefix+mSourceFile;
        QString filters("*.jpg;;*.png");
        QString mosaicFile = QFileDialog::getSaveFileName(this,"Save Mosaic File",defaultFile,filters);
        if (!mosaicFile.isNull())
        {
            mMosaicImage.save( mosaicFile,"JPG", 100);
        }
    }
    else
    {
        QMessageBox::information(this, "Save Mosaic", "No Mosaic Rendered");
    }
}


void MainWindow::on_actionExport_CSV_triggered()
{
QString strNum;
bool dispColrName = false;

    if(!mMosaicImage.isNull())
    {
        // get file name without extension e.g. .jpg
        int lastPoint = mSourceFile.lastIndexOf(".");
        QString fileNameNoExt = mSourceFile.left(lastPoint);
        QString defaultFile = mSourceDir+"/"+fileNameNoExt;
        QString filters("*.csv");
        QString csvFileName = QFileDialog::getSaveFileName(this,"Export CSV File",defaultFile,filters);
        QFile file(csvFileName);
        CPalette palette = mPaletteList.at(mParams.getPaletteIndex());

        // this is export of palette inventory
        if(file.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream output(&file);
            // write CSV file header: keyString, no of rows, no of columns
            output << KEYSTRING << ",";
            output << VERSTRING << ",";
            output << mMosaicMap.getNumCols()<< ",";
            output << mMosaicMap.getNumRows() << "\n";

            // write Mosaic elements as matrix
            for( int row = 0; row < mMosaicMap.getNumRows(); row++)
            {
                for( int col = 0; col < mMosaicMap.getNumCols(); col++)
                {
                    if( dispColrName == true)
                    {
                        int colrNum = mMosaicMap.getElement(row,col);
                        QString description = palette.getDescription(colrNum);
                        output << description;
                    }
                    else
                    {
                       strNum = QString::number(mMosaicMap.getElement(row,col) + 1);
                       output << strNum;
                    }
                    if( (col+1) < mMosaicMap.getNumCols())
                    {
                       output << ",";
                    }
               }
               output << "\n";
            }
            // write palette header
            output << PALETTESIZE << "," << mPaletteCount.size() << ",";
            output << "Colours used, "<< mNumCols << ",";
            output << palette.getName() << ".csv" <<  "\n";

            for(int colr = 0;colr < mPaletteCount.size();colr++)
            {
                QString description = palette.getDescription(colr);
                int red = palette.getRed(colr);
                int green = palette.getGreen(colr);
                int blue = palette.getBlue(colr);
                QString count = QString::number(mPaletteCount.at(colr));
                output << (colr + 1) << "," << red << "," << green << "," << blue << "," << description << "," << count << "\n";
                qDebug() << red << green << blue;
            }
            file.close();
        }
    }
    else
    {
        QMessageBox::information(this, "Export CSV File", "No Mosaic Rendered");
    }   
}


void MainWindow::on_actionImport_CSV_triggered()
{
unsigned int brickPitch = 8;
QString csvString;
QStringList csvList;
CPalette palette(this);
QString colrStr;
QString csvLine;

int colrNum;


    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "", "Importing a CSV file will erase the current render.  Do you wish to continue?");
    if (reply == QMessageBox::No)
    {
        return;
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), mSourceDir, tr("csv files (*.csv)"));
    if( fileName.isEmpty() || fileName.isNull())
    {
        return;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(this, "", "Failed to Load File");
        return;
    }
    // read contents of entire file
    QString temp = file.readAll();
    //QTextStream csvStream(&fileName);
    QTextStream csvStream(&temp);

    // read file header line
    csvLine = csvStream.readLine();
    csvList = csvLine.split(",");

    // check first entry in file
    if( csvList.at(0) != KEYSTRING )
    {
        QMessageBox::warning(this, "", "File Incorrect Format - KEYSTRING");
        file.close();
        return;
    }
    // ignore second enty (VERSTRING); reserved for future use

    int width = csvList.at(2).toInt();
    int height = csvList.at(3).toInt();
    //qDebug() << width << height;
    QSize mosaicSize = QSize(width,height);
    mMosaicMap.setSize( height, width );

    // read mosaic map
    for( int row = 0; row < height; row++)
    {
        // read each row of array
        csvLine = csvStream.readLine();
        csvList = csvLine.split(",");
        // qDebug() << csvLine;
        for( int col = 0; col < width; col++)
        {
            colrStr = csvList.at(col);
            colrStr.remove(QChar(0x0001));
            colrNum = colrStr.toInt();
            //qDebug() << colrNum;
            mMosaicMap.setElement(row, col, colrNum-1);
        }
    }

    // read palette header
    csvLine = csvStream.readLine();
    csvList = csvLine.split(",");

    if( csvList.at(0) != PALETTESIZE )
    {
        QMessageBox::warning(this, "", "File Incorrect Format - PALETTESIZE");
        file.close();
        return;
    }
    palette.mNumCols = csvList.at(1).toInt();
    //int colrsUsed = csvList.at(3).toInt();

    // read palette table
    palette.mDescriptionList.clear();
    palette.mRgbColorList.clear();

    for( int colr = 1; colr <= palette.mNumCols; colr++)
    {
        csvLine = csvStream.readLine();
        csvList = csvLine.split(",");
        palette.mDescriptionList.append(csvList.at(4));
        QColor rgbCol(csvList.at(1).toInt(), csvList.at(2).toInt(), csvList.at(3).toInt());
        palette.mRgbColorList.append(rgbCol);
    }
    file.close();

    // render mosaic
    mMosaicImage = mpMosaic->renderCSV( palette, mosaicSize );

    // display rendered mosaic
    QSize mosaicWinSize = ui->mosaicView->size();
    QImage mosaicImageWin = mMosaicImage.scaled(mosaicWinSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsScene* mosaicGraphics = new QGraphicsScene( this );
    mosaicGraphics->addPixmap( QPixmap::fromImage( mosaicImageWin ));
    ui->mosaicView->setBackgroundBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
    ui->mosaicView->setScene( mosaicGraphics);

    QString mosaicText = QString("%1 x %2 bricks (%3 x %4 mm)").arg(mosaicSize.width()).arg(mosaicSize.height()).arg(brickPitch*mosaicSize.width()).arg(brickPitch*mosaicSize.height());
    ui->mosaicLabel->setText(mosaicText);

    mNumCols = mPaletteCount.size() - mPaletteCount.count(0);
    QString countText = QString("%1/%2 palette colours").arg(mNumCols).arg(mPaletteCount.size());
    ui->countLabel->setText(countText);

    ui->actionSave_Mosaic->setEnabled(true);
    ui->actionExport_CSV->setEnabled(false);
}


void MainWindow::renderButtonClicked()
{
unsigned int    brickPitch = 8;

    // calculate size of mosaic in bricks
    QSize mosaicSize = mpMosaic->getSize( mSourceImage );
    QSize mosaicWinSize = ui->mosaicView->size();

    //
    ui->renderButton->setDisabled(true);
    mMosaicImage = mpMosaic->render( mSourceImage, mosaicSize );
    QImage mosaicImageWin = mMosaicImage.scaled(mosaicWinSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsScene* mosaicGraphics = new QGraphicsScene( this );
    mosaicGraphics->addPixmap( QPixmap::fromImage( mosaicImageWin ));
    ui->mosaicView->setBackgroundBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
    ui->mosaicView->setScene( mosaicGraphics);

    QString mosaicText = QString("%1 x %2 bricks (%3 x %4 mm)").arg(mosaicSize.width()).arg(mosaicSize.height()).arg(brickPitch*mosaicSize.width()).arg(brickPitch*mosaicSize.height());
    ui->mosaicLabel->setText(mosaicText);

    mNumCols = mPaletteCount.size() - mPaletteCount.count(0);
    QString countText = QString("%1/%2 palette colours").arg(mNumCols).arg(mPaletteCount.size());
    ui->countLabel->setText(countText);

    ui->actionSave_Mosaic->setEnabled(true);
    ui->actionExport_CSV->setEnabled(true);
}


void MainWindow::on_paletteComboBox_currentIndexChanged(int index)
{
    mParams.setPaletteIndex(index);
    resetRender();
}

void MainWindow::on_sizeComboBox_currentIndexChanged(int index)
{
    mParams.setMaxSize(index);
    ui->progressBar->reset();
    ui->renderButton->setEnabled(true);
}


void MainWindow::on_algoComboBox_currentIndexChanged(int index)
{
    mParams.setAlgorithm(index);
    setSliders();
}


void MainWindow::setSliders()
{
    switch(mParams.getAlgorithm())
    {
    case none:
    case greyscale:
        ui->brightSlider->enable(true);
        ui->contrastSlider->enable(true);
        ui->redSlider->enable(false);
        ui->redSlider->setTitle("");
        ui->greenSlider->enable(false);
        ui->greenSlider->setTitle("");
        ui->blueSlider->enable(false);
        ui->blueSlider->setTitle("");
        ui->mixSlider->enable(false);
        ui->mixSlider->setTitle("");
        ui->randomSlider->enable(true);
        ui->randomSlider->setTitle("Random");
        break;

    case rgb:
        ui->brightSlider->enable(true);
        ui->contrastSlider->enable(true);
        ui->redSlider->enable(true);
        ui->redSlider->setTitle("Red");
        ui->greenSlider->enable(true);
        ui->greenSlider->setTitle("Green");
        ui->blueSlider->enable(true);
        ui->blueSlider->setTitle("Blue");
        ui->mixSlider->enable(false);
        ui->mixSlider->setTitle("");
        ui->randomSlider->enable(true);
        ui->randomSlider->setTitle("Random");
        break;

    case hsv:
        ui->brightSlider->enable(true);
        ui->contrastSlider->enable(true);
        ui->redSlider->enable(true);
        ui->redSlider->setTitle("Hue");
        ui->greenSlider->enable(true);
        ui->greenSlider->setTitle("Saturation");
        ui->blueSlider->enable(true);
        ui->blueSlider->setTitle("Value");
        ui->mixSlider->enable(false);
        ui->mixSlider->setTitle("");
        ui->randomSlider->enable(true);
        ui->randomSlider->setTitle("Random");
        break;

    case hybrid:
        ui->brightSlider->enable(true);
        ui->contrastSlider->enable(true);
        ui->redSlider->enable(true);
        ui->redSlider->setTitle("Red");
        ui->greenSlider->enable(true);
        ui->greenSlider->setTitle("Green");
        ui->blueSlider->enable(true);
        ui->blueSlider->setTitle("Blue");
        ui->mixSlider->enable(true);
        ui->mixSlider->setTitle("Mix");
        ui->randomSlider->enable(true);
        ui->randomSlider->setTitle("Random");
        break;
    }
    ui->progressBar->reset();
    ui->renderButton->setEnabled(true);
}


void MainWindow::on_tileComboBox_currentIndexChanged(int index)
{
    mParams.setTileType(index);
    resetRender();
}


void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog aboutDialog;
    aboutDialog.setModal(true);
    aboutDialog.setIcon( mIconPixmap );
    aboutDialog.exec();
}

void MainWindow::on_actionImport_Palette_triggered()
{
    QString filters("*.csv");
    QString csvFileName = QFileDialog::getOpenFileName(this, "Select Palette File",mPaletteDir, filters );
    if (!csvFileName.isNull())
    {
        importPalette(csvFileName);
    }
}

bool
MainWindow::importPalette(QString csvFileName)
{
    CPalette palette(this);
    QFile paletteFile(csvFileName);
    if( palette.load( &paletteFile ))
    {
        mPaletteList.append(palette);
        ui->paletteComboBox->addItem(palette.getName());
        return true;
    }
    else
    {
        return false;
    }
}

bool
MainWindow::importPalette(QString directory, QString filename)
{
    QString csvFileName(directory + "/" + filename);
    return importPalette(csvFileName);
}


void MainWindow::on_actionPreferences_2_triggered()
{
    PreferencesDialog preferencesDialog;
    preferencesDialog.setModal(true);
    preferencesDialog.showPreferences(mSourceDir,mPaletteDir,mPrefix);

    if( preferencesDialog.exec() == QDialog::Accepted)
    {
        mSourceDir = preferencesDialog.getSourceDir();
        mPaletteDir = preferencesDialog.getPaletteDir();
        mPrefix = preferencesDialog.getPrefix();
        saveSettings();
    }
}


void MainWindow::on_resetButton_clicked()
{
    ui->brightSlider->setValue(50.0);
    ui->contrastSlider->setValue(50.0);
    ui->redSlider->setValue(50.0);
    ui->greenSlider->setValue(50.0);
    ui->blueSlider->setValue(50.0);
    ui->mixSlider->setValue(50.0);
    ui->randomSlider->setValue(0.0);
}

void MainWindow::brightSliderChanged( int newPosition )
{
    mParams.setBrightness(newPosition);
    sourceLightnessContrast();
    resetRender();
}

void MainWindow::contrastSliderChanged( int newPosition )
{
    mParams.setContrast(newPosition);
    sourceLightnessContrast();
    resetRender();
}

void MainWindow::redSliderChanged( int newPosition )
{
    mParams.setRedWeight( newPosition);
    resetRender();
}

void MainWindow::greenSliderChanged( int newPosition )
{
    mParams.setGreenWeight( newPosition);
    resetRender();
}

void MainWindow::blueSliderChanged( int newPosition )
{
    mParams.setBlueWeight( newPosition);
    resetRender();
}

void MainWindow::mixSliderChanged( int newPosition )
{
    mParams.setMixWeight( newPosition );
    resetRender();
}

void MainWindow::randomSliderChanged( int newPosition )
{
    mParams.setRandomise( newPosition );
    resetRender();
}

void
MainWindow::resetRender()
{
    if( mImageLoaded )
    {
        ui->renderButton->setEnabled(true);
        ui->progressBar->reset();
    }
}

void
MainWindow::updateProgressBar(float percentComplete)
{
    ui->progressBar->setValue(percentComplete);
}

void
MainWindow::sourceLightnessContrast()
{
    QSize sourceWinSize = ui->sourceImageView->size();
    QImage sourceImageWin = mSourceImage.scaled(sourceWinSize, Qt::KeepAspectRatio);

    mpMosaic->applyBrightnessContrast(&sourceImageWin);

    QGraphicsScene* sourceGraphics = new QGraphicsScene( this );
    sourceGraphics->addPixmap( QPixmap::fromImage( sourceImageWin ));
    ui->sourceImageView->setBackgroundBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
    ui->sourceImageView->setScene(sourceGraphics);
}


// from https://stackoverflow.com/questions/27318631/parsing-through-a-csv-file-in-qt
QStringList
MainWindow::parseCSV(const QString &string)
{
    enum State {Normal, Quote} state = Normal;
    QStringList fields;
    QString value;

    for (int i = 0; i < string.size(); i++)
    {
        const QChar current = string.at(i);

        // Normal state
        if (state == Normal)
        {
            // Comma
            if (current == ',')
            {
                // Save field
                fields.append(value.trimmed());
                value.clear();
            }

            // Double-quote
            else if (current == '"')
            {
                state = Quote;
                value += current;
            }

            // Other character
            else
                value += current;
        }

        // In-quote state
        else if (state == Quote)
        {
            // Another double-quote
            if (current == '"')
            {
                if (i < string.size())
                {
                    // A double double-quote?
                    if (i+1 < string.size() && string.at(i+1) == '"')
                    {
                        value += '"';

                        // Skip a second quote character in a row
                        i++;
                    }
                    else
                    {
                        state = Normal;
                        value += '"';
                    }
                }
            }

            // Other character
            else
                value += current;
        }
    }

    if (!value.isEmpty())
        fields.append(value.trimmed());

    // Quotes are left in until here; so when fields are trimmed, only whitespace outside of
    // quotes is removed.  The outermost quotes are removed here.
    for (int i=0; i<fields.size(); ++i)
        if (fields[i].length()>=1 && fields[i].left(1)=='"')
        {
            fields[i]=fields[i].mid(1);
            if (fields[i].length()>=1 && fields[i].right(1)=='"')
                fields[i]=fields[i].left(fields[i].length()-1);
        }

    return fields;
}

