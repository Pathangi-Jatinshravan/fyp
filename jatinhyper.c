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

void print_packet(char *msg, struct sk_buff *skb) {
	struct tcphdr *tcph;
	struct iphdr *ip_header;
	struct ethhdr *eth;
	char *it, *user_data, *tail;
	char data[104];
	char *hex_data;
	unsigned int counter = 0;
	unsigned int index = 0;
	
	char HANDSHAKE[] = {'1', '6', '0', '3'};
	bool handshake = true;

	tcph = tcp_hdr(skb);
	eth = (struct ethhdr *) skb_mac_header(skb);
	ip_header = (struct iphdr *) skb_network_header(skb);

	user_data = (unsigned char *)((unsigned char *)tcph + (tcph->doff * 4));
	tail = skb_tail_pointer(skb);
		
	printk("print_packet from %s, data:\n", msg);
	printk("\n\n");
	printk("user_data: %s\n",user_data); 
	for (it=user_data; it!=tail; it++) {
		char c = *(char *) it;
		/*
		if (index <=51) {	
			data[index++] = c;
		} else {
			break;
		}
		*/	
		if (counter <= 3 && c == HANDSHAKE[counter]) {
			data[index++] = c;
			counter++;
 		} else if (counter > 3) {
			data[index++] = c;
			if (index == 11) {
				if (data[index] != 1) {
					printk("print_packet: Not a client hello\n");
					break;
				} else {
					printk("print_packet: Found client hello\n");
					break;
				}
			}
		} else if (c == '\0') {
			break;
		}
		/*
		data[counter++] = c;	
		if (counter == 103) {
			printk("print_packet: Reached 103\n");
			break;
		}
		*/
	
	}
	
	//hex_data = decode(data);
	printk("print_packet dataval: %s\n", data);
	//kfree(hex_data);
}
