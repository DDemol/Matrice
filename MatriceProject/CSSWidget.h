#ifndef CSSWIDGET_H
#define CSSWIDGET_H

#include <QString>

class CSSWidget
{
public:
    CSSWidget();

    static QString getButtonStyleSheet(QString iconDir);
    static QString getSpinBoxStyleSheet();
};

#endif // CSSWIDGET_H
