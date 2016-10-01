#include "line.h"
#include<QDebug>
#include "utilities.h"
#include "linelist.h"
#include "tokenlist.h"
#include "token.h"

Token* Line::get(int i){
    return this->data->get(i);
}
TokenList* Line::insertNOARGS(TokenList* data){
    int i=0;
    TokenList* temp = new TokenList();
    while(i<data->size()){
        temp->add(data->get(i));
        if(QString::compare(data->get(i)->getString(),"$")==0 &&
           QString::compare(data->get(i+1)->getString(),"(")==0 &&
           QString::compare(data->get(i+2)->getString(),")")==0){
            temp->add(new Token("NO_ARGS"));
            i++;
            i++;
        }
        i++;
    }
    return temp;
}

TokenList* Line::insertFunctionMarkers(TokenList* data){
    int i=0;
    TokenList* temp = new TokenList();
    while(i<data->size()){
        if(i>0){
            if(data->get(i)->isLeftBracket()==true && data->get(i-1)->isOperator()==false && data->get(i-1)->isLambda()==false){
                temp->add(new Token("$"));
            }
        }
        temp->add(data->get(i));
        i++;
    }
    return temp;
}

QVector<Line*> Line::LinesRegister={};

void Line::clearLinesRegister(){
    int i=0;
    while(i<Line::LinesRegister.size()){
        delete LinesRegister[i];
        i++;
    }
    Line::LinesRegister.resize(0);
}

int Line::size(){
    return this->data->size();
}

bool Line::isClassDef(){
        int i=0;
        while(i<this->data->size()){
            if(this->data->get(i)->isClassDef()){
                return true;
            }
            i++;
        }
        return false;
    }
bool Line::isArgumentMarker(){
        return false;
    }
bool Line::isReturnStatement(){return false;}
bool Line::isLiteral(){
        int i=0;
        while(i<this->data->size()){
            if(this->data->get(i)->isClassDef()){
                return true;
            }
            i++;
        }
        return false;
    }

bool Line::isFunctionHeader(){
        return this->_isFunctionHeader;
    }
QByteArray Line::compile(){
        QByteArray qb;
        return qb;
    }
/*
void Line::addHeapData(QByteArray heap){
        //get all the strings

    }
    */
void Line::flattenRPNS(){}
void Line::mergeConditionals(){
        if(this->data->size()>1){
            if(this->data->get(0)->isConditionalElse()==true){
                if(this->data->get(1)->isConditionalIf()==true){
                    int i=2;
                    TokenList* res= new TokenList();
                    res->add(new Token("elseif"));
                    while(i<this->data->size()){
                        res->add(this->data->get(i));
                        i++;
                    }
                    this->data = res;
                }
            }
        }
        int i=0;
        while(i<this->childs->size()){
            this->childs->get(i)->mergeConditionals();
            i++;
        }
    }
void Line::takeChilds(LineList *data){
        int i=0;
        LineList* subChilds = new LineList();
        while(i<data->size()){
            int childIndentation = data->get(i)->getIndentation();
            if(childIndentation==(this->indent+1)){
                if(subChilds->size()>0){
                    this->childs->last()->takeChilds(subChilds);
                    subChilds = new LineList();
                }
                this->childs->add(data->get(i));
            }
            else{
                subChilds->add(data->get(i));
            }
            i++;
        }
        if(subChilds->size()>0 && this->childs->size()>0){
            this->childs->last()->takeChilds(subChilds);
        }
    }

void Line::pullDownArguments(){
        int i=0;
        if(this->isFunctionHeader()==true){
            TokenList* newData = new TokenList();
            TokenList* args = new TokenList();
            i=0;
            bool inArgs=false;
            while(i<this->data->size()){
                if(inArgs==false){
                    if(this->data->get(i)->isLeftBracket()){
                        inArgs=true;
                    }
                    else{
                        newData->add(this->data->get(i));
                    }
                }
                else{
                    if(this->data->get(i)->isRightBracket()){
                        inArgs=false;
                    }
                    else{
                        if(this->data->get(i)->isComma()==false){
                            args->add(this->data->get(i));
                        }
                    }
                }
                i++;
            }
            i=args->size()-1;
            while(i>=0){
                this->childs->prepend(new Line(this->lineNumber,this->indent,args->get(i),Line::ArgumentMarker));
                i--;
            }
            this->data = newData;
        }
        i=0;
        while(i<this->childs->size()){
            this->childs->get(i)->pullDownArguments();
            i++;
        }
    }
