#include "filewriter.h"

#include <QFile>
#include <QTextStream>

FileWriter::FileWriter(QString outputDirectory){
    this->dir = outputDirectory;
}

void FileWriter::writeRequest(QString filename, QByteArray contents){
    QFile file(this->dir+filename);
    if (file.open(QIODevice::ReadWrite)) {
       QTextStream stream(&file);
       stream << contents;
    }
}

void FileWriter::debugRequest(QString filename,QString contents){
    QFile file(this->dir+filename);
    if (file.open(QIODevice::WriteOnly)) {
       QTextStream stream(&file);
       stream << contents;
    }
    file.close();
}
