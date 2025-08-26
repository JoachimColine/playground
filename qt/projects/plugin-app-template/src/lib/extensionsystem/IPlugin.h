#include <QtPlugin>
#include <QString>

namespace JApp {

class IPlugin
{
public:
    virtual ~IPlugin() = default;
    virtual QString name() const = 0;
    virtual QString version() const = 0;
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
};

Q_DECLARE_INTERFACE(IPlugin, "japp.IPlugin/1.0")

}