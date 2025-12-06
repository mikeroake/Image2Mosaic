#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDebug>


namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    ~PreferencesDialog();

    void    showPreferences(QString sourceDir,QString paletteDir,QString prefix);
    QString getSourceDir();
    QString getPaletteDir();
    QString getPrefix();

private slots:
    void on_soureDirButton_clicked( );

    void on_paletteDirButton_clicked();

private:
    Ui::PreferencesDialog *ui;
    QString mSourceDir;
    QString mPaletteDir;
    QString mPrefix;
};

#endif // PREFERENCESDIALOG_H
