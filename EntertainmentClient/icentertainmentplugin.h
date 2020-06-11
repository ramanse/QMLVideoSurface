#ifndef ICENTERTAINMENTPLUGIN_H
#define ICENTERTAINMENTPLUGIN_H
#include <QQmlExtensionPlugin>
#include "icentertainmenthandler.h"

class ICEntertainmentPlugin: public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
public:
    void registerTypes(const char *uri) override;
};

#endif // ICENTERTAINMENTPLUGIN_H
