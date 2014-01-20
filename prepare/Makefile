all: prepare_pcap
CC=gcc

prepare_pcap: prepare_pcap.o
	$(CC) prepare_pcap.o -o prepare_pcap -lpcap -lm

prepare_pcap.o:
	$(CC) -c prepare_pcap.c -o prepare_pcap.o

clean:
	rm -rf *.o prepare_pcap
