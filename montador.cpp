// TRABALHO 1 DE SOFTWARE BÁSICO - UNB - 2/2018
// PROFESSOR - BRUNO
// ALUNOS:
// - FREDERICO PINHEIRO DIB - 15/0125925
// - LUCAS DA SILVA MOUTINHO - 15/0015747

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
using namespace std;

void first_passage() {
  cout << "FALA FRED" << endl;
  string s;
  getline(cin, s);
  transform(s.begin(), s.end(), s.begin(), ::toupper); /*Deixa toda a string maiuscula*/
  cout << s << endl;
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
  ifstream inputfile;
  if (correcet_execution(argc, argv[1])) {
    if (fexists(argv[1])) {
      inputfile.open(argv[1]);
      first_passage();
      second_passage();
    }
    else{
      cout << "O Arquivo "<< argv[1] <<" não existe" << endl;
    }
  }

  return 0;
}