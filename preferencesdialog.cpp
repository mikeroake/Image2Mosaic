#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}


void
PreferencesDialog::showPreferences(QString sourceDir,QString paletteDir,QString prefix)
{
    mSourceDir = sourceDir;
    ui->sourceDirPref->setText(mSourceDir);
    mPaletteDir = paletteDir;
    ui->paletteDirPref->setText(mPaletteDir);
    mPrefix = prefix;
    ui->prefixPref->setText(mPrefix);
}


void PreferencesDialog::on_soureDirButton_clicked( )
{
    mSourceDir = QFileDialog::getExistingDirectory( this, "Select Source/Mosaic Directory", mSourceDir, QFileDialog::ShowDirsOnly );
    ui->sourceDirPref->setText(mSourceDir);
}


void PreferencesDialog::on_paletteDirButton_clicked()
{
    mPaletteDir = QFileDialog::getExistingDirectory( this, "Select Palette Directory", mPaletteDir, QFileDialog::ShowDirsOnly );
    ui->paletteDirPref->setText(mPaletteDir);
}


QString
PreferencesDialog::getSourceDir()
{
    return mSourceDir;
}

QString
PreferencesDialog::getPaletteDir()
{
    return mPaletteDir;
}

QString
PreferencesDialog::getPrefix()
{
    mPrefix = ui->prefixPref->text();
    // remove non-alphanumeric characters except '_'
    mPrefix = mPrefix.remove(QRegularExpression("[" + QRegularExpression::escape("-`~!@#$%^&*()—+=|:;<>«»,.?/{}\'\"") + "]"));
    // remove white space
    return mPrefix.simplified().remove(' ');
}