bool Line::isExpression(){
       return this->_isExpression;
    }
TokenList* Line::insertDUDRightHands(TokenList* tokenList){
    TokenList* tl = new TokenList();
    int i=0;
    while(i<tokenList->size()){
        tl->add(tokenList->get(i));
        if(tl->last()->isUnaryOperator()==true){
            tl->add(new Token(Token::DudRightHand));
        }
        i++;
    }
    return tl;
}
TokenList* Line::convert2RPN(TokenList* data){
    data = Line::insertDUDRightHands(data);
    data = Line::insertFunctionMarkers(data);
    data = Line::insertNOARGS(data);
    TokenList* operatorStack = new TokenList();
    TokenList* operandStack = new TokenList();
    int i=0;

    while(i<data->size()){
       // foreach token in stream:
    //   qDebug()<<data[i];
       if(data->get(i)->isRightBracket()==true){
         //  qDebug()<<"RB";
           //   if right bracket:
           //          take each item of the top of the operator stack
            //         and put it on the operand stack
            //         until left bracket:
            //         left bracket does not remain anywhere when finished.
      //     while(operatorStack.size()>0 && QString::compare(operatorStack[operatorStack.size()-1],"(")!=0){
           while(operatorStack->size()>0 && operatorStack->last()->isLeftBracket()==false){
               //if(QString::compare(operatorStack[operatorStack.size()-1],"(")!=0 && QString::compare(operatorStack[operatorStack.size()-1],")")!=0){
               if(operatorStack->last()->isLeftBracket()==false && operatorStack->last()->isRightBracket()==false){
                   operandStack->add(operatorStack->last());
               }
               operatorStack->shrink(1);
           }
      //     if(operatorStack.size()>0 && QString::compare(operatorStack[operatorStack.size()-1],"(")==0){
           if(operatorStack->size()>0 && operatorStack->last()->isRightBracket()==false){
       //        qDebug()<<"Final shred!";
           //    operatorStack.resize(operatorStack.size()-1);
               operatorStack->shrink(1);
           }
       }
       else if(data->get(i)->isOperator()==true){
        //   qDebug()<<data[i]<<"is ooperator";
           //   if is operator:
      //      qDebug()<<"An operator!";
            //     if operator stack is empty:
              //       put it on the stack
           if(operatorStack->size()==0){
          //      qDebug()<<"And operator stack is empty, so putting it on...";
                operatorStack->add(data->get(i));
            }
//            else if(QString::compare(data[i],"(")==0){
           else if(data->get(i)->isLeftBracket()==true){
               operatorStack->add(data->get(i));
            }
            else{
//                if(utility->isHigherPrecedenceThan(
               if(operatorStack->last()->isHigherPrecedenceThan(data->get(i))){
                 //   qDebug()<<"Item on the top of the stack is of higher precedence.";
                    bool escape=false;
                    while(escape==false && operatorStack->last()->isHigherPrecedenceThan(data->get(i))){
//                        if(QString::compare(operatorStack[operatorStack.size()-1],"(")!=0 && QString::compare(operatorStack[operatorStack.size()-1],")")!=0){
                        if(operatorStack->last()->isLeftBracket()==false && operatorStack->last()->isRightBracket()==false){
                   //         qDebug()<<"Adding top item:"<<operatorStack[operatorStack.size()-1];
                            operandStack->add(operatorStack->last());
                        }
                        operatorStack->shrink(1);
                        if(operatorStack->size()==0){
                            escape=true;
                        }
                    }
                    operatorStack->add(data->get(i));
                }
                else{
                    operatorStack->add(data->get(i));
                }
            }
        }
        else{
          //  qDebug()<<"Not an operator. Placing on operand stack...";
            operandStack->add(data->get(i));
        }
     //   qDebug()<<"end of iteration operandstack:"<<operandStack;
     //   qDebug()<<"end of iteration operatorstack:"<<operatorStack;
       // qDebug()<<"end";
        i++;
    }
    //      if operator stack is not empty:
    if(operatorStack->size()!=0){
        //        take each item of the top of the operator stack
          //      and put it on the operand stack
        while(operatorStack->size()>0){
//            if(QString::compare(operatorStack[operatorStack.size()-1],"(")!=0 && QString::compare(operatorStack[operatorStack.size()-1],")")!=0){
            if(operatorStack->last()->isRightBracket()==false && operatorStack->last()->isRightBracket()==false){
                operandStack->add(operatorStack->last());
            }
            operatorStack->shrink(1);
        }
    }
    return operandStack;
}
void Line::adjustToRPN(){
        if(this->data->size()>1){
            if(this->isExpression()==true && !this->isConditionalStatement() && !this->isReturnStatement()){
                Token* terminator = this->data->last();
                this->data->shrink(1);
                this->data = Line::convert2RPN(this->data);
                this->data->add(terminator);
                int i=0;
                if(this->isClassDef()==true || this->isLiteral()==true){
                    while(i<this->childs->size()){
                        int e=0;
                        while(e<this->childs->get(i)->childs->size()){
                            this->childs->get(i)->childs->get(e)->adjustToRPN();
                            e++;
                        }
                        i++;
                    }
                }
                else{
                    i=0;
                    while(i<this->childs->size()){
                        this->childs->get(i)->adjustToRPN();
                        i++;
                    }
                }
            }
            else if(this->isConditionalStatement()==true || this->isReturnStatement()==true){
                Token* terminator = this->data->last();
                Token* conditional = this->data->get(0);
                qDebug()<<conditional->toString(0);
                this->data->shrink(1);
                TokenList* temp = new TokenList();
                int i=1;
                while(i<this->data->size()){
                    temp->add(this->data->get(i));
                    i++;
                }
                this->data = new TokenList();
                this->data->add(conditional);
                this->data->addFromList(Line::convert2RPN(temp));
                this->data->add(terminator);
                i=0;
                while(i<this->childs->size()){
                    this->childs->get(i)->adjustToRPN();
                    i++;
                }
            }
        }
    }
