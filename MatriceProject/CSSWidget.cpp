#include "CSSWidget.h"

CSSWidget::CSSWidget()
{
}

QString CSSWidget::getButtonStyleSheet(QString iconDir)
{
    return "QPushButton "
            "{"
            "border-radius: 6px;"
            "image:url(" + iconDir + ");"
            "min-height: 2em;"
            "min-width: 2em;"
            "max-height: 2em;"
            "max-width: 2em;"
            "padding: 6px;"
            "}"
            "QPushButton:checked "
            "{"
            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "stop: 0 #787878, stop: 0.9 #B4B4B4,"
            "stop: 0.9 #B4B4B4, stop: 1.0 #D6D6D6);"
            "}"
            "QPushButton:pressed"
            "{"
            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "stop: 0 #959595, stop: 0.9 #C6C6C6,"
            "stop: 0.9 #C6C6C6, stop: 1.0 #E8E8E8);"
            "}"
            ;
}

QString CSSWidget::getSpinBoxStyleSheet()
{
    return "QSpinBox "
            "{"
            "border-radius: 6px;"
            "min-height: 2em;"
            "min-width: 3em;"
            "max-height: 2em;"
            "max-width: 2em;"
            "padding: 6px;"
            "background-color: #F0F0F0;"
            "}"
            "QSpinBox::up-button { width: 20px; }"
            "QSpinBox::down-button { width: 20px; }"
            ;
}
