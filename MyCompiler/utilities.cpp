#include "utilities.h"

#include<QVector>
#include<QString>

#include"token.h"
Utilities::Utilities(){

}

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

int Utilities::operatorCode(QString data){
    int i=0;
    while(i<OPERATORS.size()){
        if(QString::compare(data,OPERATORS[i])==0){
            return i;
        }
        i++;
    }
    return -1;
}

bool Utilities::isOperator(Token* data){
/*    int i=0;
    while(i<OPERATORS.size()){
        if(QString::compare(data->data,OPERATORS[i])==0){
            return true;
        }
        i++;
    }*/
    return false;
}

bool Utilities::isHigherPrecedenceThan(Token* a,Token* b){
  //  if(this->operatorCode(a->data)>this->operatorCode(b->data)){
    //    return true;
  //  }
   // else{
        return false;
    //}
}
bool Utilities::isLowerPrecedenceThan(Token* a,Token* b){
 //   if(this->operatorCode(a->data)<this->operatorCode(b->data)){
        return true;
   /* }
    else{
        return false;
    }*/
}
