#include <bits/stdc++.h>
using namespace std;

class production{
public:
    int n;
    vector<char> LHS;
    vector<vector<string>> RHS;
    vector<char> ans;
    unordered_map<char,unordered_set<char>> first;
    unordered_map<char,unordered_set<char>> follow;
    unordered_set<char> visited;
    unordered_set<char> terminals;
    unordered_map<char, unordered_map<int,unordered_set<string>>> table;
    bool LLone;
    
    production(const int sz);
    void init(const vector<string> &productions);
    void recurse_first(char v,unordered_map<char,vector<char>>&g);
    void create(string curr_production, int idx);
    void calculate_first();
	void calculate_follow();    
    void generate_table(bool display = true);
    void parse_string(string input);
    void checkLL1();
};

//visited and ans are being used during dfs
// for table for every X: int represents non terminals