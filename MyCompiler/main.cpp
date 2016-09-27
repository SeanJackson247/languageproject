#include <QCoreApplication>
#include "filereader.h"
#include "filewriter.h"
#include "compiler.h"

#include <QDebug>
#include "tokenlist.h"
#include "line.h"
#include "linelist.h"

void rpn_testing(){
 //   RPNConverter* rpnConverter = new RPNConverter();
/*    QVector<QString> testData = {
        "x",
        "=",
        "(",
        "4",
        "*",
        "2",
        ")",
        "+",
        "4"
    };
    qDebug() << rpnConverter->convert2RPN(testData);
    testData = {
            "(",
            "x",
            "==",
            "4",
            ")"
    };
    qDebug() << rpnConverter->convert2RPN(testData);


*/
/*    QVector<QString>
    testData = {
        "G",
        "=",
        "(",
        "(",
        "360",
        "/",
        "Math",
        ".",
        "PI",
        ")",
        "*",
        "4",
        ")",
        "+",
        "(",
        "16",
        "*",
        "4",
        ")",
        "*",
        "10"
    };

    qDebug()<<"Test Data...";

    QVector<Token*> rpn = rpnConverter->convert2RPN(testData);

    qDebug()<<"RPN Done...";

    qDebug() << rpn;

    delete rpnConverter;*/
}

void token_testing(){
 /*   qDebug()<<"Token Testing...";
    Token* t = new Token("varname");
    qDebug()<<t->toString(0);
    TokenList* tl = new TokenList();
    tl->add(t);
    qDebug()<<tl->toString(0);*/
}

void line_testing(){
    qDebug().noquote()<<"Line Testing...";
    Line* l = new Line(0,1,"x = add((2*4)/12,3);");
  //  LineList* ll = new LineList();
  //  ll->add(l);
  //  ll = LineList::hoistVariables(ll);
    l->adjustToRPN();
    qDebug().noquote()<<l->toString(0);
}

int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);
    Compiler* compiler = new Compiler();

    FileWriter* fileWriter = new FileWriter("C:/proto/src/compiled/");

    FileReader* fileReader = new FileReader("C:/proto/src/");

    QObject::connect(fileReader,SIGNAL(readFile(QString,QVector<QString>)),compiler,SLOT(doWork(QString,QVector<QString>)));

 //   QObject::connect(compiler,SIGNAL(workCompleted(QString,QByteArray)),fileWriter,SLOT(writeRequest(QString,QByteArray)));

    QObject::connect(compiler,SIGNAL(debugScript(QString,QString)),fileWriter,SLOT(debugRequest(QString,QString)));

    fileReader->loadFile("main.proto");
//    rpn_testing();
//    token_testing();
 //   line_testing();
    return a.exec();
}
