#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using std::cout; using std::cerr; using std::endl; 
using std::ifstream; using std::ofstream; 
using std::vector;
using std::string;

int main(int argc, char* argv[]) {
    bool     decInput = false,      /* Entrada en formato decimal.           */
            decOutput = false,      /*  Salida en formato decimal.           */
            pointerOF = false,      /* OverFlow del puntero (Si al superar el
                                        MemorySize vuelve al 0).             */
         memoryViewer = false,      /* Permitir que se vea la memoria y las
                                        operaciones realizadas.              */
            debugMode = false;
    string bfFilename = "main.bf";  /* Nombre del archivo Brainfuck.         */
    string separator = " ";
    int    memorySize = 1024;       /* Tamano de la memoria.                 */

    bool expectingSize = false;
    bool nameFound = false;
    for (int i = 1; i < argc; i++){
        if(expectingSize){
            int p = 0;
            int res = 0;
            while(argv[i][p] != '\0'){
                res*=10;
                int digito = argv[i][p] - 48;
                if(digito < 0 || digito > 9){
                    cerr<<"Ingresa un valor para memorySize valido" << endl;
                    return 1;
                }
                res+=digito;
                p++;
            }
            memorySize = res;
            expectingSize = false;
        }else if (argv[i][0] == '-'){
            if(argv[i][1] == '-'){
                bool isMemoryFlag = true;
                char memoryFlag[] = "--memory";
                for(int j = 0; j < 8; j++){
                    if(argv[i][j] != memoryFlag[j]) isMemoryFlag = false;
                }
                if(isMemoryFlag) expectingSize = true;
            }else {
                int p = 1;
                while(argv[i][p] != '\0'){
                    switch (argv[i][p])
                    {
                    case 'i':
                        decInput = true;
                        break;
                    case 'o':
                        decOutput = true;
                        break;
                    case 'f':
                        pointerOF = true;
                        break;
                    case 'v':
                        memoryViewer = true;
                        break;
                    case 'd':
                        debugMode = true;
                    default:
                        break;
                    }
                    p++;
                }
            }
        } else {
            bfFilename = argv[i];
        }
    }
    // cout << "Entrada Decimal: " << decInput << endl;
    // cout << "Salida Decimal: " << decOutput << endl;
    // cout << "MemorySize: " << memorySize << endl;
    // cout << "Pointer overflow: " << pointerOF << endl;
    // cout << "Memory Viewer: " << memoryViewer << endl;
    // cout << "Nombre archivo: " << bfFilename << endl;

    vector<char> bytes;
    char byte = 0;

    ifstream file;
    file.open(bfFilename);
    if(!file.is_open()){
        cerr << "Could not open the file: '" << bfFilename << "'" << endl;
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
    output.open("bf2cpp.cpp");
    if(!output.is_open()){
        cerr << "Could not open the file" << endl;
        return EXIT_FAILURE;
    }
    output << "#include <iostream>" << endl;
    output << "using namespace std;" << endl;
    output << endl;
    output << "int main() {" << endl;
    output << "    char a["<<memorySize<<"]; //Array de 1 kb donde cada elemento es un byte" << endl;
    output << "    for(int i = 0; i<1024; i++)" << endl;
    output << "        a[i] = 0;" << endl;
    output << "    int p = 0;" << endl;
    output << endl;

    int ident = 0;
    int iter = 0;
    for( const auto &c : bytes ){
        //cout << c << ": " << ident << endl;
        if(debugMode) output << "\tcout << \"DEBUG: STEP "<<iter++<<": " << c << "\"<<endl;";
        output << '\t';
        switch(c){
            case '[':
                for(int i = 0; i < ident; i++) output << '\t';
                output << "while(a[p] != 0) {";
                output << endl;
                ident++;
                break;
            case ']':
                ident--;
                for(int i = 0; i < ident; i++) output << '\t';
                output << "}";
                output << endl;
                break;
            case ',':
                for(int i = 0; i < ident; i++) output << '\t';
                output << "cin >> a[p];";
                output << endl;
                break;
            case '.':
                for(int i = 0; i < ident; i++) output << '\t';
                if (decOutput){
                    output << "cout << (int) a[p] << \""<<separator<<"\";";
                } else {
                    output << "cout << a[p] << \""<<separator<<"\";";
                }
                output << endl;
                break;
            case '+':
                for(int i = 0; i < ident; i++) output << '\t';
                output << "a[p]++;";
                if(debugMode) output<< "cout <<\"DEBUG: VALUE: \"<< (int)a[p]<<endl; ";
                output << endl;
                break;
            case '-':
                for(int i = 0; i < ident; i++) output << '\t';
                output << "a[p]--;";
                if(debugMode) output<< "cout <<\"DEBUG: VALUE: \"<< (int)a[p]<<endl; ";
                output << endl;
                break;
            case '<':
                for(int i = 0; i < ident; i++) output << '\t';
                output << "p--;";
                if(debugMode) output<< "cout <<\"DEBUG: POINT: \"<< p<<endl; ";
                output << endl;
                break;
            case '>':
                for(int i = 0; i < ident; i++) output << '\t';
                output << "p++;";
                if(debugMode) output<< "cout <<\"DEBUG: POINT: \"<< p<<endl; ";
                output << endl;
                break;
            default:
                break;
        }
    }

    output << "    cout << endl;" << endl;
    output << "    return 0;" << endl;
    output << "}" << endl;
    output << endl;
    output.close();
    return 0;



}
