// TRABALHO 1 DE SOFTWARE BÁSICO - UNB - 2/2018
// PROFESSOR - BRUNO
// ALUNOS:
// - FREDERICO PINHEIRO DIB - 15/0125925
// - LUCAS DA SILVA MOUTINHO - 15/0015747

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector> 
using namespace std;


/*
Retorna um array de palavras, posições:
0 - rotulo
1 - operação
2 - operador 1
3 - operador 2
*/
vector<string> separate_instructions(string line) {
  vector<string> words;
  words.push_back("");

  return words;
}

/*Função de primeira passagem do montador*/
void first_passage(char *argv[]) {
  ifstream inputfile;
  int line_count=1, instruction_count=0;
  string line, aux;

  inputfile.open(argv[1]); /*Abre o arquivo*/

  while (getline(inputfile, line)) {
    transform(line.begin(), line.end(), line.begin(), ::toupper); /*Deixa toda a string maiuscula*/
    separate_instructions(line);
  }
  
}

void second_passage() {
}

bool correcet_execution(int argc, string s) {
  bool correct=true;
  if (argc != 2) {
    cout << "Numero de argumentos errados!" << endl;
    correct = false;
  }
  if (s.length() <= 4) {
    cout << "Arquivo fora de formato!" << endl;
    correct = false;
  }
  else if (s.find(".asm") == string::npos) {
    cout << "Arquivo fora de formato!" << endl;
    correct = false;
  }
  return correct;
}

bool fexists(const string &filename){
  ifstream ifile(filename.c_str());
  return (bool) ifile;
}

int main(int argc, char *argv[]) {
  if (correcet_execution(argc, argv[1])) {
    if (fexists(argv[1])) {
      first_passage(argv);
      second_passage();
    }
    else{
      cout << "O Arquivo "<< argv[1] <<" não existe" << endl;
    }
  }

  return 0;
}