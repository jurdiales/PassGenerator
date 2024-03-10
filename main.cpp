#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QFontDatabase>
#include <QQmlContext>
#include <QIcon>

#include "generator.h"
#include "connector.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon("favicon.svg"));

    QQmlApplicationEngine engine;
    QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    fixedFont.setPointSize(12);
    fixedFont.setWeight(QFont::DemiBold);
    engine.rootContext()->setContextProperty("fixedFont", fixedFont);
    qmlRegisterType<Generator>("PassGenerator.Generator", 1, 0, "Generator");

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("PassGenerator", "Main");

    Connector* connector = new Connector(&engine);
    QObject::connect(&engine, &QQmlApplicationEngine::destroyed, connector, &Connector::deleteLater);

    return app.exec();
}
