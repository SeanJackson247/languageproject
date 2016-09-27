#include "filereader.h"

#include <QFile>
#include <QDebug>

FileReader::FileReader(QString inputDirectory){
    this->dir = inputDirectory;
}

void FileReader::loadFile(QString fileName){
    QFile file(this->dir+fileName);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "error opening file: " << file.error();
    }
    QTextStream instream(&file);
    QVector<QString> data;
    int i=0;
    while(!instream.atEnd()){
        QString line = instream.readLine();
        i++;
        data.resize(i);
        data[i-1]=line;
    }
    file.close();
    emit readFile(fileName,data);
}

void FileReader::loadBytesFromFile(QString fileName){
    QFile file(this->dir+fileName);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "error opening file: " << file.error();
    }
    QByteArray data = file.readAll();

    file.close();
    emit readBytesFromFile(fileName,data);
}
