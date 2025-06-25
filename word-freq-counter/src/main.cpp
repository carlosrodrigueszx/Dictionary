#include <iostream>
#include <fstream>
#include <string>
#include "../include/AVL.hpp"
#include "../include/RedBlackTree.hpp"
#include "../include/TextProcessor.hpp"
#include "../include/Utils.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    // @declarando o timer
    Timer t;

    // @nomeando argumentos
    if (argc < 4) {
        cerr << "Uso: " << argv[0] << " <dictionary_avl|dictionary_rb> <entrada.txt> <saida.txt>\n";
        return 1;
    }

    string dictType = argv[1];
    string inputFile = argv[2];
    string outputFile = argv[3];

    try {
        // @iniciando processamento de strings...
        auto words = readAndProcessText(inputFile);

        ofstream out(outputFile);

        // @estabelecendo o tipo de dicionário...
        if (!out) {
            cerr << "Erro ao abrir arquivo de saída: " << outputFile << '\n';
            return 1;
        }

        out << "{Dicionário}\n";

            if (dictType == "dictionary_avl") 
            {
                AVL<string, int> avl;
                t.begin();
                for (const auto& word : words) {
                    cout << "inserindo " << word << "..." << endl;
                    avl.insert(word);
                }
                cout << "criação e inserção bem-sucedidas." << endl;
                avl.print(out);

                if (avl.contains("cansado"))
                {
                    cout << "Tem 'cansado' no dicionário. " << endl; //
                    cout << "Quantidade de ocorrências: " << avl.get("cansado") << endl; //
                    cout << "Tentando updatar " << endl; //
                    avl.update("cansado", 2); //
                    cout << "Nova qntd. de ocorrências: " << avl.get("cansado") << endl; //
                    avl.remove("cansado"); //
                    cout << "Ainda tem 'cansado'? " << avl.contains("cansado") << endl;
                    cout << "Ainda tem 'cansado'? " << avl.contains("cansado") << endl;
                    cout << "Veja o que ainda tem no dicionário: " << endl;
                    avl.forEach([](const std::string& key, const int& value){
                        cout << key << "; " << value << endl;
                    });
                }
                
            }
         
            else if (dictType == "dictionary_rb") 
            {
                RedBlackTree<string, int> rb;
                t.begin();

                for(const auto& word : words){
                    rb.insert(word);
                }
            }
            
        else 
        {
            cerr << "Tipo de dicionário inválido: " << dictType << '\n';
            return 1;
        }

        cout << "Resultados gravados em '" << outputFile << "' com sucesso.\n";

    } 
    catch (const exception& e) 
    {
        cerr << "Erro: " << e.what() << '\n';
        return 1;
    }
    
    t.stop();

    cout << "Duração da execução: " << t.durationMs() << endl;
    
    return 0;
}
