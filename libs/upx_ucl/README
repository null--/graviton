1. copy all object files from upx and ucl libraries into one folder
2. link'm all!

e.g.:
	# cd ucl/
	# make
	# cd src/
	# cp *.o ../../../../libs/upx/linux-x86_64/
	# cd ../../upx/
	# make all 
	# cd src/
	# cp *.o ../../../../libs/upx/linux-x86_64/
	# cd ../../../../libs/upx/linux-x86_64/
	# rm main.o
	# rc rcs libupxucl.a *.o

mingw-w64:
	build lib:
		cd ucl/
		export CC=i686-w64-mingw32-gcc && export CXX=i686-w64-mingw32-g++ && ./configure
		cp libucl.a ../upx/libupxucl.a
		cd ../upx
		find -name \*.cpp -exec i686-w64-mingw32-g++ -c -I ../zlib/ -I ../ucl/include/ {} \;
		rm main.o
		i686-w64-mingw32-ar rcs libupxucl.a *.o ../ucl/asm/i386/obj/win32/*.obj
		
	build upx:
		i686-w64-mingw32-g++ *.o -L../ucl/ -lucl -lwinmm -L../zlib/ -lzlib ../ucl/asm/i386/obj/win32/*.obj -static-libstdc++ -static-libgcc -o upx.exe
