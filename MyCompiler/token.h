#ifndef TOKEN_H
#define TOKEN_H

#include<QString>

class Token{
//private static variables...
private:static QVector<Token*> TokensRegister;
//public static variables...
public:static constexpr int RPNBuffer1=0;
public:static constexpr int RPNBuffer2=1;
public:static constexpr int DudRightHand=2;
//public static methods...
public:static void registerToken(Token* token);
public:static void clearTokensRegister();
//private instance variables...
private:QString _data;
private:int _type;
private:bool _isAlphaNumeric;
private:bool _isUnderScore;
private:bool _isSecondaryOperator;
private:bool _isLeadingOperator;
private:bool _isConditionalElse;
private:bool _isConditionalIf;
private:bool _isConditionalStatement;
private:bool _isDotOperator;
private:bool _isOperator;
private:bool _isColon;
private:bool _isSemiColon;
private:bool _isVarDec;
private:bool _isLeftBracket;
private:bool _isRightBracket;
private:bool _isComma;
private:bool _isLambda;
private:bool _isReturnStatement;
private:bool _isRPNBufferToken;
private:bool _isRPNBuffer1;
private:bool _isRPNBuffer2;
private:bool _isUnaryOperator;
private:bool _isDudRightHand;
//private instance methods...
private:void eval();
//public constructors...
public:Token(QString data);
public:Token(int id);
//public instance methods...
public:bool isRPNBufferToken();
public:bool isRPNBufferToken1();
public:bool isConditionalStatement();
public:bool isClassDef();
public:bool isLiteral();
public:bool isRightBracket();
public:bool isLeftBracket();
public:bool isComma();
public:bool isArgumentMarker();
public:bool isSemiColon();
public:bool isAlphaNumeric();
public:void concatenate(Token* other);
public:bool isUnderScore();
public:bool isVarDec();
public:bool isColon();
public:int getIndentation();
public:bool isBackSlash();
public:bool equals(Token *token);
public:bool equals(char c);
public:QStringList split(QRegExp s);
public:bool isOperator();
public:bool isLambda();
public:bool isConditionalIf();
public:bool isConditionalElse();
public:bool isReturnStatement();
public:bool isLeadingOperator();
public:bool isSecondaryOperator();
public:bool isDotOperator();
public:bool isUnaryOperator();
public:bool isHigherPrecedenceThan(Token* other);
public:QString getString();
public:QString toString(int indent);
};

#endif // TOKEN_H
