#include "token.h"
#include "utilities.h"

#include <QStringList>

#include <QVector>
#include <QDebug>

QVector<Token*> Token::TokensRegister={};

void Token::clearTokensRegister(){
    int i=0;
    while(i<Token::TokensRegister.size()){
        delete TokensRegister[i];
        i++;
    }
    Token::TokensRegister.resize(0);
}

QVector<QString> CONDITIONAL_STATEMENTS={
    "IF",
    "ELSE",
    "ELSEIF",
    "DO",
    "WHILE",
    "FOR"
};
int CONDITIONAL_IF=0;
int CONDITIONAL_ELSE=1;
int CONDITIONAL_ELSEIF=2;
int CONDITIONAL_DO=3;
int CONDITIONAL_WHILE=4;
int CONDITIONAL_FOR=5;

int _isConditionalStatement(QString data){
    int i=0;
    while(i<CONDITIONAL_STATEMENTS.size()){
        if(QString::compare(data,CONDITIONAL_STATEMENTS[i],Qt::CaseInsensitive)==0){
            return i;
        }
        i++;
    }
    return -1;
}

bool Token::isConditionalStatement(){
    return this->_isConditionalStatement;
}

bool Token::isReturnStatement(){
    this->_isReturnStatement;
}

bool Token::isClassDef(){
    if(QString::compare(this->_data,"classdef")==0){
        return true;
    }
    return false;
}

bool Token::isLiteral(){
    if(QString::compare(this->_data,"literal")==0){
        return true;
    }
    return false;
}

bool Token::isRightBracket(){
    return this->_isRightBracket;
}

bool Token::isLeftBracket(){
    return this->_isLeftBracket;
}
bool Token::isComma(){
    return this->_isComma;
}

bool Token::isUnderScore(){
    return this->_isUnderScore;
}
bool Token::isUnaryOperator(){
    return this->_isUnaryOperator;
}
bool Token::isArgumentMarker(){
    if(QString::compare(this->_data,"earg")==0){
        return true;
    }
    return false;
}

bool Token::isAlphaNumeric(){
        return this->_isAlphaNumeric;
    }

bool Token::isVarDec(){
        return this->_isVarDec;
    }

void Token::concatenate(Token* other){
    this->_data+=other->getString();
    this->eval();
}

bool Token::isSemiColon(){
    return this->_isSemiColon;
}

bool Token::isColon(){
        return this->_isColon;
    }

int Token::getIndentation(){
    return 0;
}

bool Token::isBackSlash(){
        return false;
    }

bool Token::equals(char c){
        return false;
    }

bool Token::equals(Token* token){
        return false;
    }


bool Token::isOperator(){
        return this->_isOperator;
    }

bool Token::isLambda(){
        return this->_isLambda;
    }

bool Token::isConditionalElse(){
        return this->_isConditionalElse;
    }
bool Token::isConditionalIf(){
        return this->_isConditionalIf;
    }

QStringList Token::split(QRegExp s){
    QStringList dud = {};
    return dud;
}


QString Token::toString(int indent){
    if(this->_isRPNBufferToken==false){
        return "TOKEN["+this->_data+
                "][an="+QString::number(this->isAlphaNumeric())+
                ",_="+QString::number(this->isUnderScore())+
                ",isColon:="+QString::number(this->isColon())+
                ",isSemiColon:="+QString::number(this->isSemiColon())+
                ",cs:="+QString::number(this->isConditionalStatement())+
                ",lambda:="+QString::number(this->isLambda())+
                "];";
    }
    else{
        return "RPN TOKEN:1:="+QString::number(this->_isRPNBuffer1)+",2:"+QString::number(this->_isRPNBuffer2)+";";
    }
}
QString Token::getString(){
    return this->_data;
}



QVector<QString> OPERATORS={
    "=",
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
    "%=",
    "is",
    "not",
    "and",
    "or",
    "in",
    "&&",
    "||",
    "$",
    "."
};

QVector<QString> UNARY_OPERATORS={
    "!",
    "++",
    "--"
};

bool Token::isRPNBufferToken(){
    return this->_isRPNBufferToken;
}

bool Token::isRPNBufferToken1(){
    return this->_isRPNBuffer1;
}

QVector<QString> SECONDARY_OPERATORS={
    "+",
    "-",
    "=",
    "+"
};

bool __isSecondaryOperator(QString data){
    int i=0;
    while(i<SECONDARY_OPERATORS.size()){
        if(QString::compare(data,SECONDARY_OPERATORS[i])==0){
            return true;
        }
        i++;
    }
    return false;
}


QVector<QString> LEADING_OPERATORS={
    "+",
    "-",
    "=",
    "!",
    "%",
    "*",
    "/"
};

bool __isLeadingOperator(QString data){
    int i=0;
    while(i<LEADING_OPERATORS.size()){
        if(QString::compare(data,LEADING_OPERATORS[i])==0){
            return true;
        }
        i++;
    }
    return false;
}

