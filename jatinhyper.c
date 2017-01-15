#include "jatinhyper.h"

void print_packet(char *msg, struct sk_buff *skb) {
	struct tcphdr *tcph;
	struct iphdr *ip_header;
	struct ethhdr *eth;
	char *it, *user_data, *tail;
	char data[104];
	unsigned int counter = 0;

	user_data = (unsigned char *)((unsigned char *)tcph + (tcph->doff * 4));
	tail = skb_tail_pointer(skb);
	
	printk("print_packet from %s, data:\n", msg);
	printk("\n\n");

	if (*(char *)user_data == '1' && *(char *)((user_data + 1)) == '6'){
		for (it=user_data; it!=tail; it++) {
			char c = *(char *) it;		
			data[counter++] = c;	
			if (counter == 103) {
				break;
			}
		}
	}
	
	printk("print_packet : %s", data);
}
