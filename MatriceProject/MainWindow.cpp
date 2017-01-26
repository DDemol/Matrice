#include "MainWindow.h"

MainWindow::MainWindow():QMainWindow()
{

}

QString getButtonStyleSheet(QString iconDir)
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

QString getSpinBoxStyleSheet()
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


void MainWindow::setScene(GraphicsScene *scene)
{
    this->_scene = scene;
    _view = new QGraphicsView(_scene);

    createActions();
    createMenus();

    palette = new ColorPaletteWidget();

    tree = new TreeWidget(&_scene->getItem());

    QButtonGroup* group = new QButtonGroup();

    draw = new QPushButton();
    edit = new QPushButton();
    recognize = new QPushButton();

    hbox = new QHBoxLayout();
    hbox->addWidget(draw);
    hbox->addWidget(edit);
    hbox->addWidget(recognize);

    group->addButton(draw);
    group->addButton(edit);
    group->addButton(recognize);

    draw->setCheckable(true);
    edit->setCheckable(true);
    recognize->setCheckable(true);

    draw->setChecked(true);

    hbox->addWidget(new QLabel("<img src=\":/Resources/separator.png\">"));

    hbox->addWidget(palette);
    hbox->addStretch(1);

    hbox2 = new QHBoxLayout();
    hbox2->addWidget(_view,4);
    hbox2->addWidget(tree,1);


    vbox = new QVBoxLayout();
    vbox->addLayout(hbox);
    vbox->addLayout(hbox2);


    screen = new QWidget(this);
    screen->setLayout(vbox);

    setCentralWidget(screen);

    draw->setStyleSheet(getButtonStyleSheet(":/Resources/draw.png"));
    edit->setStyleSheet(getButtonStyleSheet(":/Resources/edit.png"));
    recognize->setStyleSheet(getButtonStyleSheet(":/Resources/gesture.png"));
    palette->getAllColorButton()->setStyleSheet(getButtonStyleSheet(":/Resources/color.png"));
    palette->getSizeButton()->setStyleSheet(getSpinBoxStyleSheet());

    QWidget::connect(draw, SIGNAL(clicked()), _scene, SLOT(setDraw()));
    QWidget::connect(edit, SIGNAL(clicked()), _scene, SLOT(setEdit()));
    QWidget::connect(recognize, SIGNAL(clicked()), _scene, SLOT(setRecognize()));
    QWidget::connect(tree->getTree(), SIGNAL(selectedRow(QString)) , _scene, SLOT(setSelectedRow(QString)));
    QWidget::connect(tree->getTree(), SIGNAL(selectedRows(QStringList)), _scene, SLOT(setSelectedRows(QStringList)));
    QWidget::connect(tree->getTree(),SIGNAL(sendNewName(QString,QString)),_scene,SLOT(changedName(QString,QString)));
    QWidget::connect(palette, SIGNAL(sizePenClicked(int)),_scene,SLOT(setSizePen(int)));


    QWidget::connect(this, SIGNAL(resetTree(QList <CurveItem>)), tree->getTree() , SLOT(removeAll(QList <CurveItem>)));
    QWidget::connect(_scene,SIGNAL(newItemSignal(CurveItem)),tree->getTree(),SLOT(insertRow(CurveItem)));
    QWidget::connect(_scene,SIGNAL(removeItemSignal(CurveItem)),tree->getTree(),SLOT(removeRow(CurveItem)));


    QWidget::connect(palette->getAllColorButton(),SIGNAL(clicked()),this,SLOT(allColor()));
    QWidget::connect(palette->getRedButton(),SIGNAL(clicked()),this,SLOT(redButton()));
    QWidget::connect(palette->getOrangeButton(),SIGNAL(clicked()),this,SLOT(orangeButton()));
    QWidget::connect(palette->getYellowButton(),SIGNAL(clicked()),this,SLOT(yellowButton()));
    QWidget::connect(palette->getGreenButton(),SIGNAL(clicked()),this,SLOT(greenButton()));
    QWidget::connect(palette->getBlueButton(),SIGNAL(clicked()),this,SLOT(blueButton()));
    QWidget::connect(palette->getPurpleButton(),SIGNAL(clicked()),this,SLOT(purpleButton()));
    QWidget::connect(palette->getBrownButton(),SIGNAL(clicked()),this,SLOT(brownButton()));
    QWidget::connect(palette->getWhiteButton(),SIGNAL(clicked()),this,SLOT(whiteButton()));
    QWidget::connect(palette->getGrayButton(),SIGNAL(clicked()),this,SLOT(grayButton()));
    QWidget::connect(palette->getBlackButton(),SIGNAL(clicked()),this,SLOT(blackButton()));

}

