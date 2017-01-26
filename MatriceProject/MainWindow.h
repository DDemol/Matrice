#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QMainWindow>
#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QComboBox>
#include <QColorDialog>
#include <QButtonGroup>
#include <QLayout>
#include <qmessagebox.h>

#include "GraphicsScene.h"
#include "ColorPaletteWidget.h"
#include "Tree/TreeWidget.h"
#include "Tree/TreeModel.h"

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    void setScene(GraphicsScene *scene);

public slots:
    void allColor();
    void redButton();
    void orangeButton();
    void yellowButton();
    void greenButton();
    void blueButton();
    void purpleButton();
    void brownButton();
    void whiteButton();
    void grayButton();
    void blackButton();

signals:
    void resetTree(QList <CurveItem> l);

private:
    void createActions();
    void createMenus();

    QGraphicsView* _view;
    GraphicsScene* _scene;

    QMenu *fileMenu;
    QActionGroup *alignmentGroup;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *quitAct;

    QPushButton *draw;
    QPushButton *edit;
    QPushButton *recognize;

    ColorPaletteWidget *palette;

    TreeWidget *tree;
    TreeModel *model;

    QHBoxLayout *hbox;
    QHBoxLayout *hbox2;
    QVBoxLayout *vbox;

    QWidget *screen;

    QString filename;

private slots:
    void newFile();
    void open();
    void save();
    void quit();

};

#endif // MAINWINDOW_H
