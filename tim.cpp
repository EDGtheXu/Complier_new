
#include<iostream>
#include"tim.h"

void Tim::operandGen(ArgRecord* arg) {
	if (arg->form== "ValueForm") {
		cout<<line++<<":" << "LDC 0," << arg->value << ",0" << endl;
	}
	else if (arg->form == "LabelForm") {
		cout<<line++<<":" << "LDC 0," << arg->label<< ",0" << endl;
	}
	else if (arg->form== "AddrForm"&&(arg->access==1)) {
		cout<<line++<<":" << "LD 0,0,0" << endl;
	}
	else {
		cout<<line++<<":" << "LD 1,0,ac\nLD,0,0,1" << endl;
	}
}



void Tim::FindAddr(ArgRecord* arg) {
	int vl = arg->dataLevel;
	int vo = arg->dataOff;
	if (arg->access == 1) {
		FindSp(vl);
		cout<<line++<<":" << "LDC 1,"<<vo<<",0" << endl;
		cout << line++ << ":" << "ADD 0,0,1"<< endl;
	}
	else {
		cout << line++ << ":" << "LDC 1," << vo << ",0" << endl;
		cout << line++ << ":" << "ADD 0,6,1" << endl;
	}
}

void Tim::FindSp(int varlevel) {
	cout<<line++<<":" << "LD 0,"<<varlevel<<",4" << endl;
	cout<<line++<<":" << "LD 0,0,6" << endl;
	cout<<line++<<":" << "LD 0,0,0" << endl;
	return ;
}

