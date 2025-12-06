#ifndef QSLIDERPLUS_H
#define QSLIDERPLUS_H

#include <QWidget>

namespace Ui {
class QSliderPlus;
}

class QSliderPlus : public QWidget
{
    Q_OBJECT

public:
    explicit QSliderPlus(QWidget *parent = nullptr);
    ~QSliderPlus();

    void setup( QString title, float minValue, float maxValue);
    void setup( QString title, QString leftLabel, QString rightLabel);
    void setValue( int position );
    void setTitle( QString title );
    void enable( bool state);

signals:
    void valueChanged( int newPosition );


private slots:
    void on_slider_sliderMoved(int position);

private:
    Ui::QSliderPlus *ui;

    float   mValue;
    float   mMinValue;
    float   mMaxValue;
    QString mTitle;
};

#endif // QSLIDERPLUS_H
