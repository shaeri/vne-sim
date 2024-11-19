#BRITE Makefile.  April 2002

all:	c++ exe
 
clean:  c++clean

c++:
	@if test -f C++/Makefile; then\
	 (cd C++; make) ; \
	 (make gui);\
	fi

c++clean:
	@if test -f C++/Makefile; then\
	 (cd C++; make clean); \
	fi

exe:
	@echo "#!/bin/sh" > brite
	@echo "" >> brite
	@echo "java -Xmx256M -classpath Java/:. GUI.Brite" >> brite
	@chmod +x brite
