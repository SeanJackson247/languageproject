#include "utility.h"

QVector<QString> OPERATORS={
    "=",
    "$",
    "(",
    ",",
    "==",
    "!=",
    "+",
    "-",
    "*",
    "/",
    "%",
    "!",
    "++",
    "--",
    "+=",
    "-=",
    "/=",
    "*=",
    "."
};

int operatorCode(QString data){
    int i=0;
    while(i<OPERATORS.size()){
        if(QString::compare(data,OPERATORS[i])==0){
            return i;
        }
        i++;
    }
    return -1;
}

bool isOperator(QString data){
    int i=0;
    while(i<OPERATORS.size()){
        if(QString::compare(data,OPERATORS[i])==0){
            return true;
        }
        i++;
    }
    return false;
}
bool isHigherPrecedenceThan(QString a,QString b){
    if(operatorCode(a)>operatorCode(b)){
        return true;
    }
    else{
        return false;
    }
}
bool isLowerPrecedenceThan(QString a,QString b){
    if(operatorCode(a)<operatorCode(b)){
        return true;
    }
    else{
        return false;
    }
}
