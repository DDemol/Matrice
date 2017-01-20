#include "ColorPaletteWidget.h"

ColorPaletteWidget::ColorPaletteWidget(): QWidget(0,Qt::FramelessWindowHint)
{
    initButton();
    initConnect();

    _colorLayout = new QGridLayout();

    _colorLayout->addWidget(red,0,0);
    _colorLayout->addWidget(orange,0,1);
    _colorLayout->addWidget(yellow,0,2);
    _colorLayout->addWidget(green,0,3);
    _colorLayout->addWidget(blue,0,4);
    _colorLayout->addWidget(purple,1,0);
    _colorLayout->addWidget(brown,1,1);
    _colorLayout->addWidget(white,1,2);
    _colorLayout->addWidget(gray,1,3);
    _colorLayout->addWidget(black,1,4);

    _paletteLayout = new QHBoxLayout();

    _paletteLayout->addWidget(_size);
    _paletteLayout->addWidget(_allColor);
    _paletteLayout->addLayout(_colorLayout);
    _paletteLayout->addStretch(1);

    setLayout(_paletteLayout);
}

void ColorPaletteWidget::initButton()
{
    _size = new QSpinBox();
    _size->setValue(1);

    _allColor = new QPushButton();

    red = new QPushButton();
    red->setStyleSheet("background-color: red;");

    orange = new QPushButton();
    orange->setStyleSheet("background-color: orange;");

    yellow = new QPushButton();
    yellow->setStyleSheet("background-color: yellow;");

    green = new QPushButton();
    green->setStyleSheet("background-color: green;");

    blue = new QPushButton();
    blue->setStyleSheet("background-color: blue;");

    purple = new QPushButton();
    purple->setStyleSheet("background-color: purple;");

    brown = new QPushButton();
    brown->setStyleSheet("background-color: brown;");

    white = new QPushButton();
    white->setStyleSheet("background-color: white;");

    gray = new QPushButton();
    gray->setStyleSheet("background-color: gray;");

    black = new QPushButton();
    black->setStyleSheet("background-color: black;");
}

void ColorPaletteWidget::initConnect()
{
    connect(_size, SIGNAL(valueChanged(int)), this, SLOT(sizePenButtonClicked(int)));

}

QPushButton *ColorPaletteWidget::getAllColorButton()
{
    return _allColor;
}

QPushButton *ColorPaletteWidget::getRedButton()
{
    return red;
}

QPushButton *ColorPaletteWidget::getOrangeButton()
{
    return orange;
}

QPushButton *ColorPaletteWidget::getYellowButton()
{
    return yellow;
}

QPushButton *ColorPaletteWidget::getGreenButton()
{
    return green;
}

QPushButton *ColorPaletteWidget::getBlueButton()
{
    return blue;
}

QPushButton *ColorPaletteWidget::getPurpleButton()
{
    return purple;
}

QPushButton *ColorPaletteWidget::getBrownButton()
{
    return brown;
}

QPushButton *ColorPaletteWidget::getWhiteButton()
{
    return white;
}

QPushButton *ColorPaletteWidget::getGrayButton()
{
    return gray;
}

QPushButton *ColorPaletteWidget::getBlackButton()
{
    return black;
}

QSpinBox *ColorPaletteWidget::getSizeButton()
{
    return _size;
}

void ColorPaletteWidget::sizePenButtonClicked(int value)
{

    if(value <= 0)
    {
        _size->setValue(1);
        emit sizePenClicked(1);
        return;
    }

    emit sizePenClicked(value);
}
