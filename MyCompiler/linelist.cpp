#include "linelist.h"

#include "line.h"
#include "tokenlist.h"

#include <QDebug>

QVector<LineList*> LineList::LineListsRegister={};

void LineList::clearLineListsRegister(){
    int i=0;
    while(i<LineList::LineListsRegister.size()){
        delete LineList::LineListsRegister[i];
        i++;
    }
    LineList::LineListsRegister.resize(0);
}

LineList::LineList(){
    LineList::registerLineList(this);
}

void LineList::registerLineList(LineList* line){
    LineList::LineListsRegister.resize(LineList::LineListsRegister.size()+1);
    LineList::LineListsRegister[LineList::LineListsRegister.size()-1]=line;
}
void LineList::add(Line* l){
    this->_data.resize(this->_data.size()+1);
    if(this->_data.size()>0){
        this->_data[this->_data.size()-1]=l;
    }
    else{
        qDebug()<<"ERROR!!!Attempt to Add to LineList fail.";
    }
}
void LineList::addFromList(LineList* l){
    int i=0;
    while(i<l->size()){
        this->add(l->get(i));
        i++;
    }
}
Line* LineList::first(){
    if(this->_data.size()>0){
        return this->_data[0];
    }
    else{
        qDebug()<<"ERROR!! - attempted access of first member of empty array.";
        return NULL;
    }
}
void LineList::clearChilds(int currentIndex){
    this->_data[currentIndex]->childs = new LineList();
}
Line* LineList::last(){
    if(this->_data.size()>0){
        return this->_data[this->_data.size()-1];
    }
    else{
        qDebug()<<"ERROR!!!Request for last member of empty LineList";
        return NULL;
    }
}

void LineList::prepend(Line* l){
    this->_data.prepend(l);
}

void LineList::clear(){
    qDebug()<<"Clearing linelist...";
    this->_data.resize(0);
    qDebug()<<"linelist cleared...";
}
Line* LineList::get(int i){
    if(i>=0 && i<this->_data.size()){
        return this->_data[i];
    }
    else{
        qDebug()<<"LineList::Range Request Error!! Request for "<<i<<" out of "<<this->_data.size();
        return NULL;
    }
}
void LineList::set(int i,Line* l){
    this->_data[i]=l;
}
int LineList::size(){
    return this->_data.size();
}

LineList* LineList::FlattenLexicalScopes(LineList* lines){
    //this methodology is borrowed from the flatten control flows method
    //but at this stage everything can be flattened...
    int i=0;
    bool modified=false;
    bool cleanSweep=false;
    while(cleanSweep==false){
        LineList* tempLines=new LineList();
        i=0;
        modified=false;
        while(i<lines->size()){
            if(lines->get(i)->childs->size()>0){
                int e=0;
                tempLines->add(lines->get(i));
                while(e<lines->get(i)->childs->size()){
                    tempLines->add(lines->get(i)->childs->get(e));
                    e++;
                }
                if(e!=0){
                    tempLines->add(new Line(lines->get(i)->getLineNumber(),lines->get(i)->getIndent(),"END"));
                    modified=true;
                }
                lines->get(i)->childs = new LineList();
            }
            else{
                tempLines->add(lines->get(i));
            }
            i++;
        }
        if(modified==false){
            cleanSweep=true;
        }
        lines=tempLines;
    }
    return lines;
}

QString LineList::toString(int indent){
    int i=0;
    QString res="LINELIST["+QString::number(this->size())+"][:\n";
    while(i<this->_data.size()){
        res+=this->get(i)->toString(indent+1)+"\n";
        i++;
    }
    res+="];\n";
    return res;
}

LineList* LineList::flattenControlFlows(LineList *lines){
    int i=0;
    bool modified=false;
    bool cleanSweep=false;
    while(cleanSweep==false){
        LineList* tempLines=new LineList();
        i=0;
        modified=false;
        while(i<lines->size()){
            bool x = lines->get(i)->isConditionalStatement();
            if(x==true){
                int e=0;
                tempLines->add(lines->get(i));
                while(e<lines->get(i)->childs->size()){
                    tempLines->add(lines->get(i)->childs->get(e));
                    e++;
                }
                if(e!=0){
                    tempLines->add(new Line(lines->get(i)->getLineNumber(),lines->get(i)->getIndent(),"END"));
                    modified=true;
                }
                lines->get(i)->childs = new LineList();
            }
            else{
                tempLines->add(lines->get(i));
            }
            i++;
        }
        if(modified==false){
            cleanSweep=true;
        }
        lines=tempLines;
    }
    i=0;
    while(i<lines->size()){
        lines->get(i)->flattenControlFlows();
        i++;
    }
    return lines;
}

int LineList::CurrentLexicalID = 0;

int LineList::getNextLexicalID(){
    LineList::CurrentLexicalID++;
    return (LineList::CurrentLexicalID-1);
}

QVector<int> LineList::LexicalParentsList = {};

void LineList::setLexicalParent(int childID,int parentID){
    if(childID!=parentID){
        if(childID>=LineList::LexicalParentsList.size()){
            LineList::LexicalParentsList.resize(childID+1);
        }
        LineList::LexicalParentsList[childID]=parentID;
    }
}

LineList* LineList::SetLexicalMarkers(LineList* lines,int parentID){
    int lexicalID = LineList::getNextLexicalID();
    LineList::setLexicalParent(lexicalID,parentID);
 //   qDebug()<<"Fetched lexical ID";
    LineList* res = new LineList();
    res->add(new Line(
        lines->get(0)->getLineNumber(),
        lines->get(0)->getIndent(),
        new TokenList(),
        Line::LexicalMarker,
        lexicalID
    ));
//    qDebug()<<"Set lexical scope id marker";
    int i=0;
    while(i<lines->size()){
        res->add(lines->get(i));
        i++;
    }
    res->add(new Line(
         lines->get(0)->getLineNumber(),
         lines->get(0)->getIndent(),
         new TokenList(),
         Line::LexicalPassMarker
    ));

  //  qDebug()<<"Set lexical pass marker";
    i=0;
    while(i<res->size()){
        res->get(i)->setLexicalMarkers(lexicalID);
        i++;
    }
    return res;
}

LineList* LineList::SetLexicalMarkers(LineList* lines){
    return LineList::SetLexicalMarkers(lines,0);
}

LineList* LineList::RPNPadding(LineList *lines){
    int i=0;

    LineList* temp = new LineList();

    while(i<lines->size()){
        if(lines->get(i)->isExpression()==true){
            temp->addFromList(lines->get(i)->spaceOutRPNS());
        }
        else{
            temp->add(lines->get(i));
        }
        i++;
    }

    i=0;
    while(i<temp->size()){
        if(temp->get(i)->childs->size()>0){
            temp->get(i)->childs = LineList::RPNPadding(temp->get(i)->childs);
        }
        i++;
    }

    return temp;
}

LineList* LineList::hoistVariables(LineList* lines){
    int i=0;
    while(i<lines->size()){
        if(lines->get(i)->size()>2){
           lines->get(i)->setAsExpression();
           if(lines->get(i)->isVarDec()){

             //   qDebug()<<"Line "<<lines->get(i)->getLineNumber()<<" is a var dec for hoisting...";

                TokenList* vect = lines->get(i)->getVarDec();
                lines->get(i)->stripVarDec();
                lines->prepend(new Line(
                    lines->get(i)->getLineNumber(),
                    lines->get(i)->getIndent(),
                    vect
                ));
                i++;
           }
        }
        i++;
    }
    i=0;
    while(i<lines->size()){
        lines->get(i)->hoistVariables();
        i++;
    }
    return lines;
}
