#include <QQmlApplicationEngine>
#include "myapplication.h"

int main(int argc, char *argv[])
{
    MyApplication app(argc, argv, "MaxKazakov", "Spectrum");

    QQmlApplicationEngine* engine = new QQmlApplicationEngine();
    app.SetQmlEngine(engine);
    engine->load(QUrl(QStringLiteral("qrc:/resources/main.qml")));

    app.Init();

    return app.exec();
}