void Line::mergeOperators(){
        int i=0;
        TokenList* newData = new TokenList();
        if(this->data->size()>0){
            while(i<this->data->size()-1){
                if(this->data->get(i)->isLeadingOperator()==true && this->data->get(i+1)->isSecondaryOperator()==true){
                    this->data->get(i)->concatenate(this->data->get(i+1));
                    newData->add(this->data->get(i));
                    i++;
                }
                else{
                    newData->add(this->data->get(i));
                }
                i++;
            }
            newData->add(this->data->last());
            this->data = newData;
        }
        i=0;
        while(i<this->childs->size()){
            this->childs->get(i)->mergeOperators();
            i++;
        }
    }
bool Line::isUnderScore(){
        return false;
    }
void Line::mergeUnderScores(){
        int i=0;
        TokenList* newData = new TokenList();
        bool followingUnderScore=false;
        while(i<this->data->size()){
            if(followingUnderScore==false){
                if(this->data->get(i)->isUnderScore()==true){
                 //   qDebug()<<"Underscore encountered!";
                    followingUnderScore=true;
                    if(newData->size()==0){
                        newData->add(this->data->get(i));
                    }
                    else{
                        if(newData->last()->isAlphaNumeric()){
                            //alphanumeric...
                   //         qDebug()<<"Concatenating "<<newData->last()->toString(0)<<" and "<<this->data->get(i)->toString(0);
                            newData->last()->concatenate(this->data->get(i));
                        }
                        else{
                            newData->add(this->data->get(i));
                        }
                    }
                }
                else{
                    newData->add(this->data->get(i));
                }
            }
            else{
                if(this->data->get(i)->isUnderScore()==true){
                    newData->last()->concatenate(this->data->get(i));
                }
                else{
                 //   QRegExp reg = QRegExp("[^a-zA-Z\\d\\s]");
                   // int x = reg.indexIn(this->data[i]);
            //        //qDebug()<<"String:"<<this->data[i];
            //        //qDebug()<<"Matches regex = "<<x;
                    //if(x<0){
                    if(this->data->get(i)->isAlphaNumeric()==true){
                        //is alphanumeric...
                        newData->last()->concatenate(this->data->get(i));
                    }
                    else{
                        newData->add(this->data->get(i));
                        followingUnderScore=false;
                    }
                }
            }
//            qDebug()<<"End of Iteration. New Data:"<<newData->toString(0);
            i++;
        }
        this->data=newData;
        i=0;
        while(i<this->childs->size()){
            this->childs->get(i)->mergeUnderScores();
            i++;
        }
    }
