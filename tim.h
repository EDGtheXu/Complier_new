#include "mid.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

typedef ARG ArgRecord;

struct CodeFile {
	string op;
};
enum OPT{ADD,SUB,MULT,DIV,EQC,LTC,READC,WRITEC,RETURNC,ASSIG,AADD,LABEL,JUMP,JUMP0,CALL,VARACT,VALACT,PENTRY,ENDPROC,MENTRY};


class Tim{

private:
    Argnode* head;
    
    int line = 0;



    void operandGen(ArgRecord* arg);
    void FindAddr(ArgRecord* arg);
    void FindSp(int varlevel);

    OPT getType(Argnode*);
    void arithGen(Argnode* a);
    void aaddGen(Argnode* a);
    void readGen(Argnode* a);
    void writeGen(Argnode* a);
    void returnGen(Argnode* a);
    void assigGen(Argnode* a);
    void labelGen(Argnode* a);
    void jumpGen(Argnode* a);
    void jump0Gen(Argnode* a);
    void valactGen(Argnode* a);
    void varactGen(Argnode* a);
    void callGen(Argnode* a);
    void pentryGen(Argnode* a);
    void endprocGen(Argnode* a);
    void mentryGen(Argnode* a);

public:
    Tim(Argnode* head):head(head){}
    void codeGen();
    // 测试函数
    void printArgnodeList();

    // 读取中间代码到 Argnode 链表的函数
    static Argnode* readIntermediateCode(const string& filename);

};