FILE=ftusb
URL=https://github.com/mainahjeff/ftdi2pic16-18_programmer.git
AC=gh repo clone mainahjeff/ftdi2pic16-18_programmer
REPO=ftdi2pic16-18_programmer
GITRM_FILE=*.txt *.o
make:
#	gcc -o ${FILE} ${FILE}.c /usr/local/lib/libftd2xx.so.1.4.27
#	gcc -o ${FILE}.o ${FILE}.c /usr/lib/x86_64-linux-gnu/libusb-1.0.so
	gcc -o ${FILE}.o ${FILE}.c -L. -lftd2xx -Wl,-rpath /usr/local/lib	
	./${FILE}.o 
kern:	
	rmmod ftdi_sio
gmit:
	
	git commit ${FILE}.c ${FILE}.h -m "test"
	# git push ${FILE}.c ${FILE}.h
grmv:	
	git rm -r ${GITRM_FILE}
clean:	
	rm -f ${FILE}
	rm -f *.o *./${FILE}
	clear