bool Line::isVarDec(){
        if(this->data->size()){
            return this->data->get(0)->isVarDec();
        }
        else{
            return false;
        }
    }
/*int Line::indexOfColon(int colonCount){
    colonCount++;
    int i=0;
    int e=0;
    while(i<this->data->size()){
        if(this->data->get(i)->isColon()==true){
            e++;
        }
        if(e==(colonCount)){
            return i;
        }
        i++;
    }
    return -1;
}*/
Line* Line::subLine(int startIndex,int endIndex){
    TokenList* tokenList = new TokenList();
    int i = startIndex;
//    qDebug()<<"S";
    while(i<endIndex && i<this->data->size()){
      //  qDebug()<<"L";
        tokenList->add(this->data->get(i));
        i++;
    }
  //  qDebug()<<"F";
    Line* res = new Line(this->lineNumber,this->indent,tokenList);
    //qDebug()<<res->toString(0);
    return res;
}
/*int Line::indexOfSemiColon(int semiColonCount){
    semiColonCount++;
    int i=0;
    int e=0;
    while(i<this->data->size()){
        if(this->data->get(i)->isSemiColon()==true){
            e++;
        }
        if(e==(semiColonCount)){
            return i;
        }
        i++;
    }
    return -1;
}*/
void Line::pullDownChildren(){
    int i=0;
    bool inSub=false;
    TokenList* container = new TokenList();
    TokenList* container2 = new TokenList();
    while(i<this->data->size()){
        if(inSub==true){
            container2->add(this->data->get(i));
        }
        else{
            if(this->data->get(i)->isColon()==true){
                container->add(this->data->get(i));
                inSub=true;
            }
            else if(this->data->get(i)->isSemiColon()==true){
                if(i==this->data->size()-1){
                    container->add(this->data->get(i));
                }
                else{
                    qDebug()<<"ERROR!!! - Line Terminal prior to end of string";
                }
            }
            else{
                container->add(this->data->get(i));
            }
        }
        i++;
    }
    if(container2->size()>0){
        LineList* newChilds = new LineList();
        newChilds->add(new Line(this->lineNumber,this->indent+1,container2));
        this->data = container;
        i = 0;
        LineList* finalChilds=new LineList();
        while(i<this->childs->size()){
            finalChilds->add(this->childs->get(i));
            i++;
        }
        newChilds->last()->childs = finalChilds;
        this->childs = newChilds;
    }
 //   qDebug()<<"End of adjustment";
}
void Line::hoistVariables(){
        if(this->isClassDef()==false){
            this->childs = LineList::hoistVariables(this->childs);
        }
        else{
            int i=0;
            while(this->childs->size()){
                this->childs->get(i)->hoistVariables();
                i++;
            }
        }
    }
