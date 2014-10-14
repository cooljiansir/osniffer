OpenWrtSniffer: OpenWrtSniffer.o cgl.o pcapfunc.o snifferd
	$(CC) -L../libpcap-1.5.3/ -lpcap $(LDFLAGS) OpenWrtSniffer.o cgl.o pcapfunc.o -o OpenWrtSniffer
snifferd: snifferd.o
	$(CC) -L../libpcap-1.5.3/ -lpcap $(LDFLAGS) snifferd.o -o snifferd
OpenWrtSniffer.o: OpenWrtSniffer.c 
	$(CC)  -I../libpcap-1.5.3/  $(CFLAGS) -c OpenWrtSniffer.c -o OpenWrtSniffer.o
snifferd.o: snifferd.c
	$(CC) $(CFLAGS) -I../libpcap-1.5.3/ $(CFLAGS) -c snifferd.c -o snifferd.o
cgl.o: cgl.c
	$(CC) $(CFLAGS) -c cgl.c -o cgl.o
pcapfunc.o: pcapfunc.c
	$(CC) $(CFLAGS) -c pcapfunc.c -o pcapfunc.o
clean:
	rm *.o OpenWrtSniffer
