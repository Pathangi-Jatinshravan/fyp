#include "jatinhyper.h"

void print_packet(char *msg, struct sk_buff *skb) {
	struct tcphdr *tcph;
	struct iphdr *ip_header;
	struct ethhdr *eth;
	char *it, *user_data, *tail;
	char data[104];
	unsigned int counter = 0;
	unsigned int index = 0;
	
	char HANDSHAKE[] = {'1', '6', '0', '3', '0', '2', '0', '0', '2', 'f'};
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
		if (handshake && c == HANDSHAKE[counter]) {
			handshake = true;
			data[index++] = c;
			counter++;
 		} else if (c == '\0') {
			handshake = false;
			printk ("print_packet: Not a handshake message\n");
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
	
	printk("print_packet dataval: %s\n", data);

}