void Line::stripVarDec(){
        int i=1;
        TokenList* temp = new TokenList();
        while(i<this->data->size()){
            temp->add(this->data->get(i));
            i++;
        }
        this->data = temp;
        if(this->data->last()->isColon()==true){
            bool handled=false;
            if(this->data->size()==2){
                //class dec
                i=0;
                temp=new TokenList();
                while(i<this->data->size()-1){
                    temp->add(this->data->get(i));
                    i++;
                }
                temp->add(new Token("="));
                temp->add(new Token("classdef"));
                temp->add(new Token(":"));
                this->data=temp;
                handled=true;
            }
            else if(this->data->size()==4){
                //could be literal...
            //    if(QString::compare(this->data[1],"{")==0 && QString::compare(this->data[2],"}")==0){
                if(this->isLiteral()==true){
                    //is literal
                    i=0;
                    temp={};
                    while(i<1){
                        temp->add(this->data->get(i));
                        i++;
                    }
                    temp->add(new Token("="));
                    temp->add(new Token("literal"));
                    temp->add(new Token(":"));
                    this->data=temp;
                    handled=true;
                }
            }
            if(handled==false){
                //is a function...
                this->_isFunctionHeader=true;
                i=0;
                temp=new TokenList();
                while(i<1){
                    temp->add(this->data->get(i));
                    i++;
                }

                temp->add(new Token("="));
                temp->add(new Token("lambda"));
                temp->add(new Token("("));

                i=2;
                while(i<this->data->size()-2){
                    temp->add(this->data->get(i));
                    i++;
                }

                temp->add(new Token(")"));
                temp->add(new Token(":"));
                this->data=temp;
                handled=true;
            }
        }
    }
void Line::flattenControlFlows(){
        this->childs = LineList::flattenControlFlows(this->childs);
    }
void Line::setAsExpression(){
        this->_isExpression=true;
    }

bool Line::isConditionalStatement(){
        if(this->data->size()>0){
           return this->data->get(0)->isConditionalStatement();
        }
        return false;
    }

bool Line::isLexicalMarker(){
    return this->_isLexicalMarker;
}

bool Line::isLexicalPassMarker(){
    return this->_isLexicalPassMarker;
}

QString Line::toString(int indent){
    if(this->isLexicalMarker()==false && this->isLexicalPassMarker()==false){
        int i=0;
        QString tabs="";
        while(i<indent){
            tabs+='\t';
            i++;
        }
        i=0;
        QString res=tabs+"//LineNumber:=";
        res+=QString::number(this->lineNumber);
        res+=", indent:=";
        res+=QString::number(this->indent);
        res+=", isFunctionHeader:=";
        res+=QString::number(this->_isFunctionHeader);
        res+=", isVarDec:=";
        res+=QString::number(this->isVarDec());
        res+=", isExpression:=";
        res+=QString::number(this->isExpression());
        res+=", isConditionalStatement:=";
        res+=QString::number(this->isConditionalStatement());
        res+=", rpnbuff:1:=";
        res+=QString::number(this->_toRPNBuffer1);
        res+=",2:=";
        res+=QString::number(this->_toRPNBuffer2);
        res+="\n";
        while(i<this->data->size()){
            res+=tabs+"[";
            res+=QString::number(i);
            res+="]:";
            res+=this->data->get(i)->toString(indent+1);
            res+="\n";
            i++;
        }
//        res+="\n";
        res+=tabs+"CHILDS[";
        res+=QString::number(this->childs->size());
        res+="]:\n";
        i=0;
        while(i<this->childs->size()){
            res+=this->childs->get(i)->toString(indent+1);
            res+="\n";
            i++;
        }
        res+=tabs+"Endl//";
        return res;
    }
    else if(this->isLexicalMarker()==false){
        return "**Lexical Pass Marker**";
    }
    else{
        return "**Lexical Marker["+QString::number(this->_lexicalID)+"]**";
    }
}

Line::Line(int lineNumber, int indent, TokenList *data){
        this->registerLine(this);
        this->_isLexicalMarker=false;
        this->_isFunctionHeader=false;
        this->lineNumber = lineNumber;
        this->data = data;
        this->indent = indent;
        this->childs = new LineList();
    }
int Line::getIndentation(){
        return this->indent;
    }

