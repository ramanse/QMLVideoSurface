#include "icentertainmentplugin.h"
#include <QQmlEngine>
void ICEntertainmentPlugin::registerTypes(const char *uri)
{

    Q_ASSERT(uri == QLatin1String("ICEntertainmentPlugin"));
    qmlRegisterType<ICEntertainmentHandler>(uri, 1, 0, "HuEntertainment");

}
