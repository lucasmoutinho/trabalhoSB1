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

struct table_use_struct {
	string label;
	int position;
};

struct table_definition_struct {
	string label;
	int position;
};

struct file_struct {
	string name;
	vector<table_use_struct> table_use;
	vector<table_definition_struct> table_definition;
	vector<int> relative;
	vector<int> code;
};

vector<file_struct> files;
vector<table_definition_struct> table_definition_global;


void create_global_table_use() {

}

void update_global_table_use() {
	
}

void verify_one_file() {
  ifstream inputfile;
  ofstream objfile;
  string line;

  inputfile.open((inputname[0] + ".obj").c_str()); /*Abre o arquivo*/

  getline(inputfile,line);
  if(line.find("TABLE USE") != string::npos){
    getline(inputfile, line);
    if (line.empty()){
      while(getline(inputfile, line)){
        if(line.find("CODE") != string::npos){
          getline(inputfile, line);
          objfile.open((inputname[0] + ".e").c_str()); /*Abre o arquivo executável*/
          objfile << line;
          break;
        }   
      }
    }
    else{
      cout << "ERROR: Apenas um módulo encontrado: (" << inputname[0] << ".obj). É necessário mais de um para a ligação correta" << endl;
      exit(0);
    }
  }
  else{
    objfile.open((inputname[0] + ".e").c_str()); /*Abre o arquivo executável*/
    objfile << line;
  }

  inputfile.close();
  objfile.close();
  cout << "Arquivo ligado corretamente" << endl;
  exit(0);
}

void verify_if_all_file_is_module() {

}

void parser() {
	unsigned int j;
	int i;
	string line, aux="";
	ifstream file_input;
	file_struct file;
	table_definition_struct table_definition;
	table_use_struct table_use;

	for (i=0;i<num_arquivos;i++) {
		file_input.open((inputname[i] + ".obj").c_str());

		file.name = "";
		file.table_definition.clear();
		file.table_use.clear();
		file.code.clear();
		file.relative.clear();

		file.name = inputname[i];

		getline(file_input, line);
		getline(file_input, line);
		/*Tabela de uso*/
		while (line != "\0") {
			aux="";
			j=0;
			while (line[j] != ' ') {
				aux = aux + line[j];
				j++;
			}
			table_use.label = aux;
			j++;
			aux="";
			while (j<line.length()) {
				aux = aux + line[j];
				j++;
			}
			table_use.position = atoi(aux.c_str());
			file.table_use.push_back(table_use);
			getline(file_input, line);
		}

		getline(file_input, line);
		getline(file_input, line);
		/*Tabela de definições*/
		while (line != "\0") {
			aux="";
			j=0;
			while (line[j] != ' ') {
				aux = aux + line[j];
				j++;
			}
			table_definition.label = aux;
			j++;
			aux="";
			while (j<line.length()) {
				aux = aux + line[j];
				j++;
			}
			table_definition.position = atoi(aux.c_str());
			file.table_definition.push_back(table_definition);
			getline(file_input, line);
		}

		getline(file_input, line);
		getline(file_input, line);
		/*Relativo*/
		j=0;
		while (j<line.length()) {
			aux="";
			while (line[j] != ' ') {
				aux = aux + line[j];
				j++;
			}
			j++;
			file.code.push_back(atoi(aux.c_str()));
		}


		getline(file_input, line);
		getline(file_input, line);
		/*Code*/
		getline(file_input, line);
		j=0;
		while (j<line.length()) {
			aux="";
			while (line[j] != ' ') {
				aux = aux + line[j];
				j++;
			}
			j++;
			file.code.push_back(atoi(aux.c_str()));
		}

		files.push_back(file);

		file_input.close();
	}
}

bool fexists(const string &filename){
	ifstream ifile(filename.c_str());
	return (bool) ifile;
}

void all_file_exist(int argc, char *argv[]) {
	int i;
	num_arquivos = argc-1;
	for(i = 0; i < num_arquivos; i++){
		inputname.push_back(argv[i+1]);
		if(!(fexists((inputname[i] + ".obj").c_str()))){
			cout << "ERROR: O Arquivo " << inputname[i] << ".obj não existe" << endl;
			exit(0);
		}
	}
}

int main(int argc, char *argv[]){
	if(argc < 2) {
		cout << "ERROR: Número de argumentos não pode ser vazio. Por favor insira o nome de um a quatro arquivos de entrada na linha de comando" << endl;
		exit(0);
	}
	if(argc > 5) {
		cout << "ERROR: Número de argumentos não pode ultrapassar o limite de 4 nomes de arquivo" << endl;
		exit(0);
	}
	all_file_exist(argc, argv);
  if(num_arquivos == 1){
	  verify_one_file();
  }
  else{
    verify_if_all_file_is_module();
    parser();
    create_global_table_use();
    update_global_table_use();
  }
	cout << "Arquivo ligado corretamente" << endl;

	return 0;
}