#include <iostream>
#include <fstream>
#include "ll.h"

using namespace std;

int main(int argc, char const *argv[]) {
    ifstream infile("productions");
    streambuf *cinbuf = cin.rdbuf();
    cin.rdbuf(infile.rdbuf());
    int n;
	cin>>n;
	vector<string> productions(n);
	for(int i=0;i<n;++i){
	    cin>>productions[i];
	}
	string input;
	cin>>input;
    cin.rdbuf(cinbuf);
	production p(n);
	p.init(productions);
	p.calculate_first();
	p.calculate_follow();
    ofstream outfile("parse_table");
    streambuf *coutbuf = cout.rdbuf(); 
    cout.rdbuf(outfile.rdbuf()); 
    p.generate_table();
    cout.rdbuf(coutbuf);
    p.checkLL1(); 
	p.parse_string(input);
	return 0;
}
