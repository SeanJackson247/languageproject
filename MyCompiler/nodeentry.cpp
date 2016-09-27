#include "nodeentry.h"
#include <QString>

NodeEntry::NodeEntry(int indentation){
    this->parent=NULL;
    this->indentation = indentation;
}

NodeEntry::NodeEntry(int indentation,NodeEntry* parent){
    this->parent=parent;
    this->indentation = indentation;
}

void NodeEntry::addLine(QString line){
    qDebug()<<"NodeEntry is adding line...";
    this->lines.resize(this->lines.size()+1);
    this->lines[this->lines.size()-1] = line;
    qDebug()<<"line added";
}
void NodeEntry::addChild(int currentIndentation,QString line){
    qDebug()<<"Adding child...";
    if((indentation+1)==currentIndentation){
        this->childs.resize(this->childs.size()+1);
        this->childs[this->childs.size()-1] = new NodeEntry(currentIndentation,this);
    }
    else{

    }
    qDebug()<<"Child Added.";
}
void NodeEntry::print(){
    qDebug() << "Lines:";
    int i=0;
    while(i<this->lines.size()){
        qDebug() << this->lines[i];
        i++;
    }
}
QByteArray NodeEntry::compile(){
    QByteArray res;
    return res;
}
