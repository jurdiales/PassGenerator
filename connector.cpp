#include "connector.h"

Connector::Connector(QObject *parent)
    : QObject{parent}
{}

Connector::Connector(QQmlApplicationEngine *engine, QObject *parent)
    : Connector(parent)
{
    auto root = engine->rootObjects();
    generator = dynamic_cast<Generator*>(root[0]->findChild<QObject*>("generator"));
    passwordTextField = root[0]->findChild<QObject*>("generatedPassword");
    weakProgressBar = root[0]->findChild<QObject*>("weakbar");
    mediumProgressBar = root[0]->findChild<QObject*>("mediumbar");
    strongProgressBar = root[0]->findChild<QObject*>("strongbar");
    popup = root[0]->findChild<QObject*>("popup");

    QObject::connect(generator, &Generator::passwordGenerated, this, &Connector::setPasswordText);
    QObject::connect(generator, &Generator::entropyCalculated, this, &Connector::setEntropyBar);
}

void Connector::setPasswordText(QString password)
{
    if (passwordTextField) {
        passwordTextField->setProperty("text", QVariant(password));
        generator->copy(password);
    }
}

void Connector::setEntropyBar(int entropy)
{
    // minimum: 4 numbers -> 14 bits
    // maximum: 32 combination of 4 options -> 205 bits
    // each bar has 0 - 100 range -> [14, 205] -> [0, 300]
    // weak: [0-35] medium: [36-64] strong: [65-128]
    if (entropy <= 35) {
        int value = static_cast<int>(entropy / 35.0 * 100.0);
        weakProgressBar->setProperty("value", QVariant(value));
        mediumProgressBar->setProperty("value", QVariant(0));
        strongProgressBar->setProperty("value", QVariant(0));
        generator->opt.entropy = Generator::WEAK;
    } else if (entropy <= 64) {
        int value = static_cast<int>((entropy - 36) / (64.0 - 36.0) * 100.0);
        weakProgressBar->setProperty("value", QVariant(100));
        mediumProgressBar->setProperty("value", QVariant(value));
        strongProgressBar->setProperty("value", QVariant(0));
        generator->opt.entropy = Generator::MEDIUM;
    } else {
        int value = static_cast<int>((entropy - 65) / (128.0 - 65.0) * 100.0);
        weakProgressBar->setProperty("value", QVariant(100));
        mediumProgressBar->setProperty("value", QVariant(100));
        strongProgressBar->setProperty("value", QVariant(value));
        generator->opt.entropy = Generator::STRONG;
    }
}