void Tim::codeGen() {
    Argnode* a = head;
	//cout<<line++<<":" << "ST 0,0(0)\n* 为主程序入口预留跳转空间\nJMP savedLoc\n* 主程序入口\nsavedLoc : " << endl;
    
	while (a!=nullptr) {
		
		OPT TYPE;
		TYPE = getType(a);
		
		switch (TYPE) {
			case ADD:
			case SUB:
			case MULT:
			case DIV:
				arithGen(a);
				break;
			case AADD:
				aaddGen(a);
				break;
			case READC:
				readGen(a);
				break;
			case WRITEC:
				writeGen(a);
				break;
			case RETURNC:
				returnGen(a);
				break;
			case ASSIG:
				assigGen(a);
			case LABEL:
				labelGen(a);
				break;
			case JUMP:
				jumpGen(a);
				break;
			case JUMP0:
				jump0Gen(a);
				break;
			case VALACT:
				valactGen(a);
				break;
			case CALL:
				callGen(a);
				break;
			case PENTRY:
				pentryGen(a);
				break;
			case ENDPROC:
				endprocGen(a);
				break;
			case MENTRY:
				mentryGen(a);
				break;

		}
		a = a->next;
		//cout<<line++<<":" << endl;
	}
	cout<<line++<<":" << "HALT 0,0,0" << endl;
}
OPT Tim::getType(Argnode* a) {
	//cout<<line++<<":" << "codekind" << a->codekind <<  endl;
	if (a->codekind == "ADD") return ADD;
	if (a->codekind == "SUB") return SUB;
	if (a->codekind == "MULT") return MULT;
	if (a->codekind == "DIV") return DIV;
	if (a->codekind == "AADD") return AADD;
	if (a->codekind == "READC") return READC;
	if (a->codekind == "WRITEC") return WRITEC;
	if (a->codekind == "RETURNC") return RETURNC;
	if (a->codekind == "ASSIGN") return ASSIG;
	if (a->codekind == "LABEL") return LABEL;
	if (a->codekind == "JUMP") return JUMP;
	if (a->codekind == "JUMP0") return JUMP0;
	if (a->codekind == "CALL") return CALL;
	if (a->codekind == "VALACT") return VALACT;
	if (a->codekind == "VARACT") return VARACT;
	if (a->codekind == "PENTRY") return PENTRY;
	if (a->codekind == "ENDPROC") return ENDPROC;
	if (a->codekind == "MENTRY") return MENTRY;
	//cout<<line++<<":" << "??" << endl;
	return ADD; // 默认值
}
void Tim::arithGen(Argnode* a) {
	operandGen(a->Arg1);
	cout<<line++<<":" << "LDA 1,0,0"<<endl;
	operandGen(a->Arg2);
	cout<<line++<<":" << "LDA 1,0,2" << endl;
	OPT TYPE = getType(a);
	switch (TYPE) {
	case ADD:
		cout<<line++<<":" << "ADD 0,1,0" << endl;
		break;
	case SUB:
		cout<<line++<<":" << "SUB 0,1,0" << endl;
		break;

	case MULT:
		cout<<line++<<":" << "MUL 0,1,0" << endl;
		break;
	case DIV:
		cout<<line++<<":" << "DIV 0,1,0" << endl;
		break;
	default:
		break;
	}
	cout<<line++<<":" << "LDA 2,0,0" << endl;
	FindAddr(a->res);
	cout<<line++<<":" << "LDA 1,0,2" << endl;
	cout<<line++<<":" << "ST 1,0,0" << endl;
}
void Tim::aaddGen(Argnode* a) {
	if (a->Arg1->access == 1) {
		FindAddr(a->Arg1);
	}
	else {
		FindAddr(a->Arg1);
		cout<<line++<<":" << "LD 0,0,0" << endl;
	}
	cout<<line++<<":" << "LDA 2,0,0" << endl;
	operandGen(a->Arg2);
	cout<<line++<<":" << "ADD 2,2,0" << endl;
	FindAddr(a->res);
	cout<<line++<<":" << "ST 2,0,0" <<endl;
}
void Tim::readGen(Argnode* a) {
	cout<<line++<<":" << "IN 2,0,0" << endl;
	FindAddr(a->Arg1);
	if (a->Arg1->access == 1) {
		cout<<line++<<":" << "ST 2,0,0" << endl;
	}
	else {
		cout<<line++<<":" << "LD 1,0,ac\n" << "ST,2,0,1" << endl;
	}
}
void Tim::writeGen(Argnode* a) {
	operandGen(a->Arg1);
	cout<<line++<<":" << "OUT 0,0,0" << endl;
}
void Tim::returnGen(Argnode* a) {
	endprocGen(a);
}
void Tim::assigGen(Argnode* a) {
	FindAddr(a->Arg2);
	cout<<line++<<":" << "LDA 2,0,0" << endl;
	operandGen(a->Arg1);
	if (a->Arg2->access == 1) {
		cout<<line++<<":" << "LD 1,0,2" << endl;
		cout<<line++<<":" << "ST 0,0,1" << endl;

	}
	else {
		cout<<line++<<":" << "ST 0,0,2" << endl;
	}
}
void Tim::labelGen(Argnode* a) {
	cout<<line++<<":" << "L" << a->res->label << ":" << endl;
}
void Tim::jumpGen(Argnode* a) {
	cout<<line++<<":" << "JMP 0,0,L" << a->res->label << endl;
}
void Tim::jump0Gen(Argnode* a) {
	operandGen(a->Arg1); // 加载 Arg1 到 ac
	cout<<line++<<":" << "JEQ,0,0,L" << a->res->label << endl;
}
void Tim::valactGen(Argnode* a) {
	operandGen(a->Arg1);              // 加载值到 ac
	cout<<line++<<":" << "ST,0,<param_offset>,sp" << endl; // 存储到栈中的参数位置
}
void Tim::varactGen(Argnode* a) {
	FindAddr(a->Arg1);                // 获取变量地址到 ac
	cout<<line++<<":" << "ST,0,<param_offset>,sp" << endl; // 存储地址到栈中
}
void Tim::callGen(Argnode* a) {
	cout<<line++<<":" << "CALL,0,0,L" << a->res->label << endl; // 调用函数
}
void Tim::pentryGen(Argnode* a) {
	cout<<line++<<":" << "ST,fp,0,sp" << endl;      // 保存旧的 fp 到栈顶
	cout<<line++<<":" << "LDA,fp,0,sp" << endl;     // 设置新的 fp 为当前 sp
	cout<<line++<<":" << "SUB,sp,sp,<local_size>" << endl; // 分配局部变量空间
}
void Tim::endprocGen(Argnode* a) {
	cout<<line++<<":" << "ADD,sp,sp,<local_size>" << endl; // 释放局部变量空间
	cout<<line++<<":" << "LD,fp,0,sp" << endl;             // 恢复旧的 fp
	cout<<line++<<":" << "RET,0,0,0" << endl;              // 返回
}
void Tim::mentryGen(Argnode* a) {
	cout<<line++<<":" << "LDA,sp,stack_top,0" << endl; // 初始化栈指针
	cout<<line++<<":" << "LDA,fp,stack_top,0" << endl; // 初始化帧指针
}