Line::Line(int lineNumber,int indent,TokenList* data,int TYPE_ID){
    this->registerLine(this);
    this->_isLexicalMarker=false;
    this->_isFunctionHeader=false;
    this->_isExpression=true;
    this->lineNumber = lineNumber;
    this->indent = indent;
    this->childs = new LineList();
    this->data = data;
    this->_isLexicalPassMarker=false;
    if(TYPE_ID==Line::ToRPNBuffer1){
        this->_toRPNBuffer1=true;
        this->_toRPNBuffer2=false;
    }
    else if(TYPE_ID==Line::ToRPNBuffer2){
        this->_toRPNBuffer1=false;
        this->_toRPNBuffer2=true;
    }
    else if(TYPE_ID==Line::LexicalPassMarker){
        this->_isLexicalPassMarker=true;
    }
}

Line::Line(int lineNumber,int indent,Token* data,int TYPE_ID){
    this->registerLine(this);
    this->_isLexicalMarker=false;
    this->_isFunctionHeader=false;
    this->_isExpression=true;
    this->lineNumber = lineNumber;
    this->indent = indent;
    this->childs = new LineList();
    this->data = new TokenList();
    this->data->add(data);
    this->_isLexicalPassMarker=false;
    if(TYPE_ID==Line::ArgumentMarker){
        this->_toRPNBuffer1=false;
        this->_toRPNBuffer2=false;
        this->_isExpression=false;
    }
}


void Line::setLexicalMarkers(int parentID){
    if(this->childs->size()>0){
        this->childs = LineList::SetLexicalMarkers(this->childs,parentID);
    }
}

Line::Line(int lineNumber,int indent,TokenList* data,int TYPE_ID,int lexicalID){
    this->registerLine(this);
    this->_isFunctionHeader=false;
    this->_isExpression=true;
    this->lineNumber = lineNumber;
    this->indent = indent;
    this->childs = new LineList();
    this->data = data;
    this->_isLexicalMarker=true;
    if(TYPE_ID==Line::ToRPNBuffer1){
        this->_toRPNBuffer1=true;
        this->_toRPNBuffer2=false;
    }
    else if(TYPE_ID==Line::ToRPNBuffer2){
        this->_toRPNBuffer1=false;
        this->_toRPNBuffer2=true;
    }
    this->_lexicalID = lexicalID;
}

Line::Line(int lineNumber, int indent, QString line){
    this->registerLine(this);
        this->_isFunctionHeader=false;
        this->_isExpression=false;
    this->_isReturnStatement;
    this->_isConditionalStatement;
    this->_lexicalID;
    this->_toRPNBuffer1=false;
    this->_toRPNBuffer2=false;
    this->_isLexicalPassMarker=false;
    this->_isLexicalMarker=false;






        this->lineNumber = lineNumber;
        this->indent = indent;
        this->childs = new LineList();
        this->data = new TokenList();
    this->_isLexicalMarker=false;
        TokenList* final = new TokenList();
        int o=0;
        TokenList* byString = new TokenList();
        QString container="";
        char separator=' ';
        QVector<bool> isCode;
        bool inString=false;
        while(o<line.length()){
            if(line[o]=='\'' || line[o]=='"'){
                if(separator==' '){
                    if(inString==true){
                        inString=false;
                    }
                    else{
                        inString=true;
                    }
                }
                else{
                    if(o>=2){
                        if(line[o]==separator && (line[o-1]!='\\' || (line[0-2]=='\\'))){
                            if(inString==true){
                                inString=false;
                            }
                            else{
                                inString=true;
                            }
                        }
                    }
                    else if(o==1){
                        if(line[o]==separator && line[o-1]!='\\'){
                            if(inString==true){
                                inString=false;
                            }
                            else{
                                inString=true;
                            }
                        }
                    }
                    else{//o==0
                        if(line[o]==separator){
                            if(inString==true){
                                inString=false;
                            }
                            else{
                                inString=true;
                            }
                        }
                    }
                }
                if(container.length()>0){
                    byString->add(new Token(container));
                    isCode.append(inString);
                    container="";
                }
            }
            else{
                container+=line[o];
            }
            o++;
        }
        if(container.length()>0){
            byString->add(new Token(container));
            isCode.append(!inString);
            container="";
        }
        o=0;
        while(o<isCode.size()){
            if(isCode[o]==true){
                QStringList list = byString->get(o)->getString().split(QRegExp("\\s"));
                int i=0;
                while(i<list.size()){
                    if(list[i].length()>0){
                        QRegExp reg = QRegExp("[^a-zA-Z\\d\\s]");
                        int x = reg.indexIn(list[i]);
                        if(x>=0){
                            int e=0;
                            container="";
                            while(e<list[i].length()){
                                if(list[i].at(e).isLetter() || list[i].at(e).isDigit()){
                                    std::string stdStr = (list[i].toStdString());
                                    char a = stdStr[e];
                                    container+=a;
                                }
                                else{
                                    if(container.length()>0){
                                        final->add(new Token(container));
                                        container="";
                                    }
                                    final->add(new Token(QString(QChar(list[i].toStdString()[e]))));
                                }
                                e++;
                            }
                            if(container.length()>0){
                                final->add(new Token(container));
                            }
                        }
                        else{
                            final->add(new Token(list[i]));
                        }
                    }
                    i++;
                }
            }
            else{
                final->add(new Token("\""+byString->get(o)->getString()+"\""));
            }
            o++;
        }
        this->data=final;
        this->_isExpression=false;
        this->_isConditionalStatement=false;
        if(this->data->size()>0){
            if(this->data->first()->isReturnStatement()==true){
                this->_isExpression=true;
                this->_isReturnStatement=true;
            }
            else if(this->data->first()->isConditionalStatement()==true){
                this->_isExpression=true;
                this->_isConditionalStatement=true;
            }
        }
    }
