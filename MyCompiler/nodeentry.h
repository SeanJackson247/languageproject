#ifndef NODEENTRY_H
#define NODEENTRY_H

#include<QString>

#include<QVector>
#include<QDebug>

class NodeEntry{
public: NodeEntry* parent;
public: QVector<NodeEntry*> childs;
public: int indentation;
public: NodeEntry(int indentation);
public: NodeEntry(int indentation, NodeEntry *parent);
public: QVector<QString> lines;
public: void addLine(QString line);
public: void addChild(int currentIndentation,QString line);
public: void print();
public: QByteArray compile();
};

#endif // NODEENTRY_H
