#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct entry {
    char net_ml[15]; // Including space for '\0'
    char next_hop;
};

int matchIP(char ip1[], char ip2[], int prefix_len) {
	int i;
    	for (i = 0; i < prefix_len / 4; i++) {
		char c1 = ip1[i];
        	char c2 = ip2[i];

        	int val1 = (c1 >= '0' && c1 <= '9') ? (c1 - '0') : (c1 - 'A' + 10);
        	int val2 = (c2 >= '0' && c2 <= '9') ? (c2 - '0') : (c2 - 'A' + 10);

        	for (int j = 0; j < 4; j++) {
            		int bit1 = (val1 >> (3 - j)) & 1;
            		int bit2 = (val2 >> (3 - j)) & 1;

            		if (bit1 != bit2) {
                		return 0; // Mismatch
            		}
        	}
    	}
    	return 1;
}

char findNextHop(struct entry routing_table[], int table_size, char dest_ip[]) {
    char next_hop = ' '; 
    int max_prefix_len = -1;

    for (int i = 0; i < table_size; i++) {
        char net_ip[15];
        char prefix_len_str[3];

        sscanf(routing_table[i].net_ml, "%[^/]/%s", net_ip, prefix_len_str);

	int prefix_len = atoi(prefix_len_str);

	printf("%s : %d\n", net_ip, prefix_len);
        if (matchIP(dest_ip, net_ip, prefix_len) && prefix_len > max_prefix_len) {
            max_prefix_len = prefix_len;
            next_hop = routing_table[i].next_hop;
        }
    }

    return next_hop;
}

int main() {
    struct entry routing_table[] = {
        {"C4.50.0.0/12", 'A'},
        {"C4.5E.10.0/20", 'B'},
        {"C4.60.0.0/12", 'C'},
        {"C4.68.0.0/14", 'D'},
        {"80.0.0.0/1", 'E'},
        {"40.0.0.0/2", 'F'},
        {"00.0.0.0/2", 'G'},
    };

    char dest_ip[15];
    printf("Enter the destination IP address: ");
    scanf("%s", dest_ip);

    char next_hop = findNextHop(routing_table, sizeof(routing_table) / sizeof(routing_table[0]), dest_ip);

    if (next_hop != ' ') {
        printf("Next hop: %c\n", next_hop);
    } else {
        printf("No matching entry in the routing table.\n");
    }

    return 0;
}

