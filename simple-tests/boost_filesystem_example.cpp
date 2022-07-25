#include <iostream>
#include <boost/filesystem.hpp>

using namespace boost::filesystem;

// example from https://www.boost.org/doc/libs/1_53_0/libs/filesystem/doc/tutorial.html
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: tut1 path\n";
        return 1;
    }
    const char* path = argv[1];
    if (is_regular_file(path)) {
        std::cout << argv[1] << " " << file_size(argv[1]) << ", directory: " << is_directory(argv[1]) <<'\n';
    }
    else {
        std::cout << argv[1] << " is directory: " << is_directory(argv[1]) <<'\n';
    }
    return 0;
}