#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>
#include "token.h"

class Utilities{
public: Utilities();
public: int operatorCode(QString data);
public: bool isOperator(Token *data);
public: bool isHigherPrecedenceThan(Token *a,Token *b);
public: bool isLowerPrecedenceThan(Token *a,Token *b);
};

#endif // UTILITIES_H
