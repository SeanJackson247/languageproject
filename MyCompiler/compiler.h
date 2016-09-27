#ifndef COMPILER_H
#define COMPILER_H

#include <QObject>

#include"utility.h"

class Compiler:public QObject{
    Q_OBJECT
public:Compiler();
public slots:
    void doWork(QString fileName, QVector<QString> data);
signals:
    void workCompleted(QString,QByteArray);
    void debugScript(QString,QString);
};

#endif // COMPILER_H
