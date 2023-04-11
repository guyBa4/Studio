#include "Studio.h"
#include <iostream>

using namespace std;

Studio* backup = nullptr;

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "usage: studio <config_path>" << std::endl;
        return 0;
    }
    string configurationFile = argv[1];
    Studio* studio = new Studio(configurationFile);
    studio->start();
    if (backup != nullptr) {
        delete backup;
        backup = nullptr;
    }
    delete studio;
    return 0;
}