void Token::eval(){
    QRegExp reg = QRegExp("[^a-zA-Z\\d\\s]");
    int x = reg.indexIn(this->_data);
    if(x<0){
        this->_isAlphaNumeric=true;
    }
    else{
        this->_isAlphaNumeric=false;
    }
    if(QString::compare(this->_data,"_")==0){
        this->_isUnderScore=true;
    }
    else{
        this->_isUnderScore=false;
    }
    if(QString::compare(this->_data,":")==0){
        this->_isSemiColon=true;
    }
    else{
        this->_isSemiColon=false;
    }
    if(__isSecondaryOperator(this->_data)==true){
        this->_isSecondaryOperator=true;
    }
    else{
        this->_isSecondaryOperator=false;
    }
    if(__isLeadingOperator(this->_data)==true){
        this->_isLeadingOperator=true;
    }
    else{
        this->_isLeadingOperator=false;
    }
    if(QString::compare(this->_data,"if")==0 || QString::compare(this->_data,"IF")==0){
        this->_isConditionalIf=true;
    }
    else{
        this->_isConditionalIf=false;
    }
    if(QString::compare(this->_data,"if")==0 ||
            QString::compare(this->_data,"elseif")==0 ||
            QString::compare(this->_data,"else")==0 ||
            QString::compare(this->_data,"while")==0 ||
            QString::compare(this->_data,"for")==0 ||
            QString::compare(this->_data,"switch")==0){
        this->_isConditionalStatement=true;
    }
    else{
        this->_isConditionalStatement=false;
    }
    if(QString::compare(this->_data,"else")==0 || QString::compare(this->_data,"ELSE")==0){
        this->_isConditionalElse=true;
    }
    else{
        this->_isConditionalElse=false;
    }
    if(QString::compare(this->_data,".")==0){
        this->_isDotOperator=true;
    }
    else{
        this->_isDotOperator=false;
    }
    if(QString::compare(this->_data,":")==0){
        this->_isColon=true;
    }
    else{
        this->_isColon=false;
    }
    if(QString::compare(this->_data,";")==0){
        this->_isSemiColon=true;
    }
    else{
        this->_isSemiColon=false;
    }
    if(
            QString::compare(this->_data,"let")==0 ||
            QString::compare(this->_data,"LET")==0 ||
            QString::compare(this->_data,"fix")==0 ||
            QString::compare(this->_data,"FIX")==0
            ){
        this->_isVarDec=true;
    }
    else{
        this->_isVarDec=false;
    }

    if(QString::compare(this->_data,"(")==0){
        this->_isLeftBracket=true;
    }
    else{
        this->_isLeftBracket=false;
    }

    if(QString::compare(this->_data,")")==0){
        this->_isRightBracket=true;
    }
    else{
        this->_isRightBracket=false;
    }

    if(QString::compare(this->_data,",")==0){
        this->_isComma=true;
    }
    else{
        this->_isComma=false;
    }

    if(QString::compare(this->_data,"lambda")==0){
        this->_isLambda=true;
    }
    else{
        this->_isLambda=false;
    }

    if(QString::compare(this->_data,"return")==0){
        this->_isReturnStatement=true;
    }
    else{
        this->_isReturnStatement=false;
    }

    int i=0;
    bool found=false;
    while(i<OPERATORS.size() && found==false){
        if(QString::compare(this->_data,OPERATORS[i])==0){
           found=true;
        }
        i++;
    }
    this->_isOperator=found;

    i=0;
    found=false;
    while(i<UNARY_OPERATORS.size() && found==false){
        if(QString::compare(this->_data,UNARY_OPERATORS[i])==0){
           found=true;
        }
        i++;
    }
    this->_isUnaryOperator=found;


}

bool Token::isLeadingOperator(){
    return this->_isLeadingOperator;
}
bool Token::isSecondaryOperator(){
    return this->_isSecondaryOperator;
}

Token::Token(QString data){
    Token::registerToken(this);
    this->_data = data;
    this->_isAlphaNumeric=false;
    this->_isUnderScore=false;
    this->_isSecondaryOperator=false;
    this->_isLeadingOperator=false;
    this->_isConditionalElse=false;
    this->_isConditionalIf=false;
    this->_isConditionalStatement=false;
    this->_isDotOperator=false;
    this->_isOperator=false;
    this->_isColon=false;
    this->_isSemiColon=false;
    this->_isVarDec=false;
    this->_isLeftBracket=false;
    this->_isRightBracket=false;
    this->_isComma=false;
    this->_isLambda=false;
    this->_isReturnStatement=false;
    this->_isRPNBufferToken=false;
    this->_isRPNBuffer1=false;
    this->_isRPNBuffer2=false;
    this->_isUnaryOperator=false;
    this->_isDudRightHand=false;


    this->eval();
}
Token::Token(int id){
    Token::registerToken(this);
    this->eval();
    if(id==Token::RPNBuffer1){
        this->_isRPNBufferToken=true;
        this->_isRPNBuffer1=true;
        this->_isRPNBuffer2=false;
        this->_isOperator=false;
    }
    else if(id==Token::RPNBuffer2){
        this->_isRPNBufferToken=true;
        this->_isRPNBuffer1=false;
        this->_isRPNBuffer2=true;
        this->_isOperator=false;
    }
    else if(id==Token::DudRightHand){
        this->_isRPNBufferToken=false;
        this->_isRPNBuffer1=false;
        this->_isRPNBuffer2=false;
        this->_isOperator=false;
        this->_isDudRightHand=true;
    }
    else{
        this->_isRPNBufferToken=false;
        this->_isRPNBuffer1=false;
        this->_isRPNBuffer2=false;
    }
}

bool Token::isDotOperator(){
    return this->_isDotOperator;
}

void Token::registerToken(Token* token){
    Token::TokensRegister.resize(Token::TokensRegister.size()+1);
    Token::TokensRegister[Token::TokensRegister.size()-1]=token;
}

bool Token::isHigherPrecedenceThan(Token* other){
    int m=0;
    int o=0;
    int i=0;
    bool mdone=false;
    while(i<OPERATORS.size() && mdone==false){
        if(QString::compare(this->_data,OPERATORS[i])==0){
            mdone=true;
            m=i;
        }
        i++;
    }
    i=0;
    while(i<OPERATORS.size()){
        if(QString::compare(other->getString(),OPERATORS[i])==0){
            o=i;
            if(m>o){
                return true;
            }
            else{
                return false;
            }
        }
        i++;
    }
    qDebug()<<"ERROR!! - Control Flow should never reach here.";
    return false;
}
