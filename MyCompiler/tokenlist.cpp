#include "tokenlist.h"
#include "token.h"

#include <QDebug>

TokenList::TokenList(){
    TokenList::registerTokenList(this);
}

void TokenList::registerTokenList(TokenList* tokenList){
    TokenList::TokenListsRegister.resize(TokenList::TokenListsRegister.size()+1);
    TokenList::TokenListsRegister[TokenList::TokenListsRegister.size()-1]=tokenList;
}

QVector<TokenList*> TokenList::TokenListsRegister={};

void TokenList::clearTokenListsRegister(){
    int i=0;
    while(i<TokenList::TokenListsRegister.size()){
        delete TokenList::TokenListsRegister[i];
        i++;
    }
    TokenList::TokenListsRegister.resize(0);
}

void TokenList::add(Token* t){
    this->_data.resize(this->_data.size()+1);
    this->_data[this->_data.size()-1]=t;
}
void TokenList::addFromList(TokenList* tl){
    int i=0;
    while(i<tl->size()){
        this->add(tl->get(i));
        i++;
    }
}

void TokenList::shrink(int i){
    this->_data.resize(this->_data.size()-i);
}

Token* TokenList::first(){
    this->_data[0];
}
Token* TokenList::last(){
    return this->_data[this->_data.size()-1];
}
void TokenList::clear(){
    this->_data.resize(0);
}
Token* TokenList::get(int i){
    if(i>=0 && i<this->_data.size()){
        return this->_data[i];
    }
    else{
        qDebug()<<"TokenList::Range Request Error!! Request for "<<i<<" out of "<<this->_data.size();
        return NULL;
    }
}
void TokenList::set(int i,Token* t){
    this->_data[i]=t;
}
int TokenList::size(){
    return this->_data.size();
}

QString TokenList::toString(int indent){
    int i=0;
    QString res="TOKENLIST[:\n";
    while(i<this->_data.size()){
        res+=this->get(i)->toString(indent+1)+"\n";
        i++;
    }
    res+="];\n";
    return res;
}
