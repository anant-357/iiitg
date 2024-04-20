#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include "ll.h"
using namespace std;


// initializes lhs and rhs of grammar
void production::create(string curr_production, int idx){
    string curr_right;
    bool found = false;
    for(int i=0;i<(int)curr_production.size();++i){
        // we have completed -> aka LHS
        if(i+1<(int)curr_production.size() and found == false and curr_production[i] == '-' and curr_production[i+1] == '>'){
            found = true;
        }
        if(found == false){
            LHS[idx] = curr_production[i];
        }
        else{
            curr_right.push_back(curr_production[i]);
        }
    }
    // current right hand side alternative of production
    string curr;
    // iterates over each character in right
    for(int i=0;i<(int)curr_right.size();++i){
        if(curr_right[i] == ' ' or curr_right[i] == '-' or curr_right[i] == '>'){
            continue;
        }
        if(curr_right[i] == '|'){
            RHS[idx].push_back(curr);
            curr.clear();
        }
        else{
            curr.push_back(curr_right[i]);
        }
    }
    RHS[idx].push_back(curr);
}

void production::calculate_first(){
    // map g to keep track of symbols that can appear after each non terminal
    unordered_map<char,vector<char>> g;
    //iterate over each production
    for(int i=0;i<n;++i){
        //iterate over each alternative production
        for(int j=0;j<(int)RHS[i].size();++j){
            //iterate over each symbol in alternative
            for(int k=0;k<(int)RHS[i][j].size();++k){
                bool isnontermial = (RHS[i][j][k] >='A' and RHS[i][j][k] <= 'Z');
                //add terminals to set of terminals
                if(RHS[i][j][k]!='#' and !isnontermial){
                    terminals.insert(RHS[i][j][k]);
                }
            }
            for(int k=0;k<(int)RHS[i][j].size();++k){
                if(RHS[i][j][k] >='A' and RHS[i][j][k] <= 'Z' and LHS[i]!=RHS[i][j][k]){
                    //if symbol is non terminal and different from current non terminal add it to the set of symbols
                    // that can appear after current non terminal
                    g[LHS[i]].push_back(RHS[i][j][k]);
                }
                else{
                    break;
                }
            }
        }
    }
    
    for(int i=0;i<n;++i){
        //for each non terminal calculate first set
        if(visited.count(LHS[i]) == false){
            recurse_first(LHS[i],g);
        }
    };
    
    for(int i=0;i<n;++i){
        //if curr non terminal is LHS of first production add terminals in first alternative to its first set
        if(LHS[i] == ans[0]){
            for(auto &j:RHS[i]){
                if(j[0] >= 'A' and j[0] <= 'Z'){
                    continue;
                }
                first[ans[0]].insert(j[0]);
            }
        }
    }
    for(int i=1;i<(int)ans.size();++i){
        for(int j=0;j<n;++j){
            //for each non terminal iterate over its alternatives and cal first
            if(LHS[j] == ans[i]){
                for(auto &k:RHS[j]){
                    //iterate over each symbol
                    for(int l=0;l<(int)k.size();++l){
                        if(k[l] >= 'A' and k[l] <= 'Z'){
                            //iterate over each symbol in first set
                            for(auto &x:first[k[l]]){
                                if(x != '#'){
                                    first[ans[i]].insert(x);
                                }
                                else{
                                    if(l+1 == (int)k.size()){
                                        first[ans[i]].insert(x);
                                    }
                                }
                            }
                            // if eps is not found break out of l
                            if(first[k[l]].find('#')==first[k[l]].end()){
                                break;
                            }
                        }
                        //if terminal
                        else{
                            first[ans[i]].insert(k[l]);
                            break;
                        }
                    }
                }
            }
        }
    }
    cout<<endl;
    cout<<"First : "<<endl;
    for(int i=0;i<(int)ans.size();++i){
        cout<<ans[i]<<" : ";
        for(auto &j:first[ans[i]]){
            cout<<j<<" ";
        }
        cout<<endl;
    }
}

