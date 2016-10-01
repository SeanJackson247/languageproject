#include "compiler.h"
#include "utilities.h"

#include <QDebug>
#include <QTextEncoder>

#include <QFile>
#include <QTextStream>
#include <QIODevice>

#include <QElapsedTimer>

#include "token.h"
#include "tokenlist.h"
#include "line.h"
#include "linelist.h"

Compiler::Compiler(){

}

int getIndentation(QString str){
    int i=0;
    int tabs=0;
    int spaces=0;
    bool nonWS=false;
    while(nonWS==false && i<str.length()){
        if(str[i]!='\t' && str[i]!=' '){
            nonWS=true;
        }
        else{
            if(str[i]=='\t'){
                tabs++;
            }
            else{
                spaces++;
            }
        }
        i++;
    }
    if(i==str.length()-1){
        return 0;
    }
    else{
        int res = spaces+(4*tabs);
        if(res==0){return 0;}
        else if(res%4!=0){
            //qDebug()<<"ERROR!! Incorrect Indentation!";
            return 0;
        }
        else{res=res/4;}
        return res;
    }
}

QVector<QString> VARDEC_STATEMENTS={
    "LET",
    "FIX"
};
int LET=0;
int FIX=1;

int isVarDec(QString data){
    int i=0;
    while(i<VARDEC_STATEMENTS.size()){
        if(QString::compare(data,VARDEC_STATEMENTS[i],Qt::CaseInsensitive)==0){
            return i;
        }
        i++;
    }
    return -1;
}

void Compiler::doWork(QString fileName,QVector<QString> data){
    QElapsedTimer myTimer;
    myTimer.start();
    QString finalString="";
    qDebug() << "Compiling!...";//<<data;
    LineList* lines = new LineList();
    LineList* childs = new LineList();
    int i=0;
    int indentation = 0;

    while(i<data.size()){
        int currentIndentation=getIndentation(data[i]);
        if(indentation==currentIndentation){
            if(childs->size()>0){
                if(lines->size()>0){
                    lines->last()->takeChilds(childs);
                }
                else{
                }
                childs->clear();
            }
            lines->add(new Line(i+1,currentIndentation,data[i]));
        }
        else{
            Line* l = new Line(i+1,currentIndentation,data[i]);
            childs->add(l);
        }
        i++;
    }
    if(childs->size()>0){
        if(lines->size()>0){
            lines->last()->takeChilds(childs);
        }
        else{
            qDebug()<<"Error! First line cannot be indented.";
        }
    }

    finalString="\n\nInitial Interpretation:\n\n";

    i=0;
    while(i<lines->size()){
        finalString+=lines->get(i)->toString(0);
        i++;
    }
    finalString+="\nAfter Merging Underscores...\n";

    //merge underscores...

    i=0;
    while(i<lines->size()){
        lines->get(i)->mergeUnderScores();
        i++;
    }

    i=0;
    while(i<lines->size()){
        finalString+=lines->get(i)->toString(0);
        i++;
    }

    //merge operators...

    finalString+="\n\nAfter Merging Operators...\n\n";
    i=0;
    while(i<lines->size()){
        lines->get(i)->mergeOperators();
        i++;
    }

    i=0;
    while(i<lines->size()){
        finalString+=lines->get(i)->toString(0);
        i++;
    }

    //merge conditionals ... else if becomes elseif

    finalString+="\n\nAfter Merging Conditionals...\n\n";

    i=0;
    while(i<lines->size()){
        lines->get(i)->mergeConditionals();
        i++;
    }

    i=0;
    while(i<lines->size()){
        finalString+=lines->get(i)->toString(0);
        i++;
    }

    //merge floats ... '0' '.' '1' becomes '0.1'

    finalString+="\n\nAfter merging floats...\n\n";

    i=0;
    while(i<lines->size()){
        lines->get(i)->mergeFloats();
        i++;
    }

    i=0;
    while(i<lines->size()){
        finalString+=lines->get(i)->toString(0);
        i++;
    }


    //pulldown children...
    finalString+="\n\nPulling down children...\n\n";

    i=0;
    while(i<lines->size()){
        lines->get(i)->pullDownChildren();
        i++;
    }

    i=0;
    while(i<lines->size()){
        finalString+=lines->get(i)->toString(0);
        i++;
    }

    finalString+="\n\nAfter Flattening Control Flows...\n\n";

    //flatten control flows...

    lines = LineList::flattenControlFlows(lines);

    i=0;
    while(i<lines->size()){
        finalString+=lines->get(i)->toString(0);
        i++;
    }

    finalString+="\n\nAfter Hoisting...\n\n";

    lines = LineList::hoistVariables(lines);

    i=0;
    while(i<lines->size()){
        finalString+=lines->get(i)->toString(0);
        i++;
    }

    finalString+="\n\nAfter pulling down...\n\n";


    i=0;
    while(i<lines->size()){
        lines->get(i)->pullDownArguments();
        i++;
    }

    i=0;
    while(i<lines->size()){
        finalString+=lines->get(i)->toString(0);
        i++;
    }

    finalString+="\n\nAfter adjusting RPN...\n\n";

    i=0;
    while(i<lines->size()){
        lines->get(i)->adjustToRPN();
        i++;
    }

    i=0;
    while(i<lines->size()){
        finalString+=lines->get(i)->toString(0);
        i++;
    }

  //  qDebug()<<"LEXTREE:"<<lexTree;

    //flatten RPN
    finalString+="\n\nAfter flattening RPN...\n\n";

    lines = LineList::RPNPadding(lines);

    i=0;
    while(i<lines->size()){
        finalString+=lines->get(i)->toString(0);
        i++;
    }
/*
    finalString+="\n\nAfter setting lexical scope markers...\n\n";
    lines = LineList::SetLexicalMarkers(lines);

    i=0;
    while(i<lines->size()){
        finalString+=lines->get(i)->toString(0);
        i++;
    }

    finalString+="Scope Tree:";

    i=0;
    while(i<LineList::LexicalParentsList.size()){
        finalString+=QString::number(LineList::LexicalParentsList[i])+"\n";
        i++;
    }

  //  QVector<int> lexicalParents=LineList::GetLexicalParents(lines);
/*
    finalString+="\n\nFlattened ScopeTree:\n\n";

    i=0;
    while(i<lexicalParents.size()){
        finalString+=QString::number(lexicalParents[i])+"\n";
        i++;
    }
*//*
    finalString+="\n\nAfter flattening lexical scopes...\n\n";

    lines = LineList::FlattenLexicalScopes(lines);

    i=0;
    while(i<lines->size()){
        finalString+=lines->get(i)->toString(0);
        i++;
    }*/

    //clean up
    Token::clearTokensRegister();
    TokenList::clearTokenListsRegister();

    Line::clearLinesRegister();
    LineList::clearLineListsRegister();

    int ms = myTimer.elapsed();

    float secs = float(ms)/1000.00;

    qDebug() << "Compilation finished, took "<<ms<<" milliseconds.("<<secs<<") seconds.";
    emit debugScript(fileName,finalString);
//    emit workCompleted(fileName,bytes);
}