// 测试函数
void Tim::printArgnodeList() {
    Argnode* curr = head;
    int lineNum = 0;
    while (curr != nullptr) {
        cout << "Line " << lineNum++ << ": " << endl;
        cout << "  codekind: " << curr->codekind << endl;
        cout << "  Arg1: form=" << curr->Arg1->form << ", name=" << curr->Arg1->name
            << ", value=" << curr->Arg1->value << ", dataLevel=" << curr->Arg1->dataLevel
            << ", dataOff=" << curr->Arg1->dataOff << endl;
        cout << "  Arg2: form=" << curr->Arg2->form << ", name=" << curr->Arg2->name
            << ", value=" << curr->Arg2->value << ", dataLevel=" << curr->Arg2->dataLevel
            << ", dataOff=" << curr->Arg2->dataOff << endl;
        cout << "  res:  form=" << curr->res->form << ", name=" << curr->res->name
            << ", value=" << curr->res->value << ", dataLevel=" << curr->res->dataLevel
            << ", dataOff=" << curr->res->dataOff << endl;
        curr = curr->next;
    }
}

Argnode* Tim::readIntermediateCode(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return nullptr;
        }

        Argnode* head = nullptr; // 链表头
        Argnode* tail = nullptr; // 链表尾，用于追加节点
        string line;

        while (getline(file, line)) {
            // 去除首尾空格
            line = line.substr(line.find_first_not_of(" \t"), line.find_last_not_of(" \t") + 1);

            // 检查是否为空行
            if (line.empty()) continue;

            // 解析格式：( 操作符 , 操作数1 , 操作数2 , 结果 )
            stringstream ss(line.substr(1, line.size() - 2)); // 去掉括号
            string codekind, arg1, arg2, res;
            getline(ss, codekind, ','); // 读取操作符
            getline(ss, arg1, ',');     // 读取操作数1
            getline(ss, arg2, ',');     // 读取操作数2
            getline(ss, res, ',');      // 读取结果

            // 去除每个字段的首尾空格
            codekind = codekind.substr(codekind.find_first_not_of(" \t"), codekind.find_last_not_of(" \t") + 1);
            arg1 = arg1.substr(arg1.find_first_not_of(" \t"), arg1.find_last_not_of(" \t") + 1);
            arg2 = arg2.substr(arg2.find_first_not_of(" \t"), arg2.find_last_not_of(" \t") + 1);
            res = res.substr(res.find_first_not_of(" \t"), res.find_last_not_of(" \t") + 1);

            // 创建新节点
            Argnode* node = new Argnode();
            node->codekind = codekind;
            node->Arg1 = new ARG(arg1);
            node->Arg2 = new ARG(arg2);
            node->res = new ARG(res);
            node->next = nullptr;

            // 链接到链表
            if (head == nullptr) {
                head = tail = node;
            }
            else {
                tail->next = node;
                tail = node;
            }
        }

        file.close();
        return head;
    }