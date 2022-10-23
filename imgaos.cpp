#include <iostream>
#include<cstring>
#include<filesystem>
#include "aos.cpp"
#include "progargs.cpp"

using namespace std;

int main(int argc, char *argv[]) {
    Datastruct data;
    argparsing(argc, argv, &data);
}
