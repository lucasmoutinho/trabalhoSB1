// TRABALHO 1 DE SOFTWARE BÁSICO - UNB - 2/2018
// PROFESSOR - BRUNO
// ALUNOS:
// - FREDERICO PINHEIRO DIB - 15/0125925
// - LUCAS DA SILVA MOUTINHO - 15/0015747

#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

vector<string> inputname;
int num_arquivos;

bool fexists(const string &filename){
  ifstream ifile(filename.c_str());
  return (bool) ifile;
}

int main(int argc, char *argv[]){
  int i;
  bool correct_execution = true;

  if(argc < 2){
    cout << "ERROR: Número de argumentos não pode ser vazio. Por favor insira o nome de um a quatro arquivos de entrada na linha de comando" << endl;
  }
  else if(argc > 5){
    cout << "ERROR: Número de argumentos não pode ultrapassar o limite de 4 nomes de arquivo" << endl;
  }
  else{
    num_arquivos = argc-1;
    for(i = 0; i < num_arquivos; i++){
      inputname.push_back(argv[i+1]);
      if(!(fexists((inputname[i] + ".obj").c_str()))){
        correct_execution = false;
        cout << "ERROR: O Arquivo " << argv[i+1] << ".obj não existe" << endl;
      }
    }
    if(correct_execution){
      cout << "Arquivo ligado corretamente" << endl;
    }
  }


  return 0;
}