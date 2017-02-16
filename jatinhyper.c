#include "jatinhyper.h"

#define BUFFER_SIZE 52

char* decode(char data[]) {
	int i;
	char *res = kmalloc(2*BUFFER_SIZE + 1, GFP_KERNEL);
	char *ptr = res;
	
	for (i=0; i<BUFFER_SIZE; i++) {
		ptr += sprintf(ptr, "%02X", res[i]);
	}
	
	sprintf(ptr, "\n");
	*(ptr + 1) = '\0';

	return res;	
}
/*
bool hello_check(unsigned char *data, unsigned char *tail) {
	unsigned char HANDSHAKE[] = {1, 6, 0, 3};
	int i, j = 0;
	unsigned char *ptr = data;
	
	if (data == NULL) {
		return false;	
	}

	while (ptr != tail) {
		printk("%x ", *ptr); 		
		if (j == 3) {
			return true;
		}

		if (*ptr == HANDSHAKE[j]) {		
			j++;		
		} else {
			j = 0;
		}
		
		ptr++;
		
	}
	return false;	
} 
*/
void print_packet(char *msg, struct sk_buff *skb) {
	struct tcphdr *tcph;
	struct iphdr *ip_header;
	struct ethhdr *eth;
	char *it, *user_data, *tail;
	char hex_data[10];
	unsigned int counter = 0;
	unsigned int index = 0;
	int j=0;
	
	const char *HANDSHAKE[2];
	HANDSHAKE[0] = "16"; HANDSHAKE[1] = "03";

	tcph = tcp_hdr(skb);
	eth = (struct ethhdr *) skb_mac_header(skb);
	ip_header = (struct iphdr *) skb_network_header(skb);

	user_data = (unsigned char *)((unsigned char *)tcph + (tcph->doff * 4));
	tail = skb_tail_pointer(skb);
		
	//printk("print_packet from %s, data:\n", msg);
	printk("\n\n");
	//printk("user_data: %s\n",user_data); 
	
	if (user_data == NULL || tail <= user_data || tail == NULL) {
		return;
	}
	printk("user_data: ");
	for (it=user_data; it!=tail; it++) {
		char c = *(char *) it;
		sprintf(hex_data, "%02X", c);

		if (c == '\0') {
			break;
		} 
				
		if (counter <= 1 && !strcmp(hex_data, HANDSHAKE[counter])) {		
			counter++;		
		} else if (counter <=1 && strcmp(hex_data, HANDSHAKE[counter])){
			printk("print_packet: Not a handshake message\n");
			break;
		} else if (counter >1) {
			printk("print_packet: Handshake found %s.", hex_data);
			break;		
		}
		
		printk("%02X ", c);
	
	}

	
	//hex_data = decode(data);
	printk("\n\n");
	//kfree(hex_data);
}
