#ifndef TOKENLIST_H
#define TOKENLIST_H

#include <QVector>

class Token;

class TokenList{
public: static QVector<TokenList*> TokenListsRegister;
public: static void clearTokenListsRegister();
public: static void registerTokenList(TokenList* tokenList);
//private instance variables...
private: QVector<Token*> _data;
//constructors...
public: TokenList();
//public methods...
public: void add(Token* t);
public: void addFromList(TokenList* tl);
public: Token* first();
public: Token* last();
public: void clear();
public: Token* get(int i);
public: void set(int i,Token* t);
public: void shrink(int i);
public: int size();
public: QString toString(int indent);
};

#endif // TOKENLIST_H
