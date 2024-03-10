#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>
#include <QVariant>
#include <QQmlApplicationEngine>
#include "generator.h"

class Connector : public QObject
{
    Q_OBJECT
public:
    explicit Connector(QObject *parent = nullptr);
    Connector(QQmlApplicationEngine* engine, QObject *parent = nullptr);

public slots:
    void setPasswordText(QString password);
    void setEntropyBar(int entropy);

private:
    Generator* generator;
    QObject* passwordTextField;
    QObject* weakProgressBar;
    QObject* mediumProgressBar;
    QObject* strongProgressBar;
    QObject* popup;
};

#endif // CONNECTOR_H
