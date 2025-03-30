#include <iostream>
#include "func.h"
#include "global.h"
#include "globalVar.h"
#include "lexical.h"
#include "ll1.h"
#include "newll1.h"
#include "semantic.h"
#include "mid.h"
#include "tim.h"

using namespace std;
int mmc = 1;

int main()
{
    //lexical
    FILE*fp = fopen("test.txt", "r");
    lexicalAnalyse(fp);
    printTokenList();
    fclose(fp);
    if(lexical_correct == 0){
        cout<<"出现词法错误，重新输入！"<<endl;
        return -1;
    }
    //recursion
    TreeNode* t = program();

    if (tokenList[Index].wd.tok == ENDFILE) {
        cout << "Recursion success!" << endl;
        printTree(t, 0, true);
    }

    else{ cout << "Recursion defeat!" << endl;
        mmc = 0;
        // longjmp(jump_buffer, 1);
    }
    //oldLL1

    cout << "***************************** LL1(old) *****************************" << endl;
    treeroot = programLL1();
    printTreeLL1(treeroot, 0, false);
    //newLL1
    Node* miao;
    cout << "***************************** LL1(new) *****************************" << endl;
    Index = 0;
    initLL1();
    ParserLL1 test;
    SyntaxTree* t_root = test.run();
    Node* res = t_root->getRoot();
    /* vector<int> v(5, 0);
    show(res, 0, v, false);*/
    if (is_newLL1_correct)
        printTreeNewLL1(res, 0, true);
    miao = res;

    ////semantic
    if(mmc == 0)
    {
        cout<<"错误"<<endl;
        return -1;
        //longjmp(jump_buffer, 1);
    }
    TreeNode* tt = treeroot;
    cout << "***************************** Semantic *****************************" << endl;
    tt = tt->child[1];
    //cout << tt->name;
    // 现在t是DeclarePart
    for (int i = 0; i < tt->child.size() && tt->child[i]; i++)
    {
        TreeNode* temp = tt->child[i];
        if (temp->name == "TypeDec")
            typeSaveTable(temp, smbltable);
        else if (temp->name == "VarDec")
            varSaveTable(temp, smbltable);
        else if (temp->name == "ProcDec")
        {
            SymbolTable* p = NULL;
            procSaveTable(temp, smbltable, p);
            // cout << "here"<< endl;
        }
    }

    Check_STEP3(treeroot, smbltable);

    SymbolTable* head = new SymbolTable();
    head->addRecord(treeroot->child[0]->child.back()->child[0]->name, "procKind", "program", tt->tk, smbltable);
    head->printTable();

    if(is_correct_semantic == 0){
        //longjmp(jump_buffer, 1);
        cout<<"erro"<<endl;
        return -1;
    }
    
    ////mid
    Node* ttt=miao;
    MidProcess* midProcess = new MidProcess(ttt);
    midProcess->printMidCode();

    //Tim
    cout<<"************************************** tim code **************************************"<<endl;
    Tim* tim = new Tim(Tim::readIntermediateCode("mid.txt"));
    tim->codeGen();


    return 0;
}




