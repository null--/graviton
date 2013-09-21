#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <core/memory.hpp>
#include <utils/file.hpp>
#include <utils/directory.hpp>

using namespace GraVitoN::Utils;

int main()
{
    Directory home = Directory::cwd().parent().parent().parent(), mdir("test-dir");
    cout << "Path: " << home.getPath() << endl;
    
    std::vector<File> files;
    
    home.findFiles(files);

    for(std::vector<File>::iterator fi = files.begin(); fi != files.end(); ++fi)
    {
        cout << fi->getPath() << endl;
    }

    mdir.create();
    File myfile(mdir.getPath() + "/test.txt");

    cout << "EX: " << myfile.exists() << endl;
    
    myfile.create();

    cout << "EX: " << myfile.exists() << endl;

    mdir.createSubDirectory("test2");
 
    mdir.remove();

    return 0;
}
