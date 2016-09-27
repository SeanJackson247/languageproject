#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QObject>

class FileWriter:public QObject{
    Q_OBJECT
public: QString dir;
public: FileWriter(QString outputDirectory);
public slots:
    void writeRequest(QString filename, QByteArray contents);
    void debugRequest(QString filename, QString contents);
};

#endif // FILEWRITER_H
