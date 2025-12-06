#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    QChar copyrightSymbol(0x00A9);
    QString copyrightText = "Copyright "+ (QString) copyrightSymbol + " 2022";
    ui->copyrightLabel->setText(copyrightText);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void
AboutDialog::setIcon( QPixmap icon )
{
    mIcon = icon;
    ui->iconLabel->setPixmap(icon);
}

void AboutDialog::on_closeButton_clicked()
{
    this->close();
}

