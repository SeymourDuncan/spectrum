#include <QQmlApplicationEngine>
#include "myapplication.h"

#include "modeldata.h"

int main(int argc, char *argv[])
{
    MyApplication app(argc, argv, "MaxKazakov", "Spectrum");

    QQmlApplicationEngine* engine = new QQmlApplicationEngine();
    app.SetQmlEngine(engine);

    qmlRegisterType<QSpectrumValuesModel>("com.types.datamodel", 1, 0, "SpectrumValuesModel");
    engine->load(QUrl(QStringLiteral("qrc:/resources/main.qml")));

    app.Init();



    return app.exec();
}
