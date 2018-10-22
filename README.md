## TRABALHO 1 DA DISCIPLINA DE SOFTWARE BÁSICO - UNB - 2/2018
## PROFESSOR - BRUNO MACCHIAVELLO

### ALUNOS:
```
FREDERICO PINHEIRO DIB - 15/0125925
LUCAS DA SILVA MOUTINHO - 15/0015747
```

### PROBLEMA:
Implementar um montador e ligador em C++ para a linguagem Assembly hipotética vista em sala de aula.

### DESENVOLVIMENTO:
O programa foi feito em C++ 11, no ubuntu 16.04 LTS. Fora utilizado a seguinte versão do g++:

```
g++ --version
```

```
g++ (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609
Copyright (C) 2015 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

### COMO COMPILAR:
Para compilar ambos códigos fontes e gerar os executáveis "montador" e "ligador", utilize o seguinte comando:

```
make
```

Caso deseje apenas compilar o código fonte do montador.c e gerar o executável "montador", utilize o seguinte comando:

```
g++ -std=c++11 -ansi -Wall -o montador montador.cpp
```

ou

```
make compile_montador
```

Caso deseje apenas compilar o código fonte do ligador.c e gerar o executável "ligador", utilize o seguinte comando:

```
g++ -std=c++11 -ansi -Wall -o ligador ligador.cpp
```

ou

```
make compile_ligador
```

### COMO EXECUTAR:
Para executar o programa do montador, utilize o seguinte comando:

```
./montador arquivo
```

Onde "arquivo" deve ser o nome de um programa "arquivo.asm" presente no diretório local. Apenas um argumento é aceito por execução. Sendo assim, apenas um programa é montado por execução.

Ao executar o montador com um nome válido de arquivo, este irá realizar a montagem do mesmo; informando se houveram erros durante o processo. Caso erros tenham sido identificados, as mensagens informando o erro e a linha do "arquivo.asm" em que este ocorreu serão impressas no terminal. Caso nenhum erro tenha sido identificado, será criado um um "arquivo.obj", contendo os opcodes e operandos. Caso o "arquivo.asm" se caracterize por um módulo, a saida também conterá informações da TABELA DE USO, TABELA DE DEFINIÇÃO e RELATIVOS. Além do "arquivo.obj", será criado, também, a saída "arquivo.pre", sendo esta o programa pré-processado, ou seja, sem os comentários e seguindo um padrão de identações, espaçamentos, tabulações e quebras de linha.

Para executar o programa do ligador, utilize o seguinte comando:

```
./ligador arquivo1 arquivo2 arquivo3 arquivo4
```

Onde "arquivo1", "arquivo2", "arquivo3", "arquivo4" devem ser nomes de códigos objetos: "arquivo1.obj", "arquivo2.obj", "arquivo3.obj", "arquivo4.obj" presentes no diretório local. São necessários de 1 a 4 arquivos por linha de comando para realizar a ligação dos mesmos. 

Ao executar o ligador com nomes válidos de arquivos objetos, este irá realizar a ligação dos mesmos; informando se houveram erros durante o processo. Caso erros tenham sido identificados, as mensagens informando o erro serão impressas no terminal. Caso nenhum erro tenha sido identificado, será criado um um "arquivo.e" (come o nome do primeiro argumento passado), contendo os opcodes e operandos.