void production::calculate_follow(){
    // to keep track of symbols that can follow non terminals
    unordered_map<char,vector<char>> g;
    ans.clear();
    visited.clear();
    for(int i=0;i<n;++i){
        for(int j=(int)RHS[i].size() - 1;j>=0;--j){
            //if last of alternative is non terminal and not LHS of production add LHS to follow set of non terminal
            if(RHS[i][j].back() >= 'A' and RHS[i][j].back() <= 'Z' and LHS[i] != RHS[i][j].back()){
                g[RHS[i][j].back()].push_back(LHS[i]);
            }
            //iterate over eacg symbol in alternative starting from second last
            for(int k=(int)RHS[i][j].size() - 2;k>=0;--k){
                if(RHS[i][j][k] >='A' and RHS[i][j][k] <= 'Z'){
                        char Next = RHS[i][j][k+1];
                        //If the next symbol is a non-terminal, and the current symbol can derive the empty string, add the left-hand side to the follow set of the current non-terminal.
                        if(Next >= 'A' and Next <= 'Z' and RHS[i][j][k]!=Next and first[Next].find('#')!=first[Next].end()){
                                g[RHS[i][j][k]].push_back(LHS[i]);
                        }
                        else{
                                break;
                        }
                    }
            }
        }
    }

    for(int i=0;i<n;++i){
        if(visited.count(LHS[i]) == false){
            recurse_first(LHS[i],g);
        }
    };

    follow[LHS[0]].insert('$');

    int ptr = 0;
    while(ptr < 2){
        //iterate over non terminal
        for(int x=0;x<n;++x){
            //over each production
            for(int i=0;i<n;++i){
                //iterate over each alternative RHS
                for(int j=(int)RHS[i].size() - 1;j>=0;--j){
                    unordered_set<char> curr_follow_set;
                    for(auto &z:follow[LHS[i]]){
                        if(z != '#'){
                            curr_follow_set.insert(z);
                        }
                    }
                    for(int k=(int)RHS[i][j].size() - 1;k>=0;--k){
                        // if symbol is same as non terminal in a loop eg E->TE
                        if(RHS[i][j][k] == ans[x]){
                            for(auto &z:curr_follow_set){
                                follow[ans[x]].insert(z);
                            }
                            }
                            else{
                                if(RHS[i][j][k] >= 'A' and RHS[i][j][k] <= 'Z'){
                                    if(first[RHS[i][j][k]].find('#') != first[RHS[i][j][k]].end()){
                                        for(auto &z:first[RHS[i][j][k]]){
                                            if(z != '#'){
                                                curr_follow_set.insert(z);
                                            }
                                        }
                                    }
                                    //if terminal
                                    else{
                                        curr_follow_set.clear();
                                        for(auto &z:first[RHS[i][j][k]]){
                                            curr_follow_set.insert(z);
                                        }
                                    }
                                }
                                else{
                                    curr_follow_set.clear();
                                    if(RHS[i][j][k]!='#'){
                                        curr_follow_set.insert(RHS[i][j][k]);
                                    }
                                }
                            }
                    }
                }
            }
        }
        ptr++;
    }
    cout<<endl;
    cout<<"Follow : "<<endl;
    for(int i=0;i<(int)ans.size();++i){
        cout<<ans[i]<<" : ";
        for(auto &j:follow[ans[i]]){
            cout<<j<<" ";
        }
        cout<<endl;
    }
}
void production::generate_table(bool display){
    terminals.insert('$');
    for(int i=0;i<n;++i){
        for(int j=0;j<(int)RHS[i].size();++j){
            unordered_set<char> curr_first;
            for(int k=0;k<(int)RHS[i][j].size();++k){
                char curr_symbol = RHS[i][j][k];
                bool isnontermial = (RHS[i][j][k] >='A' and RHS[i][j][k] <= 'Z');
                if(!isnontermial){
                    if(curr_symbol == '#'){
                        curr_first.insert('#');
                        break;
                    }
                    else{
                        curr_first.insert(curr_symbol);
                        break;
                    }
                }
                else{
                    for(auto &x:first[curr_symbol]){
                        curr_first.insert(x);
                    }
                    if(first[curr_symbol].find('#')==first[curr_symbol].end()){
                        break;
                    }
                }
            }
            for(auto &x:curr_first){
                string extra = {LHS[i]};
                extra = extra + "->" + RHS[i][j];
                if(x != '#') table[LHS[i]][x].insert(extra);
            }
            if(curr_first.find('#')!=curr_first.end()){
                for(auto &x:follow[LHS[i]]){
                    string extra = {LHS[i]};
                    extra = extra + "->" + RHS[i][j];
                    table[LHS[i]][x].insert(extra);
                }
            }
        }
    }
    if(display){
            cout<<endl<<"LL(1) table"<<endl;
            unordered_map<char,int> col_width;
            for(int i=0;i<n;++i){
            for(auto &x:terminals){
                int curr = 0;
                if(table[LHS[i]][x].size() == 0){
                    curr = 6;
                }
                else{
                    for(auto &y:table[LHS[i]][x]){
                        curr += y.size()+1;
                    }
                }
                col_width[x] = max(col_width[x], curr);
            }
            }
            int row_len = 3;
            cout<<"   ";
            for(auto &x:terminals){
            cout<<setw(col_width[x]-1)<<x<<"  |";
            row_len += col_width[x]-1 + 3;
            }
            cout<<endl;
            for(int i=0;i<row_len;++i){
            cout<<"-";
            }
            cout<<endl;
        for(int i=0;i<n;++i){
                cout<<LHS[i]<<" |";
            for(auto &x:terminals){
                cout<<setw(col_width[x]);
                if(table[LHS[i]][x].size() == 0){
                    cout<<"- ";
                }
                else{
                    string z;
                    for(auto &y: table[LHS[i]][x]){
                        z += y + " ";
                    }
                    if(table[LHS[i]][x].size() > 1){
                        LLone = false;
                    }
                    cout<<z;
            }
            cout<<" |";
            }
            cout<<endl;
        }
        cout<<endl;
    }
} 

    //constructor for production
production::production(const int sz){
        LLone = true;
        ans.clear();
        first.clear();
        follow.clear();
        visited.clear();
        terminals.clear();
    n = sz;
}

//initializes production rules
void production::init(const vector<string> &productions){
    LHS.clear();
    RHS.clear();
    LHS.resize(n);
    RHS.resize(n);
    for(int i=0;i<(int)productions.size();++i){
        create(productions[i],i);
    }
}
void production::recurse_first(char v,unordered_map<char,vector<char>>&g){
    visited.insert(v);
    for(int i=0;i<(int)g[v].size();++i){
        int curr_ch = g[v][i];
        if(visited.count(curr_ch) == false){
            production::recurse_first(curr_ch,g);
        }
    }
    ans.push_back(v);
}   


