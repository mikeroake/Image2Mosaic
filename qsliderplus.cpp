#include "qsliderplus.h"
#include "ui_qsliderplus.h"

QSliderPlus::QSliderPlus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QSliderPlus)
{
    ui->setupUi(this);
}

QSliderPlus::~QSliderPlus()
{
    delete ui;
}

void
QSliderPlus::setup( QString title, float minValue, float maxValue)
{
    mTitle = title;
    mMinValue = minValue;
    mMaxValue = maxValue;
    ui->label->setText(mTitle);
    ui->minLabel->setText(QString::number(mMinValue));
    ui->maxLabel->setText(QString::number(mMaxValue));
    setValue(50.0);
}


void
QSliderPlus::setup( QString title, QString leftLabel, QString rightLabel)
{
    mTitle = title;
    mMinValue = 0.0;
    mMaxValue = 1.0;
    ui->label->setText(mTitle);
    ui->minLabel->setText(leftLabel);
    ui->maxLabel->setText(rightLabel);
    setValue(50.0);
}

void
QSliderPlus::setTitle( QString title )
{
    mTitle = title;
    ui->label->setText(mTitle);
}

void
QSliderPlus::setValue( int position )
{
    mValue =  mMinValue + position*(mMaxValue - mMinValue) / 100.0;
    ui->valueLabel->setText(QString::number(mValue));
    ui->slider->setValue(position);
    emit valueChanged( position );
}


void
QSliderPlus::enable( bool state)
{
    ui->slider->setEnabled(state);
    ui->label->setEnabled(state);
    ui->valueLabel->setEnabled(state);
    ui->minLabel->setEnabled(state);
    ui->maxLabel->setEnabled(state);
}

void QSliderPlus::on_slider_sliderMoved(int position)
{
    mValue =  mMinValue + ((float) position) * (mMaxValue - mMinValue) / 100.0;
    ui->valueLabel->setText(QString::number(mValue));
    emit valueChanged( position );
}

