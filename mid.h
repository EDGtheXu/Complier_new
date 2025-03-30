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

class MidProcess{


};

extern vector<Argnode> midtable;

struct midprintnode {
    string op;
    string m1;
    string m2;
    string m3;
};

extern vector<Node*> ArrayTable;
extern vector<Node*> FieldTable;

extern int level;


extern unordered_map<int, vector<pair<int, Node*>>> mapArray;
extern unordered_map<int, vector<pair<int, Node*>>> mapRecord;
extern vector<midprintnode> MidPrintNodeTable;

extern int tmpidx;


extern void GenAssignS(Node* t);
extern void GenCallS(Node* t);
extern void GenReadS(Node* t);
extern void GenWriteS(Node* t);
extern void GenIfS(Node* t);
extern void GenWhileS(Node* t);
extern void GenBody(Node* t);
extern string getBehind(Node* t);
extern void GenStatement(Node* t);
extern ARG* GenArray(Node* t);
extern ARG* GenField(Node* t);

extern void ProcessChild(Node* t);

extern void processMid(Node* t);

extern void printMidCode(vector<Argnode>& vec);

extern void GenProcDec(Node*);
extern void GenCode(vector<Argnode>& vec, string codekind, ARG* arg1, ARG* arg2, ARG* arg3);
extern void GenDeclare(Node* t);

extern void GenBody(Node* t,string);
