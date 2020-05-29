//
// Created by tt on 5/22/2020.
//

#ifndef HW3_SYMBOLTABLE_H
#define HW3_SYMBOLTABLE_H
#include "parser.hpp"
#include "hw3_output.hpp"
#include <map>
#include <vector>

using namespace std;


class TableEntry{
public:

    IdNode* node;
    bool is_var;

    TableEntry( IdNode* newNode, bool is_var ) : node(newNode), is_var(is_var) {

        if(is_var){
            cout<<"--------new table Var added: "<< node->name <<endl;
        }
        else{
            cout<<"--------new Function Var added: "<< node->name <<endl;
        }
    }
    //TODO: add FunctionTableEntry

};

class Scope{
public:
    map<string, TableEntry*> entries;
    int offset;

    Scope(int offset) : offset(offset) {}

    void addSymbolVar( VarNode* symbolToAdd ){
        TableEntry* entryToAdd = new TableEntry( symbolToAdd, true );
        entries.insert(make_pair(symbolToAdd->name, entryToAdd));
        offset++;
    }
    void addSymbolFunc( VarNode* funcToAdd ){
        TableEntry* entryToAdd = new TableEntry( funcToAdd, false );
        entries.insert(make_pair(funcToAdd->name, entryToAdd)); 
        offset++;
    }

    IdNode* findSymbolInScope(string key) {

        cout << "The entries in this scope are: "<< endl;
        for(auto key = entries.begin(); key != entries.end(); key++) {
            cout << key->first << " : " << key->second->node->lineno << "        ";
        }
        cout<<endl<<endl;

        map<string, TableEntry*>::iterator wanted_entry = entries.find(key);
        if (wanted_entry != entries.end()){
            return wanted_entry->second->node;
        }
        return NULL;
    }
};

class SymbolTable {
public:
    vector <Scope*> stack;

    SymbolTable() {}

    void newScope(){
         

        Scope* scopeToAdd;
        if (stack.empty()){
            scopeToAdd = new Scope(0);
            cout << "--First scope was created--" << endl;
        }
        else{
            scopeToAdd = new Scope(stack.back()->offset);
            cout << "--New scope was created--" << endl;
        }
        stack.push_back(scopeToAdd);
    }
    void closeScope(){
        stack.pop_back();
        output::endScope();
    }

    void addSymbolVar( VarNode* symbolToAdd ){
        stack.back()->addSymbolVar( symbolToAdd );
        cout << "~~~~~~~~~~~~~~~~~~~~~ added var to symbol table " << symbolToAdd->name << endl;
    }
    void addSymbolFunc( VarNode* symbolToAdd ){
        stack.back()->addSymbolFunc( symbolToAdd );
        cout << "~~~~~~~~~~~~~~~~~~~~~ added func to symbol table " << symbolToAdd->name << endl;
    }

    IdNode* findSymbolInStack(int lineno, string symbol_name){
         cout << "searching all stack"<<endl;

        for(int i = 0; i < stack.size(); i++){
            IdNode* entry = stack[i]->findSymbolInScope(symbol_name);
            if(entry != NULL) {
                return entry;
            }
        }

        output::errorUndef(lineno, symbol_name);
        exit(0);
    }

    string getIdType(int lineno, string id){
        cout << "finding type in getIdType"<<endl;
       return ((VarNode*)findSymbolInStack(lineno, id))->type;
    }
};

#endif //HW3_SYMBOLTABLE_H
