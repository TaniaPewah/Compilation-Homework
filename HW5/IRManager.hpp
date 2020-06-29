#ifndef IR_MANAGER_HPP
#define IR_MANAGER_HPP

using namespace std;
#include <iostream>
#include <string>
#include <unordered_map> 
#include "bp.hpp"
#include "hw3_output.hpp"

class VarNode;
class ExpNode;
class LabelNode;


class Register {
	int num;
    string name;
	bool used;
    
public: 
    Register() : num(0), name("invalid"), used(false) {}
    Register( int num, string name) : num(num), name(name), used(false){}
    bool isFree() {
		return !used;
	}
	void free() {
		used = false;
	}
	void setUsed() {
		used = true;
	}
	int getNum() {
		return num;
	}
	string getName() {
		return name;
	}
};

struct BackpatchInfo
{
    /* data */
    int branch_location;
    string label_false;
    string label_true;
};


class IRManager {
public:
   static IRManager *instance;
    int register_index = 0;
    int global_register_index = 0;
    int stack_offset_pointer = 0;
    CodeBuffer& codeBuffer = CodeBuffer::instance();
    unordered_map<int, vector<pair<int,BranchLabelIndex>>> list_of_labels;
    int label_list_key_gen = 0;

   // Private constructor so that no objects can be created.
   IRManager() { 
       list_of_labels = unordered_map<int, vector<pair<int,BranchLabelIndex>>>();
    }

    IRManager(IRManager const&)       = delete;
    IRManager& operator=(IRManager const&)  = delete;
    static IRManager *getInstance() {
        if (!instance)
        instance = new IRManager;
        return instance;
    }

    int addPointerToRegisterInStack(string llvm_reg);
    void assignExpNodeToVar(string variable_reg, string exp_node_reg, string exp_node_type);
    Register* getFreshReg();
    Register* getGlobalFreshReg();
    int emitToBuffer(string command);
    void emitGlobalToBuffer(string command);
    void endProgram();
    string fromI8RegisterToI32Register(string type, string original_register);
    void loadID(string type, string reg, string id_name);
    void exitFunction();
    void zeroExit();
    void zeroError();
    BackpatchInfo handlerDivZero(string exp_b_type, string exp_b_reg);
    void handlePatching(BackpatchInfo patching_info);
    string createLabel();
    void andPatching( ExpNode* node_a, ExpNode* node_b, LabelNode* MAlabel, ExpNode* resultExp);
    void orPatching( ExpNode* node_a, ExpNode* node_b, LabelNode* MOlabel, ExpNode* resultExp);
    void createFalseListAndTrueList(ExpNode* bool_node, string bool_sign);
    void expPassListNotRule(ExpNode* old_node, ExpNode* new_node);

};

#endif //IR_MANAGER_HPP