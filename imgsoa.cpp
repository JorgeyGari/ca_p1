#include <iostream>
#include<cstring>
#include<filesystem>
#include "soa.cpp"
#include "progargs.cpp"

using namespace std;

int main(int argc, char *argv[]) {
    Datastruct data;
    argparsing(argc, argv, &data);
}
