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
  int opcode;
  unsigned int number_operands;
};

struct directive_length{
  string directive;
  int length;
  unsigned int number_operands;
};

struct section_length{
  string section;
  int position;
};

struct const_struct {
  string label;
  int position_count;
  int value;
};

vector<symbols> symbols_table; /*Tabela de simbolos*/
vector<instruction_length> instructions_table; /*Tabela de instruções*/
vector<directive_length> directives_table; /*Tabela de diretivas*/
vector<section_length> section_table; /*Tabela das sessões*/
vector<const_struct> const_table; /*Tabela de constantes*/
vector<int> relative_vec; /*Vetor de relativos*/
vector<int> code_vec; /*Vetor de codigo*/
vector<string> pre_vec; /*vetor de instruções pre processadas*/
bool error = false; /*Variavel que indica se aconteceu algum erro*/
bool is_module=false;
string inputname;

/*Cria a tabela de instruções*/
void initialize_instructions_table(){
  instruction_length new_instruction;

  new_instruction.instruction = "ADD";
  new_instruction.length = 2;
  new_instruction.opcode = 1;
  new_instruction.number_operands = 1;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "SUB";
  new_instruction.length = 2;
  new_instruction.opcode = 2;
  new_instruction.number_operands = 1;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "MULT";
  new_instruction.length = 2;
  new_instruction.opcode = 3;
  new_instruction.number_operands = 1;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "DIV";
  new_instruction.length = 2;
  new_instruction.opcode = 4;
  new_instruction.number_operands = 1;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "JMP";
  new_instruction.length = 2;
  new_instruction.opcode = 5;
  new_instruction.number_operands = 1;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "JMPN";
  new_instruction.length = 2;
  new_instruction.opcode = 6;
  new_instruction.number_operands = 1;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "JMPP";
  new_instruction.length = 2;
  new_instruction.opcode = 7;
  new_instruction.number_operands = 1;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "JMPZ";
  new_instruction.length = 2;
  new_instruction.opcode = 8;
  new_instruction.number_operands = 1;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "COPY";
  new_instruction.length = 3;
  new_instruction.opcode = 9;
  new_instruction.number_operands = 2;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "LOAD";
  new_instruction.length = 2;
  new_instruction.opcode = 10;
  new_instruction.number_operands = 1;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "STORE";
  new_instruction.length = 2;
  new_instruction.opcode = 11;
  new_instruction.number_operands = 1;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "INPUT";
  new_instruction.length = 2;
  new_instruction.opcode = 12;
  new_instruction.number_operands = 1;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "OUTPUT";
  new_instruction.length = 2;
  new_instruction.opcode = 13;
  new_instruction.number_operands = 1;
  instructions_table.push_back(new_instruction);
 
  new_instruction.instruction = "STOP";
  new_instruction.length = 1;
  new_instruction.opcode = 14;
  new_instruction.number_operands = 0;
  instructions_table.push_back(new_instruction);

}

/*Cria a tabela de diretivas*/
void initialize_directives_table(){
  directive_length new_directive;

  new_directive.directive = "SECTION";
  new_directive.length = 0;
  new_directive.number_operands = 1;
  directives_table.push_back(new_directive);
 
  new_directive.directive = "SPACE";
  new_directive.length = 1;
  new_directive.number_operands = 1;
  directives_table.push_back(new_directive);
 
  new_directive.directive = "CONST";
  new_directive.length = 1;
  new_directive.number_operands = 1;
  directives_table.push_back(new_directive);
 
  new_directive.directive = "PUBLIC";
  new_directive.length = 0;
  new_directive.number_operands = 0;
  directives_table.push_back(new_directive);
 
  new_directive.directive = "EQU";
  new_directive.length = 0;
  new_directive.number_operands = 1;
  directives_table.push_back(new_directive);
 
  new_directive.directive = "IF";
  new_directive.length = 0;
  new_directive.number_operands = 1;
  directives_table.push_back(new_directive);
 
  new_directive.directive = "EXTERN";
  new_directive.length = 0;
  new_directive.number_operands = 0;
  directives_table.push_back(new_directive);
 
  new_directive.directive = "BEGIN";
  new_directive.length = 0;
  new_directive.number_operands = 0;
  directives_table.push_back(new_directive);
 
  new_directive.directive = "END";
  new_directive.length = 0;
  new_directive.number_operands = 0;
  directives_table.push_back(new_directive);

}