void Line::registerLine(Line* line){
    Line::LinesRegister.resize(Line::LinesRegister.size()+1);
    Line::LinesRegister[Line::LinesRegister.size()-1]=line;
}
bool Line::isArgumentsMarker(){
        return false;
    }
void Line::mergeFloats(){
    int i=0;
    TokenList* newData = new TokenList();
    while(i<this->data->size()-2){
        bool ok1;
        this->data->get(i)->getString().toInt(&ok1, 10);
        bool ok2;
        this->data->get(i+2)->getString().toInt(&ok2, 10);
        if(ok1==true && ok2==true && this->data->get(i+1)->isDotOperator()==true){
            newData->add(new Token(this->data->get(i)->getString()+"."+this->data->get(i+2)->getString()));
            i++;
            i++;
        }
        else{
            newData->add(this->data->get(i));
        }
        i++;
    }
    while(i<this->data->size()){
        newData->add(this->data->get(i));
        i++;
    }
    this->data=newData;
    i=0;
    while(i<this->childs->size()){
        this->childs->get(i)->mergeFloats();
        i++;
    }
}

LineList* Line::RPNPadding(LineList *lines){

    int i=0;

    LineList* temp = new LineList();

    return temp;
}
bool Line::containsRPNMarker(){
    int i=0;
    while(i<this->data->size()){
        if(this->data->get(i)->isRPNBufferToken()==true){
            return true;
        }
        i++;
    }
    return false;
}
int Line::getRPNMarkerCount(){
    int i=0;
    int e=0;
    while(i<this->data->size()){
        if(this->data->get(i)->isRPNBufferToken()==true){
            e++;
        }
        i++;
    }
    return e;
}

