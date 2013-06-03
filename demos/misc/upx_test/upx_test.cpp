#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <malkit/generic/upx.hpp>

int main()
{
    try
    {
        GraVitoN::Malkit::UPX::run("-q ./upx_test -o ./upx_test.upx");
        cout << "/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\" <<endl;
        GraVitoN::Malkit::UPX::run("-q -t ./upx_test.upx");
        cout << "/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\" <<endl;
    }
    catch(...)
    {
        cout << "SHIT HAPPENED!" << endl;
    }

    return 0;
}