/* 
Retorna o tamanho da diretiva caso seja uma diretiva. 
Retorna -1 caso não seja uma diretiva 
*/
int get_directive_length(string word, unsigned int number_operands, int line_count, string space_length = ""){
  int length = -1;
  unsigned int i, directives_size;

  directives_size = (unsigned int)directives_table.size();
  for(i = 0; i < directives_size; i++){
    if(word == directives_table[i].directive){
      if(word == "SPACE"){
        if (!(space_length.empty()) && number_operands == directives_table[i].number_operands){
          length = atoi(space_length.c_str());
        }
        else if (space_length.empty() && number_operands == 0){
          length = directives_table[i].length;
        }
        else{
          cout << "ERROR: Número de operandos inválidos para a diretiva " << word << " na linha:" << line_count << endl;
          error = true;
        }
      }
      else{
        if(number_operands != directives_table[i].number_operands){
          cout << "ERROR: Número de operandos inválidos para a diretiva " << word << " na linha:" << line_count << endl;
          error = true;
        }
        length = directives_table[i].length;
      }
      break;
    }
  }
  if (word == "BEGIN") {
    is_module = true;
  }
  return length;
}

/* Retorna o tamanho da instrução caso seja uma instrução. 
Retorna -1 caso não seja uma instrução */
int get_instruction_length(string word, unsigned int number_operands, int line_count){
  int length = -1;
  unsigned int i, instructions_size;

  instructions_size = (unsigned int)instructions_table.size();
  for(i = 0; i < instructions_size; i++){
    if(word == instructions_table[i].instruction){
      if(number_operands != instructions_table[i].number_operands){
        cout << "ERROR: Número de operandos inválidos para a instrução " << word << " na linha:" << line_count << endl;
        error = true;
      }
      length = instructions_table[i].length;
      break;
    }
  }
  return length;
}

/* 
Retorna o opcode da instrução caso seja uma instrução. 
Se o opcode não é encontrado, emite um erro 
*/
int get_instruction_opcode(string word){
  int opcode = -1;
  unsigned int i, instructions_size;

  instructions_size = (unsigned int)instructions_table.size();
  for(i = 0; i < instructions_size; i++){
    if(word == instructions_table[i].instruction){
      opcode = instructions_table[i].opcode;
      break;
    }
  }
  if(opcode == -1){
    cout << "ERROR: Não encontrado o Opcode da instrução " << word << endl;
  }
  return opcode;
}

/* 
Retorna o valor do símbolo, este que é o position_count.
Retorna -1 caso o símbolo não tenha sido encontrado.
*/
int get_symbol_value(string word){
  int position_count = -1;
  unsigned int i, ts_size;

  ts_size = (unsigned int)symbols_table.size();
  for(i = 0; i < ts_size; i++){
    if(word == symbols_table[i].label){
      position_count = symbols_table[i].position_count;
      break;
    }
  }
  return position_count;
}

