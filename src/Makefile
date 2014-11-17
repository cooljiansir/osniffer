LDFLAGS+= -lpcap -lpthread


OpenWrtSniffer: OpenWrtSniffer.o cgl.o pcapfunc.o snifferd
	$(CC)  OpenWrtSniffer.o cgl.o pcapfunc.o -o OpenWrtSniffer $(LDFLAGS)
snifferd: snifferd.o pcapserver.o
	$(CC) snifferd.o pcapserver.o -o snifferd $(LDFLAGS)
OpenWrtSniffer.o: OpenWrtSniffer.c 
	$(CC)  -c OpenWrtSniffer.c -o OpenWrtSniffer.o $(CFLAGS)
snifferd.o: snifferd.c
	$(CC) -c snifferd.c -o snifferd.o $(CFLAGS)
cgl.o: cgl.c
	$(CC) -c cgl.c -o cgl.o $(CFLAGS)
pcapfunc.o: pcapfunc.c
	$(CC) -c pcapfunc.c -o pcapfunc.o $(CFLAGS)
pcapserver.o: pcapserver.c
	$(CC) -c pcapserver.c -o pcapserver.o $(CFLAGS)
clean:
	rm *.o OpenWrtSniffer snifferd
