#pragma once
#include <iostream>
#include <vector>
#include "func.h"
#include "global.h"
#include "globalVar.h"
#include "lexical.h"
#include "LL1.h"
#include "newLL1.h"
#include "semantic.h"
#include <map>
#include <unordered_map>

using namespace std;

// ARG 类定义（假设已提供）
class ARG {
public:
    ARG() : form(""), value(0), label(0), Addr(""), name(""), dataLevel(0), dataOff(0), access(1) {}
    ARG(string _name) : form(""), value(0), label(0), Addr(_name), name(_name), dataLevel(0), dataOff(0), access(1) {
        if (_name == "-") {
            name = "";
        }
        else if (_name.find("temp") == 0) { // 临时变量
            form = "AddrForm";
            dataLevel = -1; // 临时变量层数为 -1
            dataOff = stoi(_name.substr(4)); // 提取 temp 后的数字
        }
        else if (isdigit(_name[0])) { // 常数
            form = "ValueForm";
            value = stoi(_name);
            name = "";
        }
        else { // 普通变量
            form = "AddrForm";
        }
    }

    string form;      // ValueForm, LabelForm, AddrForm
    int value;        // 常数值
    int label;        // 标号值
    string Addr;      // 变量地址
    string name;      // 变量名
    int dataLevel;    // 层数
    int dataOff;      // 偏移量
    int access;       // 访问方式 (1: dir, 2: indir)
};

// Argnode 结构定义（假设已提供）
struct Argnode {
    string codekind;
    ARG* Arg1;
    ARG* Arg2;
    ARG* res;
    Argnode* next;
};

struct midprintnode {
    string op;
    string m1;
    string m2;
    string m3;
};




class MidProcess{
private:
    vector<Argnode> midtable;

    vector<Node*> ArrayTable;
    vector<Node*> FieldTable;

    int level;
    string wacao = "";

    unordered_map<int, vector<pair<int, Node*>>> mapArray;
    unordered_map<int, vector<pair<int, Node*>>> mapRecord;
    vector<midprintnode> MidPrintNodeTable;

    int tmpidx;

    ARG* processQiantao(Node* t);
    ARG* NewTemp(int access);
    ARG* ARGValue(int value);
    int NewLabel();
    ARG* ARGLabel(int label);
    ARG* ARGAddr(const char* id, int level, int off, int access);
    void GenAssignS(Node* t);
    void GenCallS(Node* t);
    void GenReadS(Node* t);
    void GenWriteS(Node* t);
    void GenIfS(Node* t);
    void GenWhileS(Node* t);
    void GenBody(Node* t);
    string getBehind(Node* t);
    void GenStatement(Node* t);
    ARG* GenArray(Node* t);
    ARG* GenField(Node* t);
    // void ProcessChild(Node* t);
    void GenProcDec(Node*);
    void GenCode(vector<Argnode>& vec, string codekind, ARG* arg1, ARG* arg2, ARG* arg3);
    void GenDeclare(Node* t);
    void GenBody(Node* t,string);
    void processMid(Node * t);

public:
    MidProcess(Node* t){
        processMid(t);
    }
    
    void printMidCode();
};