/* 
Retorna o valor do const identificado na tabela pelo símbolo.
Retorna -1 caso o símbolo não tenha sido encontrado.
*/
int get_const_value(string word){
  int value = -1;
  unsigned int i, const_size;

  const_size = (unsigned int)const_table.size();
  for(i = 0; i < const_size; i++){
    if(word == const_table[i].label){
      value = const_table[i].value;
      break;
    }
  }
  return value;
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
  unsigned int i=0, j=0, finish=0, rotCount=0;
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
        if (aux[aux.length()-1] == '\0') {
          aux.erase(aux.length()-1, aux.length()-1);
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
    if (words[i][size] == ':') {
      rotCount++;
    }
    if ((words[i][size] == ':') || (words[i][size] == ',')) {
      words[i].erase(size, size);
    }
  }
  if (rotCount >=2) {
    cout << "ERROR: Dois rotulos para mesma instrução, linha: " << *line_count << endl;
    error = true;
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

void print_section_table() {
  unsigned int table_size = (unsigned int)section_table.size();
  unsigned int i;

  cout << "-------------" << endl;
  for(i = 0; i < table_size; i++){
    cout << section_table[i].section << endl;
    cout << section_table[i].position << endl << endl;
  }
  cout << "-------------" << endl;
}

void print_const_table() {
  unsigned int table_size = (unsigned int)const_table.size();
  unsigned int i;

  cout << "-------------" << endl;
  for(i = 0; i < table_size; i++){
    cout << const_table[i].label << endl;
    cout << const_table[i].position_count << endl;
    cout << const_table[i].value << endl << endl;
  }
  cout << "-------------" << endl;
}

// Imprime na tela o vetor de símbolos relativos
void print_relative() {
  unsigned int table_size = (unsigned int)relative_vec.size();
  unsigned int i;

  cout << "-------------" << endl;
  for(i = 0; i < table_size; i++){
    cout << relative_vec[i] << " ";
  }
  cout << endl;
  cout << "-------------" << endl;
}


// Imprime na tela o vetor code com os opcodes e os valores dos operandos
void print_code() {
  unsigned int code_size = (unsigned int)code_vec.size();
  unsigned int i;

  cout << "-------------" << endl;
  for(i = 0; i < code_size; i++){
    cout << code_vec[i] << " ";
  }
  cout << endl;
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
    return -1;
  }

  symbols new_symbol;
  new_symbol.label = label; 
  new_symbol.position_count = position_count;

  symbols_table.push_back(new_symbol); 

  return 0;
}

void insert_section_table(vector<string> words, int position_count, int line_count) {
  int flag=0;
  section_length new_section;

  if (words.size() == 3) {
    if (words[1] == "SECTION") {
      if (words[2] == "TEXT") {
        flag++;
        new_section.section = "TEXT";
        new_section.position = position_count;
      }
      if (words[2] == "DATA") {
        flag++;
        new_section.section = "DATA";
        new_section.position = position_count;
      }
      if (words[2] == "BSS") {
        flag++;
        new_section.section = "BSS";
        new_section.position = position_count;
      }
      if (flag == 0) {
        cout << "ERROR: SECTION não existente, linha: " << line_count << endl;
        error = true;
      }
      section_table.push_back(new_section);
    }
  }
}

void insert_const_table(vector<string> words, int position_count) {
  const_struct new_const;

  if (words.size() == 3) {
    if (words[1] == "CONST") {
      new_const.label = words[0];
      new_const.position_count = position_count;

      if (words[2].find("0X") != string::npos) {
        new_const.value = (int)strtol(words[2].c_str(), 0, 16);
      }
      else {
        new_const.value = atoi(words[2].c_str());
      }


      const_table.push_back(new_const);
    }
  }
}

void verify_text_section() {
  unsigned int table_size = (unsigned int)section_table.size();
  unsigned int i, flag=0;

  for(i = 0; i < table_size; i++){
    if (section_table[i].section == "TEXT") {
      flag++;
    }
  }

  if (flag == 0) {
    cout << "ERROR, SECTION TEXT não encontrada!" << endl;
    error = true;
  }
  if (section_table[0].section != "TEXT") {
    cout << "ERROR, SECTION TEXT deve ser a primeira!" << endl;
    error = true;
  }
}

/*Cria o vetor de relativos*/
void insert_relative(vector<string> words, int line_count) {
  int size_elem=words.size()-2, flag, i, num;
  flag = get_instruction_length(words[1], size_elem, line_count);
  if (flag != -1) {
    if (relative_vec.size() > 0) {
      num = relative_vec[relative_vec.size()-1];
      num++;
      for (i=0;i<size_elem;i++) {
        num++;
        relative_vec.push_back(num);
      }
    }
    else if (size_elem == 1) {
      relative_vec.push_back(1);
    }
    else if (size_elem == 2) {
      relative_vec.push_back(1);
      relative_vec.push_back(2);
    }
  }

}

/*Função de primeira passagem do montador*/
void first_passage() {
  ifstream inputfile;
  int line_count=1, position_count=0, error = 0, length = 0;
  unsigned int number_operands;
  string line, prefile="";
  vector<string> words;

  inputfile.open((inputname + ".asm").c_str()); /*Abre o arquivo*/

  while (getline(inputfile, line)) {
    transform(line.begin(), line.end(), line.begin(), ::toupper); /*Deixa toda a string maiuscula*/
    words = separate_instructions(line, &inputfile, &line_count);
    if (words.size() > 0) {

      if (words[0] != "none") {
        prefile = prefile + words[0] + ": ";
      }
      prefile = prefile + words[1] + " ";
      if (words.size() == 2) {
        prefile = prefile + "\n";
      }
      if (words.size() == 3) {
        prefile = prefile + words[2] + "\n";
      }
      if (words.size() == 4) {
        prefile = prefile + words[2] + ", " + words[3] + "\n";
      }
      pre_vec.push_back(prefile);
      prefile = "";

      insert_relative(words, line_count);

      if(words[0] != "none"){ //Rótulo existe
        error = insert_label_TS(words[0],position_count);
        if(error == -1){
          cout << "ERROR: Simbolo redefinido na linha: " << line_count << endl;
          error = true;
        }
      }
      number_operands = (unsigned int)words.size() - 2;
      length = get_instruction_length(words[1], number_operands, line_count);
      if(length != -1){
        position_count = position_count + length;
      }
      else{
        if(!(number_operands)){
          length = get_directive_length(words[1], number_operands, line_count);
        }
        else{
          length = get_directive_length(words[1], number_operands, line_count, words[2]);
        }
        if(length != -1){
          // executa diretiva
          insert_section_table(words, position_count, line_count);
          insert_const_table(words, position_count);
          position_count = position_count + length;
        }
        else{
          cout << "ERROR: Operação não identificada na linha: " << line_count << endl;
          error = true; 
        }
      }
      line_count++;

    }
    else {
      line_count++;
    }
  }
  verify_text_section();
}

/*Verifica os erros de operandos*/
void check_instruction_errors(vector<string> words, int position_count, int line_count) {
  string aux="";
  int  num=-1, space_count=0;
  unsigned int i;

  if ((words[1] == "JMP") || (words[1] == "JMPN") || (words[1] == "JMPP") || (words[1] == "JMPZ")) {
    for(i=0;i<symbols_table.size();i++) {
      if (words[2] == symbols_table[i].label) {
        num = symbols_table[i].position_count;
      }
    }
    if (num == -1) {
      cout << "ERROR: Simbolo de salto não encontrado! Linha: " << line_count << endl;
      error = true;
    }
    if ((num < section_table[0].position) || (num > section_table[1].position-1)) {
      cout << "ERROR: Label de salto fora da sessão TEXT! Linha: " << line_count << endl;
      error = true;
    }
  }

  if ((words[1] == "ADD") || (words[1] == "SUB") || (words[1] == "MULT") || (words[1] == "DIV") || (words[1] == "LOAD") || (words[1] == "STORE") || (words[1] == "INPUT") || (words[1] == "OUTPUT") || (words[1] == "COPY")) {
    if (words[2].find(" ") == string::npos) {
      aux = words[2];
    }
    else if (words[2].find(" + ") != string::npos) {
      i=0;
      while (words[2][i] != ' ') {
        aux = aux + words[2][i];
        i++;
      }
      for (i=0;i<words[2].length();i++) {
        if (words[2][i] == ' ') {
          space_count++;
        }
      }
      if (space_count > 2) {
        cout << "ERROR: Expreção não valida! Linha: " << line_count << endl;
        error = true;
      }
    }
    else {
      cout << "ERROR: Expreção não valida! Linha: " << line_count << endl;
      error = true;
    }
    for(i=0;i<symbols_table.size();i++) {
      if ((aux == symbols_table[i].label)) {
        num = symbols_table[i].position_count;
      }
    }
    if (num == -1) {
      cout << "ERROR: Simbolo não encontrado! Linha: " << line_count << endl;
      error = true;
    }
    if ((num > section_table[0].position) && (num < section_table[1].position) && (words[1] != "SECTION")) {
      cout << "ERROR: Operando na sessão TEXT! Linha: " << line_count << endl;
      error = true;
    }
  }

  if (words[1] == "COPY") {
    space_count=0;
    aux="";
    num=-1;
    if (words[3].find(" ") == string::npos) {
      aux = words[3];
    }
    else if (words[3].find(" + ") != string::npos) {
      i=0;
      while (words[3][i] != ' ') {
        aux = aux + words[2][i];
        i++;
      }
      for (i=0;i<words[3].length();i++) {
        if (words[3][i] == ' ') {
          space_count++;
        }
      }
      if (space_count > 2) {
        cout << "ERROR: Expreção não valida! Linha: " << line_count << endl;
        error = true;
      }
    }
    else {
      cout << "ERROR: Expreção não valida! Linha: " << line_count << endl;
      error = true;
    }
    for(i=0;i<symbols_table.size();i++) {
      if ((aux == symbols_table[i].label)) {
        num = symbols_table[i].position_count;
      }
    }
    if (num == -1) {
      cout << "ERROR: Simbolo não encontrado! Linha: " << line_count << endl;
      error = true;
    }
    if ((num > section_table[0].position) && (num < section_table[1].position) && (words[1] != "SECTION")) {
      cout << "ERROR: Operando na sessão TEXT! Linha: " << line_count << endl;
      error = true;
    }
  }

  if (words[1] == "DIV") {
    for(i=0;i<const_table.size();i++) {
      if ((words[2] == const_table[i].label) && (const_table[i].value == 0)) {
        cout << "ERROR: Divisão por 0! Linha: " << line_count << endl;
        error = true;
      }
    }
  }

  if ((words[1] == "STORE") || (words[1] == "INPUT")) {
    for(i=0;i<const_table.size();i++) {
      if (words[2] == const_table[i].label) {
        cout << "ERROR: STORE em CONST! Linha: " << line_count << endl;
        error = true;
      }
    }
  }

}

/*Verifica se alguma expreção foi colocada na sessão errada*/
void check_section_instruction_errors(vector<string> words, int position_count, int line_count) {
  int isntruction = get_instruction_length(words[1], words.size()-2, line_count);
  int diretive;
  unsigned int number_operands = (unsigned int)words.size() - 2;

  if (isntruction == -1) {
    if(!(number_operands)){
      diretive = get_directive_length(words[1], number_operands, line_count);
    }
    else{
      diretive = get_directive_length(words[1], number_operands, line_count, words[2]);
    }
  }

  if (isntruction != -1) {
    if ((position_count < section_table[0].position) || (position_count > section_table[1].position)) {
      cout << "ERROR: Instrução fora da sessão TEXT! Linha: " << line_count << endl;
      error = true;
    }
  }
  else if (diretive != -1) {
    if ((position_count > section_table[0].position) && (position_count <= section_table[1].position) && (words[1] != "SECTION")) {
      cout << "ERROR: Diretiva na sessão TEXT! Linha: " << line_count << endl;
      error = true;
    }
    if (section_table.size() == 3) {
      if (section_table[1].section == "DATA") {
        if (((position_count < section_table[1].position) || (position_count > section_table[2].position)) && (words[1] == "CONST")) {
          cout << "ERROR: CONST fora da sessão DATA! Linha: " << line_count << endl;
          error = true;
        }
        if ((position_count > section_table[1].position) && (position_count <= section_table[2].position) && (words[1] != "SECTION") && (words[1] == "SPACE")) {
          cout << "ERROR: SPACE fora da sessão BSS! Linha: " << line_count << endl;
          error = true;
        }
      }
      if (section_table[1].section == "BSS") {
        if (((position_count < section_table[1].position) || (position_count > section_table[2].position)) && (words[1] == "SPACE")) {
          cout << "ERROR: SPACE fora da sessão BSS! Linha: " << line_count << endl;
          error = true;
        }
         if ((position_count > section_table[1].position) && (position_count <= section_table[2].position) && (words[1] != "SECTION") && (words[1] == "CONST")) {
          cout << "ERROR: CONST fora da sessão DATA! Linha: " << line_count << endl;
          error = true;
        }
      }
    }
    else {
      if (section_table[1].section == "DATA") {
        if (words[1] == "SPACE") {
          cout << "ERROR: SPACE fora da sessão BSS! Linha: " << line_count << endl;
          error = true;
        }
      }
      if (section_table[1].section == "BSS") {
        if (words[1] == "CONST") {
          cout << "ERROR: CONST fora da sessão DATA! Linha: " << line_count << endl;
          error = true;
        }
      }
    }
  }
}

void separate_expression(string expression, string *aux1, string *aux2){
  int i = 0;
  while (expression[i] != ' '){
    *aux1 = *aux1 + expression[i];
    i++;
  }
  i = i + 3;
  while (i < (int)expression.size()){
    *aux2 = *aux2 + expression[i];
    i++;
  }
}

/*Insere os valores em memoria no vetor code_vec que representa o codigo de saida do montador*/
void insert_code_vec(vector<string> words, unsigned int number_operands, int line_count, bool instruction = true){
  int value;
  string label, second_label;

  if(instruction){
    code_vec.push_back(get_instruction_opcode(words[1]));
    if(number_operands == 1){
      label = words[2];
      if((label.find(" + ") != string::npos)){
        string aux1 = "", aux2 = "";
        separate_expression(label,&aux1,&aux2);
        value = get_symbol_value(aux1) + atoi(aux2.c_str());
      }
      else{
        value = get_symbol_value(label);
      }
      code_vec.push_back(value);
    }
    else if(number_operands == 2){
      label = words[2];
      second_label = words[3];
      if((label.find(" + ") != string::npos)){
        string aux1 = "", aux2 = "";
        separate_expression(label,&aux1,&aux2);
        value = get_symbol_value(aux1) + atoi(aux2.c_str());
      }
      else{
        value = get_symbol_value(label);
      }
      code_vec.push_back(value);
      if((second_label.find(" + ") != string::npos)){
        string aux3 = "", aux4 = "";
        separate_expression(second_label,&aux3,&aux4);
        value = get_symbol_value(aux3) + atoi(aux4.c_str());
      }
      else{
        value = get_symbol_value(second_label);
      }
      code_vec.push_back(value);
    }
    else if(number_operands != 0){
      cout << "ERROR: Número de operandos inálidos na linha: " << line_count << endl;
      error = true;
    }
  }
  else{
    label = words[1];
    if(label == "CONST"){
      value = get_const_value(words[0]);
      code_vec.push_back(value);
    }
    else if (label == "SPACE"){
      int i, n;
      if(number_operands == 0){
        n = 1;
      }
      else{
        n = atoi(words[2].c_str());
      }
      for(i = 0; i < n; i++){
        code_vec.push_back(0);
      }
    }
  }
}

/*Cria o arquivo .obj e .pre*/
void print_outputfile() {
  ofstream objfile, prefile;
  unsigned int i;

  objfile.open((inputname + ".obj").c_str()); /*Abre o arquivo obj*/
  prefile.open((inputname + ".pre").c_str()); /*Abre o arquivo pre*/

  if (is_module == true) {
    objfile << "RELATIVE" << endl;
    for (i=0;i<relative_vec.size();i++) {
      objfile << relative_vec[i] << " ";
    }
    objfile << endl;
    objfile << endl;

    objfile << "CODE" << endl;
  }
 
  for (i=0;i<code_vec.size();i++) {
    objfile << code_vec[i] << " ";
  }

  for (i=0;i<pre_vec.size();i++) {
    prefile << pre_vec[i];
  }
}

/*Segunda passagem*/
void second_passage() {
  ifstream inputfile;
  int line_count=1, position_count=0, length;
  string line, ofile;
  vector<string> words;
  unsigned int number_operands;


  inputfile.open((inputname + ".asm").c_str()); /*Abre o arquivo*/

  while (getline(inputfile, line)) {
    transform(line.begin(), line.end(), line.begin(), ::toupper); /*Deixa toda a string maiuscula*/
    words = separate_instructions(line, &inputfile, &line_count);
    if (words.size() > 0) {

      check_instruction_errors(words, position_count, line_count); /*Verifica os erros de operandos*/
      number_operands = (unsigned int)words.size() - 2;
      length = get_instruction_length(words[1], number_operands, line_count);
      if(length != -1){
        position_count = position_count + length;
        insert_code_vec(words, number_operands, line_count);
      }
      else{
        if(!(number_operands)){
          length = get_directive_length(words[1], number_operands, line_count);
        }
        else{
          length = get_directive_length(words[1], number_operands, line_count, words[2]);
        }
        if(length != -1){
          position_count = position_count + length;
          insert_code_vec(words, number_operands, line_count, false);
        }
        else{
          error = true; 
        }
      }
      check_section_instruction_errors(words, position_count, line_count); /*Verifica se alguma instrução está na sessão errada*/
      line_count++;
    }
    else {
      line_count++;
    }
  }
}

/*Verifica se o arquivo foi enviado corretamente*/
bool correcet_execution(int argc, string s) {
  s = s + ".asm";
  bool correct=true;
  if (argc != 2) {
    cout << "ERROR: Numero de argumentos errados! Apenas um arquivo de entrada é permitido por execução" << endl;
    correct = false;
  }
  if (s.length() <= 4) {
    cout << "ERROR: Arquivo de entrada com formato incorreto! Deve possuir extensão *.asm" << endl;
    correct = false;
  }
  else if (s.find(".asm") == string::npos) {
    cout << "ERROR: Arquivo de entrada com formato incorreto! Deve possuir extensão *.asm" << endl;
    correct = false;
  }
  return correct;
}

bool fexists(const string &filename){
  ifstream ifile(filename.c_str());
  return (bool) ifile;
}

int main(int argc, char *argv[]) {
  inputname = argv[1];
  if(argc != 1) {
    if (correcet_execution(argc, argv[1])) {
      if (fexists((inputname + ".asm").c_str())) {
        initialize_directives_table();
        initialize_instructions_table();
        first_passage();
        second_passage();

        if (error == false) {
          print_outputfile();
          cout << "Arquivo montado corretamente!" << endl;
        }
       
      }
      else{
        cout << "O Arquivo "<< argv[1] <<" não existe" << endl;
      }
    }
  }
  else{
    cout << "ERROR: Número de argumentos não pode ser vazio. Por favor insira o nome de um arquivo de entrada na linha de comando" << endl;
  }

  return 0;
}
