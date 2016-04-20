#include <iostream>
#include <fstream>

#include "sdf_loader.hpp"

int main(int argc, char** argv) {

    if(argc != 2) {
        std::cout << "Usage: " << argv[0] << "sdf_file" << "\n";
        return 1;
    }

    std::ifstream is(argv[1]);

    sdfparse::Loader sdf_loader;
    sdf_loader.load(is);

    return 0;
}
