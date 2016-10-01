#ifndef LINE_H
#define LINE_H

#include <QString>

class Token;
class LineList;
class TokenList;

class Line{
    //private static variables...
    private:static constexpr int ToRPNBuffer1=0;
    private:static constexpr int ToRPNBuffer2=1;
    private:static constexpr int ArgumentMarker=2;
    private:static QVector<Line*> LinesRegister;
    //public static variables...
    public:static constexpr int LexicalMarker=3;
    public:static constexpr int LexicalPassMarker = 4;
    //public static methods...
    public:static LineList* RPNPadding(LineList* lines);
    public:static QVector<Line*> FlattenLexicalScopes(QVector<Line*> lines,QVector<int>* lexTree);
    public:static TokenList* convert2RPN(TokenList* data);
    public:static TokenList* insertNOARGS(TokenList* data);
    public:static TokenList* insertFunctionMarkers(TokenList* data);
    public:static void registerLine(Line* line);
    public:static void clearLinesRegister();
    public:static TokenList* insertDUDRightHands(TokenList* tokenList);
    //constructors...
    public:Line(int lineNumber,int indent,TokenList* data);
    public:Line(int lineNumber,int indent,TokenList* data,int TYPE_ID);
    public:Line(int lineNumber,int indent,Token* data,int TYPE_ID);
    public:Line(int lineNumber,int indent,TokenList* data,int TYPE_ID,int LexicalID);
    public:Line(int lineNumber,int indent,QString line);
    //private instance variables...
    private:int lineNumber;
    private:int indent;
    private:bool _isFunctionHeader;
    private:bool _isExpression;
    private:bool _isReturnStatement;
    private:bool _isConditionalStatement;
    private:int _lexicalID;
    private:TokenList* data;
    private:bool _toRPNBuffer1;
    private:bool _toRPNBuffer2;
    //public instance variabless...
    public:LineList* childs;
    public:bool _isLexicalPassMarker;
    public:bool _isLexicalMarker;
    //public methods...
    public:void setLexicalMarkers(int parentID);
    public:bool isLexicalMarker();
public:bool isLexicalPassMarker();
    public:bool containsRPNMarker();
    public:int getRPNMarkerCount();
    public:int getFirstRPNMarkerID();
    public:bool isClassDef();
    public:bool isArgumentMarker();
    public:bool isReturnStatement();
    public:bool isLiteral();
    public:bool isFunctionHeader();
    public:QByteArray compile();
    public:void flattenRPNS();
    public:void mergeConditionals();
    public:void takeChilds(LineList* data);
    public:void pullDownArguments();
    public:bool isExpression();
    public:void setAsExpression();
    public:void adjustToRPN();
    public:void mergeOperators();
    public:bool isUnderScore();
    public:void mergeUnderScores();
    public:bool isVarDec();
    public:void pullDownChildren();
    public:void hoistVariables();
    public:void stripVarDec();
    public:void flattenControlFlows();
    public:bool isConditionalStatement();
    public:QString toString(int indent);
    public:int getIndentation();
    public:bool isArgumentsMarker();
    public:void mergeFloats();
    public:Token* get(int i);
    public:TokenList* getVarDec();
    public:LineList* spaceOutRPNS();
    public:bool isLexicalIdentifier();
//    public:int indexOfColon(int colonCount);
//    public:int indexOfSemiColon(int semiColonCount);
    public:Line* subLine(int startIndex,int endIndex);
    public:int getIndent();
    public:int getLineNumber();
    public:int size();
};

#endif // LINE_H
