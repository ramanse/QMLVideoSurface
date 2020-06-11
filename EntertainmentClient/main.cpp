#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "icentertainmenthandler.h"
#include "qmlsimuvideostream.h"
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    //ICEntertainmentHandler handler;
    qmlRegisterType<ICEntertainmentHandler>("Entertainment", 1, 0, "HuEntertainment");
    qmlRegisterType<QmlSimuVideoStream>("Entertainment", 1, 0, "SimuVideo");


    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;
    //    QQmlContext *context = engine.rootContext();

    //    context->setContextProperty("HuEntertainment", &handler);


    return app.exec();
}
