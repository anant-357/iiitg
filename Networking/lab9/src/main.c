#include <stdio.h>
#include <string.h>

struct entry{
	char net_ml[14];
	char next_hop;
}typedef entry;

entry new(char net[14], char hop){
	entry new_entry;
	strcpy(new_entry.net_ml,net);
	new_entry.next_hop = hop;
	return new_entry;
}


int main(){
	
}