int Line::getFirstRPNMarkerID(){
    int i=0;
    while(i<this->data->size()){
        if(this->data->get(i)->isRPNBufferToken()==true){
            if(this->data->get(i)->isRPNBufferToken1()==true){
                return 1;
            }
            else{
                return 2;
            }
        }
    }
    return -1;
}
LineList* Line::spaceOutRPNS(){
        LineList* result = new LineList();
        TokenList* container = new TokenList();
        qDebug()<<"Spacing out RPNs...";
        int i=0;
        if(this->isExpression()==true){
            if(this->isConditionalStatement()==false && this->isReturnStatement()==false){
                i=0;
            }
            else{
                i=1;
            }
            while(i<this->data->size()-1){
                qDebug()<<"Here is the error..."<<i;
                container->add(this->data->get(i));
                i++;
            }
        }
        bool finished=false;
        int done=0;
        int marker=1;
        while(finished==false){
            int e=0;
            bool breakOut=false;
            while(e<container->size()-2 && breakOut==false){
                if(
                    container->get(e)->isOperator()==false &&
                    container->get(e+1)->isOperator()==false &&
                    container->get(e+2)->isOperator()==true
                ){
                    TokenList* exp =new TokenList();
                    exp->add(container->get(e));
                    exp->add(container->get(e+1));
                    exp->add(container->get(e+2));
                    if(done==0){
                        //first operation in expression
                        //goes to the first buffer
                        result->add(new Line(
                            this->lineNumber,
                            this->indent,
                            exp,
                            Line::ToRPNBuffer1
                        ));
                        done++;
                        //now replacing operation with rpn reference
                        int x=0;
                        TokenList* container2 = new TokenList();
                        while(x<container->size()){
                            if(x!=e && x!=(e+1) && x!=(e+2)){
                                container2->add(container->get(x));
                            }
                            else if(x==e){
                                //the reference should point to the first rpn buffer
                                container2->add(new Token(Token::RPNBuffer1));
                            }
                            x++;
                        }
                        container=container2;
                        breakOut=true;
                    }
                    else{
                        //need to check previous result for presence of an rpn marker.
                        //if no rpn marker is present, we use the rpn marker we didnt before.
                        //if both rpn markers are present, we use the first rpn marker
                        if(result->last()->containsRPNMarker()==false){
                            if(marker==1){
                                result->add(new Line(
                                    this->lineNumber,
                                    this->indent,
                                    exp,
                                    Line::ToRPNBuffer2
                                ));
                                marker=2;
                            }
                            else{
                                result->add(new Line(
                                    this->lineNumber,
                                    this->indent,
                                    exp,
                                    Line::ToRPNBuffer1
                                ));
                                marker=1;
                            }
                        }
                        //if 1 rpn marker is present, we use the same buffer as the marker
                        else{
                            if(result->last()->getRPNMarkerCount()==1){
                                marker = result->last()->getFirstRPNMarkerID();
                                if(marker==1){
                                    result->add(new Line(
                                        this->lineNumber,
                                        this->indent,
                                        exp,
                                        Line::ToRPNBuffer1
                                    ));
                                }
                                else{
                                    result->add(new Line(
                                        this->lineNumber,
                                        this->indent,
                                        exp,
                                        Line::ToRPNBuffer2
                                    ));
                                }
                            }
                            else{
                                //if both rpn markers are present, we use the first rpn marker
                                result->add(new Line(
                                    this->lineNumber,
                                    this->indent,
                                    exp,
                                    Line::ToRPNBuffer1
                                ));
                                marker=1;
                            }
                        }
                        //now replacing operation with rpn reference
                        int x=0;
                        TokenList* container2 = new TokenList();
                        while(x<container->size()){
                            if(x!=e && x!=(e+1) && x!=(e+2)){
                                container2->add(container->get(x));
                            }
                            else if(x==e){
                                //the reference point to the last used rpn buffer
                                if(marker==1){
                                    container2->add(new Token(Token::RPNBuffer1));
                                }
                                else{
                                    container2->add(new Token(Token::RPNBuffer2));
                                }
                            }
                            x++;
                        }
                        container=container2;
                        breakOut=true;
                    }
                }
                e++;
            }
            if(breakOut==false){
                finished=true;
            }
        }
        if(result->size()>0){
            result->last()->childs = this->childs;
        }
        return result;
    }

bool Line::isLexicalIdentifier(){
        return false;
    }

QVector<Line*> Line::FlattenLexicalScopes(QVector<Line*> lines,QVector<int>* lexTree){
    QVector<Line*> temp;
    return temp;
}

int Line::getIndent(){
    return this->indent;
}
int Line::getLineNumber(){
    return this->lineNumber;
}
TokenList* Line::getVarDec(){
    if(this->data->size()>1){
        TokenList* res= new TokenList();
        res->add(this->data->get(0));
        res->add(this->data->get(1));
        res->add(new Token(";"));
        return res;
    }
    else{
        qDebug()<<"ERROR!!! - incomplete variable declaration on line : "<<this->lineNumber;
        return NULL;
    }
}
