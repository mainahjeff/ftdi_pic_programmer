FILE=ftusb
make:
#	gcc -o ${FILE} ${FILE}.c /usr/local/lib/libftd2xx.so.1.4.27
#	gcc -o ${FILE}.o ${FILE}.c /usr/lib/x86_64-linux-gnu/libusb-1.0.so
	gcc -o ${FILE}.o ${FILE}.c -L. -lftd2xx -Wl,-rpath /usr/local/lib	
	./${FILE}.o 
kern:	
	rmmod ftdi_sio
gmit:
	git commit -q  user.username "mainahjeff" user.email "mahjefprojects@gmail.com"	
clean:	
	rm -f ${FILE}
	rm -f *.o *./${FILE}
	clear
