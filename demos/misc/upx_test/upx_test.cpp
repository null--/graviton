#define GVN_ACTIVATE_LOGGER

#include <core/logger.hpp>
#include <graviton.hpp>
#include <malkit/generic/upx.hpp>

/// Compile and Build (mingw)
/// i686-w64-mingw32-g++ -I ../../../code/ -I ../../../code/external/upx/ -I ../../../code/external/ucl/include/ upx_test.cpp -c
/// i686-w64-mingw32-g++ upx_test.o -I ../../../code/ -I ../../../code/external/ucl/include/ -I ../../../code/external/upx/ -L ../../../libs/upx_ucl/win-x86-mingw/ -lupxucl -L ../../../libs/zlib/win-x86-mingw/ -lzlib -lgdi32 -static-libstdc++ -static-libgcc -o upx_test.exe

int main()
{
    try
    {	
		#ifdef INFO_OS_WINDOWS
			#define INPUT "upx_test.exe"
			#define OUTPUT "upx_test.upx.exe"
		#else
			#define INPUT "upx_test"
			#define OUTPUT "upxtest.upx"
		#endif
		
        GraVitoN::Malkit::UPX::run("-q "INPUT" -o "OUTPUT);
        cout << "/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\" <<endl;
        GraVitoN::Malkit::UPX::run("-q -t "OUTPUT);
        cout << "/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\" <<endl;
    }
    catch(...)
    {
        cout << "SHIT HAPPENED!" << endl;
    }

    return 0;
}
