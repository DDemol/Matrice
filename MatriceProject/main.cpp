#include <iostream>
#include <signal.h>

#include "MainWindow.h"
#include "GraphicsScene.h"
#include "LgEnvironment.h"
#include "OneDollarRecognizerObserver.h"

using namespace std;
using namespace lg;

bool finish = false;

void killHandler (int param)
{
    finish = true;
}

int main(int argc, char *argv[])
{
#ifdef SIGBREAK
    signal (SIGBREAK, killHandler);
#endif
    signal (SIGINT, killHandler);
    signal (SIGABRT, killHandler);
    signal (SIGTERM, killHandler);

    QApplication app(argc, argv);
    MainWindow win;

    // Environment declaration
    Environment* environment = new Environment();
    environment->enableDataCopy(false);

    // HERE, declare your generators/processors/observers and add it to the environment
    GraphicsScene *qtGenerator = new GraphicsScene("QtGen");
    win.setScene(qtGenerator);

    if(environment->registerNode(qtGenerator))
        cerr << "Register TuioGenerator" << endl;
    else
        cout << environment->getLastError().c_str() << endl;
    /* One Dollar */


    OneDollarRecognizerObserver* one = new OneDollarRecognizerObserver("OneDollar");
    one->setScene(qtGenerator);

    if(environment->registerNode(one))
        cerr << "Register OneDollar" << endl;
    else
        cout << environment->getLastError().c_str() << endl;

    // Check if your generators/processors/observers are commpatible
    if(!environment->checkCompatibility()){
        cerr << "Nodes are not compatible." << endl;
        return 2;
    }

    // Check if you started the environment
    if(!environment->start()){
        cerr << "Environnement not started." << endl;
        return 2;
    }

    win.showMaximized();
    app.exec();
/*
    while(!finish)
    {
        environment->update();
    }*/

    environment->stop();
    delete environment;
}
