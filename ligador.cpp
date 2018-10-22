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
vector<int> mod_global;

void print_file() {
	ofstream output;
	unsigned int i, j;

	output.open((inputname[0] + ".e").c_str());

	for (i=0;i<files.size();i++) {
		for (j=0;j<files[i].code.size();j++) {
			output << files[i].code[j] << " ";
		}
	}
}

void update_relative() {
	unsigned int i, j, index;
	for (i=0;i<files.size();i++) {
		for (j=0;j<files[i].relative.size();j++) {
			index = files[i].relative[j];
			files[i].code[index] = files[i].code[index] + mod_global[i];
		}
	}
}

void update_with_use_table() {
	int length, index;
	string aux;
	unsigned int i, j, h;
	for (i=0;i<files.size();i++) {
		for (j=0;j<files[i].table_use.size();j++) {
			aux = files[i].table_use[j].label;
			index = files[i].table_use[j].position;
			for (h=0;h<table_definition_global.size();h++) {
				if (table_definition_global[h].label == aux) {
					length = table_definition_global[h].position;
				}
			}
			files[i].code[index] = files[i].code[index] + length;
		}
	}
}

void print_global_table() {
	unsigned int i;
	cout << "-------------" << endl;
	for (i=0;i<table_definition_global.size();i++) {
		cout << table_definition_global[i].label << " " << table_definition_global[i].position << endl;
	}
	cout << "-------------" << endl;
}

void print_code() {
	unsigned int i, j;
	cout << "-------------" << endl;
	for (i=0;i<files.size();i++) {
		for (j=0;j<files[i].code.size();j++) {
			cout << files[i].code[j] << " ";
		}
		cout << endl;
	}
	cout << "-------------" << endl;
}

bool find_at_definition_global(string label){
  unsigned int i, global_size;
  bool found = false;

  global_size = (unsigned int)table_definition_global.size();

  for(i = 0; i<global_size; i++){
    if(table_definition_global[i].label == label){
      found = true;
      break;
    }
  }
  return found;
}

void verify_tables_use() {
  unsigned int i, j;
  unsigned int use_size;
  bool found;

  for(i = 0; i < files.size(); i++){
    use_size = (unsigned int)files[i].table_use.size();
    for(j = 0; j < use_size; j++){
      found = find_at_definition_global(files[i].table_use[j].label);
      if(!found){
        cout << "ERROR: Símbolo " << files[i].table_use[j].label << " da tabela de uso do arquivo " << files[i].name <<".obj não foi encontrado em nenhuma tabela de definição dos outros módulos. Processo de ligação foi interrompido" << endl;
        exit(0);
      }
    }
  }
}

void create_global_table_definition() {
	unsigned int i, j,mod=0;
	table_definition_struct table;

	for (i=0;i<files.size();i++) {
		mod_global.push_back(mod);
		for (j=0;j<files[i].table_definition.size();j++) {
			table.label = files[i].table_definition[j].label;
			table.position = files[i].table_definition[j].position + mod;
			table_definition_global.push_back(table);
		}
		mod = mod + files[i].code.size();
	}
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
  ifstream inputfile;
  int i;
  string line;
  bool error = false;

  for(i = 0; i < num_arquivos; i++){
    inputfile.open((inputname[i] + ".obj").c_str()); /*Abre o arquivo*/
    getline(inputfile, line);
    if(!(line.find("TABLE USE") != string::npos)){
      error = true;
      break;
    }
    inputfile.close();
  }
  if(error){
    cout << "ERROR: Arquivo " << inputname[i] <<".obj não é módulo. Ao passar mais de um arquivo como argumento, todos precisam ser módulos para a correta ligação" << endl;
    exit(0);
  }
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
			file.relative.push_back(atoi(aux.c_str()));
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
    create_global_table_definition();
    verify_tables_use();
    update_with_use_table();
    update_relative();
    print_file();

    print_global_table();
    print_code();
  }
	cout << "Arquivo ligado corretamente" << endl;

	return 0;
}