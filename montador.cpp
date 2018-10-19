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

vector<string> initialize_instructions_vector(){
  vector<string> instructions;

  instructions.push_back("ADD");
  instructions.push_back("2");

  instructions.push_back("SUB");
  instructions.push_back("2");
  
  instructions.push_back("MULT");
  instructions.push_back("2");
  
  instructions.push_back("DIV");
  instructions.push_back("2");
  
  instructions.push_back("JMP");
  instructions.push_back("2");
  
  instructions.push_back("JMPN");
  instructions.push_back("2");
  
  instructions.push_back("JMPP");
  instructions.push_back("2");
  
  instructions.push_back("JMPZ");
  instructions.push_back("2");
  
  instructions.push_back("COPY");
  instructions.push_back("3");
  
  instructions.push_back("LOAD");
  instructions.push_back("2");
  
  instructions.push_back("STORE");
  instructions.push_back("2");
  
  instructions.push_back("INPUT");
  instructions.push_back("2");
  
  instructions.push_back("OUTPUT");
  instructions.push_back("2");
  
  instructions.push_back("STOP");
  instructions.push_back("1");

  return instructions;
}

vector<string> initialize_directives_vector(){
  vector<string> directives;

  directives.push_back("SECTION");
  directives.push_back("0");

  directives.push_back("SPACE");
  directives.push_back("1");

  directives.push_back("CONST");
  directives.push_back("1");

  directives.push_back("PUBLIC");
  directives.push_back("0");

  directives.push_back("EQU");
  directives.push_back("0");

  directives.push_back("IF");
  directives.push_back("0");

  directives.push_back("EXTERN");
  directives.push_back("0");

  directives.push_back("BEGIN");
  directives.push_back("0");

  directives.push_back("END");
  directives.push_back("0");


  return directives;
}


/* Retorna o tamanho da instrução caso seja uma instrução. 
Retorna -1 caso não seja uma instrução */
int directive_length(string word){
  int length = -1;
  unsigned int i, directives_size;
  vector<string> directives = initialize_directives_vector();

  directives_size = (unsigned int)directives.size();
  for(i = 0; i < directives_size; i=i+2){
    if(word == directives[i]){
      length = atoi(directives[i + 1].c_str());
      break;
    }
  }
  return length;
}

/* Retorna o tamanho da diretiva caso seja uma diretiva. 
Retorna -1 caso não seja uma diretiva */
int instruction_length(string word){
  int length = -1;
  unsigned int i, instructions_size;
  vector<string> instructions = initialize_instructions_vector();

  instructions_size = (unsigned int)instructions.size();
  for(i = 0; i < instructions_size; i=i+2){
    if(word == instructions[i]){
      length = atoi(instructions[i + 1].c_str());
      break;
    }
  }
  return length;
}

/*
Retorna um array de palavras, posições:
0 - rotulo
1 - operação
2 - operador 1
3 - operador 2
*/
vector<string> separate_instructions(string line, ifstream *inputfile, int *line_count) {
  vector<string> words, wordsAux;
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
        if ((wordsAux.size() == 0) && (aux.find(":") == string::npos)) {
          wordsAux.push_back("none:");
        }
        wordsAux.push_back(aux);
        aux = "";
        j=0;
      }

      j++;
      i++;
    }
    if (wordsAux.size() == 0) {
      break;
    }
    if ((wordsAux.size() != 1) && (wordsAux[0].find(":") != string::npos)) { /*Caso só tenha a label ele junta com a outra linha*/
      finish = 1;
    }
    else {
      getline(*inputfile, line);
      i=0;
      *line_count = *line_count + 1;
      size = wordsAux[0].length()-1;
      wordsAux[0].erase(size, size);
    }
  }

  if (wordsAux.size() >= 2) {
    words.push_back(wordsAux[0]);
    words.push_back(wordsAux[1]);
  }
  for (i=2;i<wordsAux.size();i++) {
    aux = wordsAux[i];
    size = wordsAux[i].length()-1;
    while ((wordsAux[i][size] != ',') && (i != wordsAux.size()-1)) {
      aux = aux + " " + wordsAux[i+1];
      i++;
      size = wordsAux[i].length()-1;
    }
    words.push_back(aux);
  }

  for (i=0;i<words.size();i++) { /*retira virgulas e dois pontos*/
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
    words = separate_instructions(line, &inputfile, &line_count);
    if (words.size() > 0) {

      for (i=0;i<words.size();i++) {
        cout << i << ": " << words[i] << endl;
      }
      cout << endl;

    }
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