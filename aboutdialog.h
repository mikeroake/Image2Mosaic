#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();
    void setIcon( QPixmap icon );

private slots:
    void on_closeButton_clicked();

private:
    Ui::AboutDialog *ui;
    QPixmap mIcon;
};

#endif // ABOUTDIALOG_H
