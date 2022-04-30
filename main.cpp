#include <iostream>
#include <fstream>
#include <vector>

using std::cerr; using std::endl; 
using std::ifstream; 
using std::ofstream; 
using std::vector;

int main() {
    vector<char> bytes;
    char byte = 0;

    ifstream file;
    file.open("main.bf");
    if(!file.is_open()){
        cerr << "Could not open te file" << endl;
        return EXIT_FAILURE;
    }
    while(file.get(byte)){
        if(byte == ','|| byte == '.' || byte == '<' || byte == '>' || byte == '+' || byte == '-' || byte == '[' || byte == ']')
        bytes.push_back(byte);
    }
    file.close();

    ofstream program;
    program.open("bf2cpp.cpp");
    if(!program.is_open()){
        cerr << "Could not open te file" << endl;
        return EXIT_FAILURE;
    }
    program << "#include <iostream>" << endl;
    program << "using namespace std;" << endl;
    program << endl;
    program << "int main() {" << endl;
    program << "    char a[1024]; //Array de 1 kb donde cada elemento es un byte" << endl;
    program << "    for(int i = 0; i<1024; i++)" << endl;
    program << "        a[i] = 0;" << endl;
    program << "    int p = 0;" << endl;
    program << endl;

    int ident = 0;
    for( const auto &c : bytes ){
        //std::cout << c << ": " << ident << endl;
        program << '\t';
        switch(c){
            case '[':
                for(int i = 0; i < ident; i++) program << '\t';
                program << "while(a[p] != 0) {" << endl;
                ident++;
                break;
            case ']':
                ident--;
                for(int i = 0; i < ident; i++) program << '\t';
                program << "}" << endl;
                break;
            case ',':
                for(int i = 0; i < ident; i++) program << '\t';
                program << "cin >> a[p];" << endl;
                break;
            case '.':
                for(int i = 0; i < ident; i++) program << '\t';
                program << "cout << a[p];" << endl;
                break;
            case '+':
                for(int i = 0; i < ident; i++) program << '\t';
                program << "a[p]++;" << endl;
                break;
            case '-':
                for(int i = 0; i < ident; i++) program << '\t';
                program << "a[p]--;" << endl;
                break;
            case '<':
                for(int i = 0; i < ident; i++) program << '\t';
                program << "p--;" << endl;
                break;
            case '>':
                for(int i = 0; i < ident; i++) program << '\t';
                program << "p++;" << endl;
                break;
            default:
                break;
        }
    }

    program << "    return 0;" << endl;
    program << "}" << endl;
    program << endl;
    program.close();
    return 0;



}
