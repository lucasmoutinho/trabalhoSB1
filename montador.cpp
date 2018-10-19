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
vector<string> separate_instructions(string line, ifstream *inputfile) {
  vector<string> words;
  string aux;
  unsigned int i=0, j=0, finish=0;
  int size;

  while (finish == 0) {
    while (i <= line.length()) { /*Le a linha toda*/
      while (((line[i] == ' ') || (line[i] == '\t')) && (i < line.length())) { /*Remove espaços vazios*/
        i++;
      }

      if (line[i] == ';') { /*Ignora comentarios*/
        break;
      }

      aux = aux + line[i];

      if ((line[i+1] == ' ') || (line[i+1] == '\t') || (line[i+1] == '\n') || (i == line.length())) {
        if ((words.size() == 0) && (aux.find(":") == string::npos)) {
          words.push_back("label:");
        }
        words.push_back(aux);
        aux = "";
        j=0;
      }

      j++;
      i++;
    }
    if (words.size() == 0) {
      break;
    }
    if ((words.size() != 1) && (words[0].find(":") != string::npos)) { /*Caso só tenha a label ele junta com a outra linha*/
      finish = 1;
    }
    else {
      getline(*inputfile, line);
      i=0;
    }
  }

  for (i=0;i<words.size();i++) {
    size = words[i].length()-1;
    if ((words[i][size] == ':') || (words[i][size] == ',')) {
      words[i].erase(size, size);
    }
  }
  return words;
}

/*Função de primeira passagem do montador*/
void first_passage(char *argv[]) {
  ifstream inputfile;
  int line_count=1, instruction_count=0;
  unsigned int i=0;
  string line;
  vector<string> words;

  inputfile.open(argv[1]); /*Abre o arquivo*/

  while (getline(inputfile, line)) {
    transform(line.begin(), line.end(), line.begin(), ::toupper); /*Deixa toda a string maiuscula*/
    words = separate_instructions(line, &inputfile);

    for (i=0;i<words.size();i++) {
      cout << words[i] << " ";
    }
    cout << endl;
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