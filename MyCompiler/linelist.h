#ifndef LINELIST_H
#define LINELIST_H

#include <QVector>

class Line;

class LineList{
//public static variables...
public: static int CurrentLexicalID;
public:static QVector<LineList*> LineListsRegister;
public:static void clearLineListsRegister();
public:static void registerLineList(LineList* linelist);
public:static QVector<int> LexicalParentsList;
//public static methods...
public: static int getNextLexicalID();
public: static LineList* flattenControlFlows(LineList* lines);
public: static LineList* FlattenLexicalScopes(LineList* lines);
public: static LineList* hoistVariables(LineList* lines);
public: static LineList* RPNPadding(LineList *lines);
public: static LineList* SetLexicalMarkers(LineList *lines);
public: static LineList* SetLexicalMarkers(LineList *lines,int parentID);
public: static void setLexicalParent(int childID,int parentID);
//private instance variables...
private: QVector<Line*> _data;
//constructors...
public: LineList();
//public methods...
public: void add(Line* l);
public: void addFromList(LineList* ll);
public: Line* first();
public: Line* last();
public: void clear();
public: Line* get(int i);
public: void set(int i,Line* l);
public: void prepend(Line* l);
public: int size();
public:QString toString(int indent);
public:void clearChilds(int currentIndex);
};

#endif // LINELIST_H
