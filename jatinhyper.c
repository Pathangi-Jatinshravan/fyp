#include "jatinhyper.h"

#define BUFFER_SIZE 52

#define TLS_RSA_WITH_RC4_128_SHA "0005"

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

int print_packet(char *msg, struct sk_buff *skb) {
	struct tcphdr *tcph;
	struct iphdr *ip_header;
	struct ethhdr *eth;
	char *it, *user_data, *tail;
	char hex_data[10];
	bool client_hello = false;
	bool server_hello = false;
	char client_cipher_suite_length[2];
	char cipher_suite[10] = "";
	bool flag = false;
	
	unsigned int counter = 0;
	const char *HANDSHAKE[2];
	HANDSHAKE[0] = "16"; HANDSHAKE[1] = "03";

	tcph = tcp_hdr(skb);
	eth = (struct ethhdr *) skb_mac_header(skb);
	ip_header = (struct iphdr *) skb_network_header(skb);

	user_data = (unsigned char *)((unsigned char *)tcph + (tcph->doff * 4));
	tail = skb_tail_pointer(skb);
		
	printk("print_packet from %s, data:\n", msg);
	printk("\n\n");
	//printk("user_data: %s\n",user_data); 
	/*
	if (user_data == NULL || tail <= user_data || tail == NULL) {
		return;
	}
	*/
	if (strcmp(msg, "xenvif_tx_submit"))
		printk("user_data: ");
	else
		printk("user_data send: ");

	for (it=user_data; it!=tail; it++) {
		char c = *(char *) it;
		sprintf(hex_data, "%02X", c);
		/*
		if (c == '\0') {
			break;
		}*/ 
		
		if (counter <= 1 && !strcmp(hex_data, HANDSHAKE[counter])) {	
			counter++;		
		} else if (counter <=1 && strcmp(hex_data, HANDSHAKE[counter])){
			printk("%02X", c);
			printk("print_packet: Not a handshake message\n");
			break;
		} else if (counter >1) {
			
			if (counter == 5) {
				if (!strcmp(hex_data, "01")) 
					client_hello = true;
				else if (!strcmp(hex_data, "02")){ 
					server_hello = true;	
					printk("Server hello\n");
				}			
			} else if (counter == 45 && client_hello) {
				strcpy(client_cipher_suite_length, hex_data);
				printk("Client Cipher suites length: %s\n", client_cipher_suite_length);						
			} else if (counter <= 47 && client_hello && strlen(hex_data) == 2) {
				if (!strlen(cipher_suite))
					strcpy(cipher_suite, hex_data);
				else 
					strcpy(cipher_suite+2, hex_data);
			}

			counter++;	
		}
		printk("%02X ", c);
	
	}
	
	printk("\n\n");

	if (client_hello){
		printk ("Client's most preferred Cipher Suite: %s\n", cipher_suite);
		if (!strcmp(cipher_suite, "0005")){
			printk("Client's most preferred Cipher Suite: TLS_RSA_WITH_RC4_128_SHA\n");
			return 0;
		}
	}
	
	return 1;
}
