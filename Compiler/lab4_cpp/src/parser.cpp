#include <bits/stdc++.h>
#include <iostream>
#include "ll.h"

using namespace std;

void production::parse_string(string input){
        if(!LLone){
            return;
        }
        input.push_back('$');
        stack<char>s;
        s.push('$');
        s.push(LHS[0]);
        int ptr = 0;
        bool f = true;
        while(ptr<(int)input.length() and s.empty() == false){
            char curr = s.top();
            s.pop();
            bool isnontermial = (curr >= 'A' and curr <= 'Z');
            if(isnontermial){
                if(table[curr][input[ptr]].size() == 0 or table[curr][input[ptr]].size() > 1){
                    f = false;
                    break;
                }
                else{
                    string cc = (*table[curr][input[ptr]].begin());
                    cout << cc << endl;
                    if(cc.back() =='#'){
                        continue;
                    }
                    for(int i=(int)cc.length() - 1; i>=0;--i){
                    	if(cc[i] == '>' or cc[i] == '-'){
                    		break;
                    	}
                        s.push(cc[i]);
                    }
                }
            }
            else{
                if(input[ptr] == curr){
                    ptr++;
                    continue;
                }
                else{
                    f = false;
                    break;
                }
            }
        }
        while(s.empty() == false){
        	cout<<s.top()<<endl;
        	s.pop();
        }
        if(s.empty() and f){
            cout<<"Parsed successfully"<<endl;
        }
        else{
            cout<<"Not Parsed!"<<endl;
        }
    }

void production::checkLL1(){
    for(int i=0;i<n;i++){
        for(auto r:RHS[i]){
            if(r[0]==LHS[i])
                LLone = false;
        }
    }

    if(LLone){
        cout << "The grammar can be used for LL1 parsing" << endl;
    }else{
        cout << "This grammar cannot be used for LL1 parsing" << endl;
    }
}
