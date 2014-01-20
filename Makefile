all: prepare_pcap feats

prepare_pcap: 
	$(MAKE) -C prepare/ BASE=$(PWD)
feats:
	$(MAKE) -C feats/ BASE=$(PWD)

clean: 
	$(MAKE) -C prepare/ clean
	$(MAKE) -C feats/ clean
