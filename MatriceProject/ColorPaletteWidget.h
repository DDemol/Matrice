#ifndef COLORPALETTEWIDGET_H
#define COLORPALETTEWIDGET_H

#include <QPushButton>
#include <QGridLayout>
#include <QWidget>


class ColorPaletteWidget : public QWidget
{
    Q_OBJECT
public:
    ColorPaletteWidget();

    QPushButton *getAllColorButton();
    QPushButton *getRedButton();
    QPushButton *getOrangeButton();
    QPushButton *getYellowButton();
    QPushButton *getGreenButton();
    QPushButton *getBlueButton();
    QPushButton *getPurpleButton();
    QPushButton *getBrownButton();
    QPushButton *getWhiteButton();
    QPushButton *getGrayButton();
    QPushButton *getBlackButton();

public slots:
    void showPalette();

private:
    void initButton();

    QPushButton *_allColor;

    QPushButton *red;
    QPushButton *orange;
    QPushButton *yellow;
    QPushButton *green;
    QPushButton *blue;
    QPushButton *purple;
    QPushButton *brown;
    QPushButton *white;
    QPushButton *gray;
    QPushButton *black;

    QGridLayout *_colorLayout;
    QHBoxLayout *_paletteLayout;

};

#endif // COLORPALETTEWIDGET_H
