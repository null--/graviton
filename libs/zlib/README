build (mingw):
	find . -name \*.c -exec i686-w64-mingw32-gcc -I . -c {} \;
	rm untgz.o miniunz.o
	rm pufftest.o
	rm pufftest.o
	rm minizip.o 
	i686-w64-mingw32-ar rcs libzlib.a *.o
	# nm | grep main must return nothing:
	i686-w64-mingw32-nm -a libzlib.a | grep -in main 
