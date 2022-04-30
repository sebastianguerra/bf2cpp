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
        cerr << "Could not open the file" << endl;
        return EXIT_FAILURE;
    }
    while(file.get(byte)){
        if(
            byte == ',' || byte == '.' || 
            byte == '<' || byte == '>' || 
            byte == '+' || byte == '-' || 
            byte == '[' || byte == ']'
        )
            bytes.push_back(byte);
    }
    file.close();

    ofstream output;
    output.open("bf2cpp.cpp"); // TODO pasar el nombre por argumento
    if(!output.is_open()){
        cerr << "Could not open the file" << endl;
        return EXIT_FAILURE;
    }
    output << "#include <iostream>" << endl;
    output << "using namespace std;" << endl;
    output << endl;
    output << "int main() {" << endl;
    output << "    char a[1024]; //Array de 1 kb donde cada elemento es un byte" << endl;
    output << "    for(int i = 0; i<1024; i++)" << endl;
    output << "        a[i] = 0;" << endl;
    output << "    int p = 0;" << endl;
    output << endl;

    int ident = 0;
    for( const auto &c : bytes ){
        //std::cout << c << ": " << ident << endl;
        output << '\t';
        switch(c){
            case '[':
                for(int i = 0; i < ident; i++) output << '\t';
                output << "while(a[p] != 0) {" << endl;
                ident++;
                break;
            case ']':
                ident--;
                for(int i = 0; i < ident; i++) output << '\t';
                output << "}" << endl;
                break;
            case ',':
                for(int i = 0; i < ident; i++) output << '\t';
                output << "cin >> a[p];" << endl;
                break;
            case '.':
                for(int i = 0; i < ident; i++) output << '\t';
                output << "cout << a[p];" << endl;
                break;
            case '+':
                for(int i = 0; i < ident; i++) output << '\t';
                output << "a[p]++;" << endl;
                break;
            case '-':
                for(int i = 0; i < ident; i++) output << '\t';
                output << "a[p]--;" << endl;
                break;
            case '<':
                for(int i = 0; i < ident; i++) output << '\t';
                output << "p--;" << endl;
                break;
            case '>':
                for(int i = 0; i < ident; i++) output << '\t';
                output << "p++;" << endl;
                break;
            default:
                break;
        }
    }

    output << "    return 0;" << endl;
    output << "}" << endl;
    output << endl;
    output.close();
    return 0;



}
