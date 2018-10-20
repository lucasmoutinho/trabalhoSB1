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

struct symbols {
  string label;
  int position_count;
};

struct instruction_length{
  string instruction;
  int length;
};

struct directive_length{
  string directive;
  int length;
};

vector<symbols> symbols_table;
vector<instruction_length> instructions_table;
vector<directive_length> directives_table;

void initialize_instructions_table(){
  instruction_length new_instruction;

  new_instruction.instruction = "ADD";
  new_instruction.length = 2;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "SUB";
  new_instruction.length = 2;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "MULT";
  new_instruction.length = 2;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "DIV";
  new_instruction.length = 2;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "JMP";
  new_instruction.length = 2;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "JMPN";
  new_instruction.length = 2;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "JMPP";
  new_instruction.length = 2;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "JMPZ";
  new_instruction.length = 2;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "COPY";
  new_instruction.length = 3;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "LOAD";
  new_instruction.length = 2;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "STORE";
  new_instruction.length = 2;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "INPUT";
  new_instruction.length = 2;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "OUTPUT";
  new_instruction.length = 2;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "STOP";
  new_instruction.length = 1;
  instructions_table.push_back(new_instruction);

}

void initialize_directives_table(){
  directive_length new_directive;

  new_directive.directive = "SECTION";
  new_directive.length = 0;
  directives_table.push_back(new_directive);
 
  new_directive.directive = "SPACE";
  new_directive.length = 1;
  directives_table.push_back(new_directive);
 
  new_directive.directive = "CONST";
  new_directive.length = 1;
  directives_table.push_back(new_directive);
 
  new_directive.directive = "PUBLIC";
  new_directive.length = 0;
  directives_table.push_back(new_directive);
 
  new_directive.directive = "EQU";
  new_directive.length = 0;
  directives_table.push_back(new_directive);
 
  new_directive.directive = "IF";
  new_directive.length = 0;
  directives_table.push_back(new_directive);
 
  new_directive.directive = "EXTERN";
  new_directive.length = 0;
  directives_table.push_back(new_directive);
 
  new_directive.directive = "BEGIN";
  new_directive.length = 0;
  directives_table.push_back(new_directive);
 
  new_directive.directive = "END";
  new_directive.length = 0;
  directives_table.push_back(new_directive);

}

/* Retorna o tamanho da instrução caso seja uma instrução. 
Retorna -1 caso não seja uma instrução */
int get_directive_length(string word, string space_length){
  int length = -1;
  unsigned int i, directives_size;

  directives_size = (unsigned int)directives_table.size();
  for(i = 0; i < directives_size; i++){
    if(word == directives_table[i].directive){
      if(word == "SPACE" && !(space_length.empty())){
        length = atoi(space_length.c_str());
      }
      else{
        length = directives_table[i].length;
      }
      break;
    }
  }
  return length;
}

/* 
Retorna o tamanho da diretiva caso seja uma diretiva. 
Retorna -1 caso não seja uma diretiva 
*/
int get_instruction_length(string word){
  int length = -1;
  unsigned int i, instructions_size;

  instructions_size = (unsigned int)instructions_table.size();
  for(i = 0; i < instructions_size; i++){
    if(word == instructions_table[i].instruction){
      length = instructions_table[i].length;
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

      if ((line[i] == '\0') && (aux == "")) { /*Ignora comentarios*/
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
      j=0;
      *line_count = *line_count + 1;
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

/* Print Instructions Table no terminal */
void print_IT(){
  unsigned int table_size = (unsigned int)instructions_table.size();
  unsigned int i;

  cout << "-------------" << endl;
  for(i = 0; i < table_size; i++){
    cout << instructions_table[i].instruction << endl;
    cout << instructions_table[i].length << endl << endl;
  }
  cout << "-------------" << endl;
}

/* Print Directives Table no terminal */
void print_DT(){
  unsigned int table_size = (unsigned int)directives_table.size();
  unsigned int i;

  cout << "-------------" << endl;
  for(i = 0; i < table_size; i++){
    cout << directives_table[i].directive << endl;
    cout << directives_table[i].length << endl << endl;
  }
  cout << "-------------" << endl;
}


/* Print Symbols Table no terminal */
void print_TS(){
  unsigned int table_size = (unsigned int)symbols_table.size();
  unsigned int i;

  cout << "-------------" << endl;
  for(i = 0; i < table_size; i++){
    cout << symbols_table[i].label << endl;
    cout << symbols_table[i].position_count << endl << endl;
  }
  cout << "-------------" << endl;
}

// Procura rótulo na Tabela de Símbolos. Se achou emite um erro de símbolo redefinido e retorna -1
// Senão insere rótulo e position_count na TS e retorna 0
int insert_label_TS(string label, int position_count){
  unsigned int table_size = (unsigned int)symbols_table.size();
  unsigned int i = 0;
  bool found = false;

  while (!(found) && i < table_size){
    if(symbols_table[i].label == label){
      found = true;
    }
    else{
      i++;
    }
  }

  if(found){
    cout << "ERRO: Símbolo -" << symbols_table[i].label << "- está redefinido" << endl;
    return -1;
  }

  symbols new_symbol;
  new_symbol.label = label; 
  new_symbol.position_count = position_count;

  symbols_table.push_back(new_symbol); 

  return 0;
}

/*Função de primeira passagem do montador*/
void first_passage(char *argv[]) {
  ifstream inputfile;
  int line_count=1, position_count=0, error = 0, length = 0;
  string line;
  vector<string> words;

  inputfile.open(argv[1]); /*Abre o arquivo*/

  while (getline(inputfile, line)) {
    transform(line.begin(), line.end(), line.begin(), ::toupper); /*Deixa toda a string maiuscula*/
    words = separate_instructions(line, &inputfile, &line_count);
    if (words.size() > 0) {
      if(words[0] != "none"){ //Rótulo existe
        error = insert_label_TS(words[0],position_count);
        if(error == -1){
          break;
        }
      }
      length = get_instruction_length(words[1]);
      if(length != -1){
        position_count = position_count + length;
      }
      else{
        if(words.size() < 3){
          length = get_directive_length(words[1], "");
        }
        else{
          length = get_directive_length(words[1], words[2]);
        }
        if(length != -1){
          // executa diretiva
          position_count = position_count + length;
        }
        else{
          error = -1;
          cout << "ERROR: Operação não identificada na linha: " << line_count << endl;
          break; 
        }
      }
      line_count++;
    }
  }
  print_TS();
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
      initialize_directives_table();
      initialize_instructions_table();
      first_passage(argv);
      second_passage();
    }
    else{
      cout << "O Arquivo "<< argv[1] <<" não existe" << endl;
    }
  }

  return 0;
}