void MainWindow::allColor(){_scene->setColorPen(QColorDialog::getColor(Qt::black));}

void MainWindow::redButton(){ _scene->setColorPen(QColor(255, 26, 26));}

void MainWindow::orangeButton(){_scene->setColorPen(QColor(255, 102, 0));}

void MainWindow::yellowButton(){_scene->setColorPen(QColor(230, 230, 0));}

void MainWindow::greenButton(){_scene->setColorPen(QColor(0, 153, 0));}

void MainWindow::blueButton(){_scene->setColorPen(QColor(0,0,255));}

void MainWindow::purpleButton(){_scene->setColorPen(QColor(179, 0, 179));}

void MainWindow::brownButton(){_scene->setColorPen(QColor(153, 51, 0));}

void MainWindow::whiteButton(){_scene->setColorPen(QColor(255, 255, 255));}

void MainWindow::grayButton(){_scene->setColorPen(QColor(128, 128, 128));}

void MainWindow::blackButton(){_scene->setColorPen(QColor(0, 0, 0));}

void MainWindow::createActions()
{
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    quitAct = new QAction(tr("&Quit"),this);
    quitAct->setShortcut(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(quit()));

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);
}

void MainWindow::newFile()
{
    if((!_scene->isSaved()))
    {
        int message = QMessageBox(QMessageBox::Information, "NewFile", "Are you sure you want to open new file", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel).exec();

        if (QMessageBox::Yes == message)
            save();

        else if(QMessageBox::Cancel == message)
            return;

    }
    emit resetTree(_scene->getItem());
    _scene->RemoveAll();
    _scene->ResetParameters();
}

void MainWindow::open()
{
    if(! _scene->isSaved())
    {
        int message = QMessageBox(QMessageBox::Information, "Open", "Are you sure you want to open", QMessageBox::Yes|QMessageBox::Save|QMessageBox::No).exec();
        if(QMessageBox::Save == message)
        {
            save();
        }
        else if(QMessageBox::No == message)
            return;
    }
    emit resetTree(_scene->getItem());
    _scene->RemoveAll();
    _scene->ResetParameters();

    filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C://",
                "Text File (*.txt)"
                );

    QFile f(filename);
    if(f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QList<CurveItem> listC;

        QTextStream in(&f);
        while(!in.atEnd())
        {
            QString line = in.readLine();
            CurveItem curve;

            curve.setName(line);

            line = in.readLine();

            QStringList colorS = line.split("/");
            curve.setColor(QColor(colorS[0].toInt(), colorS[1].toInt(), colorS[2].toInt()));

            line = in.readLine();

            while( line.compare("-"))
            {
                QStringList listS = line.split("/");
                QPointF point = QPointF(listS[0].toDouble(),listS[1].toDouble());
                curve.addPoint(point);

                line = in.readLine();
            }
            listC.push_back(curve);
        }

        _scene->setItem(listC);
        _scene->drawAllCurves();
        f.close();
    }
}

void MainWindow::save()
{
    if(filename.isEmpty())
    {
        filename = QFileDialog::getSaveFileName(
                    this,
                    tr("Open File"),
                    "C://",
                    "Text File (*.txt)"
                    );
    }

    QFile f( filename);
    if(f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        _scene->setBackUpStatus(true);
        QTextStream out(&f);

        for(CurveItem c : _scene->getItem())
        {
            out << c.getName() << endl;
            out << c.getColor().red() << "/" << c.getColor().green() << "/" << c.getColor().blue() << endl;
            for(QPointF p :  c.getPoints())
                out << p.x() << "/" << p.y() << endl;

            out << "-" << endl;
        }
        f.flush();
        f.close();
    }
}

void MainWindow::quit()
{
    if(_scene->isSaved())
    {
        if (QMessageBox::Yes == QMessageBox(QMessageBox::Information, "Quit", "Are you sure you want to exit", QMessageBox::Yes|QMessageBox::No).exec())
            qApp->exit();
    }
    else
    {
        int message = QMessageBox(QMessageBox::Information, "Quit", "Are you sure you want to exit", QMessageBox::Yes|QMessageBox::Save|QMessageBox::No).exec();
        if(QMessageBox::Save == message)
            save();

        if((QMessageBox::Save == message) || (QMessageBox::Yes == message))
            qApp->exit();
    }
}
