
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>


// separar os tokens da string s supondo virgulas como delimitadores
std::vector <std::string> tokenize(std::string s) {
	std::vector <std::string> v;

	std::string a = "";
	for (int i = 0; i < (int)s.size(); i++) {
		if (s[i] == ',') {
			if (a != "") {
				v.push_back(a);
			}
			a = "";
		} else {
			a += s[i];
		}
	}
	if (a != "") {
		v.push_back(a);
	}
	//printf("here here size =  %d\n", (int)v.size());
	return v;
}

// retorna 1 caso as strings a e b sejam iguais, retorna 0 caso contrario
int match(std::string a, std::string b) {
	if ((int)a.size() != (int)b.size()) {
		return 0;
	}
	for (int i = 0; i < (int)a.size(); i++) {
		if (a[i] != b[i]) {
			return 0;
		}
	}
	return 1;
}

// retorna o primeiro inteiro no arquivo descrito pela string a
int get_int(std::string a) {	
	std::ifstream arquivo;
	int res;
	arquivo.open(a);
	arquivo >> res;
	arquivo.close();
	return res;
}


// retorna a string delimitada por "<tag>" no arquivo arq
std::string get_val(std::string arq, std::string tag) {
	std::ifstream arquivo;
	char c;

	arquivo.open(arq);
	while(arquivo.get(c)) {
		if (c == '<') {
			std::string aux = "";
			arquivo.get(c);
			while(c != '>') {
				aux += c;
				arquivo.get(c);
			}
			//std::cout << "here aux = " << aux << std::endl;
			if (aux == tag) {
				arquivo.get(c);
				std::string res = "";
				while(c != '<') {
					res += c;
					arquivo.get(c);
				}
				//std::cout << "here res = " << res << std::endl;
				return res;
			}
		}
	}
	//printf("not found\n");
	return ""; // nao achou
}

// resto da divisao do numero representado pela string s por mod
int get_rem(std::string s, int mod) {
	int res = 0;
	for (int i = 0; i < (int)s.size(); i++) {
		int idx;
		if (isalpha(s[i])) {
			idx = s[i] - 'A' + 10;
		} else {
			idx = s[i] - '0';
		}
		res = (res * 16 + idx) % mod;
	}
	return res;
}
	
/* escolhe o numero necessario de locais de votacao. Os locais sao escolhidos atraves de um funcao aleatoria
	provida pelo NIST - NIST Randomness Beacon
	documentacao - https://www.nist.gov/programs-projects/nist-randomness-beacon
*/
void gerar_amostra(std::vector <std::vector <std::string> > locais_votacao, int necessario, std::string cidade) {
	std::vector <std::vector <std::string> > amostra;
	std::ifstream arquivo; // arquivo com numero aleatorio gerado
	std::ofstream query_nist; // arquivos com queries ao nist
	std::set <int> used;
	int timestamp = 1378606800; // TODO mudar timestamp para cada amostra

	system("rm query_nist.txt"); // deletar arquivo caso exista para usar novo
	query_nist.open("query_nist.txt");
	// iniciar numeros aleatorios
	std::string cmd = "curl -H \"Content-Type: application/xml\" -X GET https://beacon.nist.gov/rest/record/next/";
	cmd += std::to_string(timestamp);
	query_nist << cmd; // append query ao nist no arquivo
	cmd += " > temp.txt";
	system(cmd.c_str());

	// pegar valores 
	std::string output = get_val("temp.txt", "outputValue");
	timestamp = stoi(get_val("temp.txt", "timeStamp"));

	int idx = get_rem(output, (int)locais_votacao.size());
	used.insert(idx);
	for (int i = 1; i < necessario; i++) {
		printf("got = %d\n", i);
		//usar funcao aleatoria do NIST chamando-a atraves da linha de comando
		cmd = "curl -H \"Content-Type: application/xml\" -X GET https://beacon.nist.gov/rest/record/next/";
		cmd += std::to_string(timestamp);
		query_nist << '\n';
		query_nist << cmd; // append query ao nist no arquivo
		cmd += " > temp.txt";
		system(cmd.c_str());

		// pegar valores
		output = get_val("temp.txt", "outputValue");
		timestamp = stoi(get_val("temp.txt", "timeStamp"));	
		idx = get_rem(output, (int)locais_votacao.size());
		if (used.find(idx) == used.end()) {
			used.insert(idx);
		} else {
			i--; // pegar outra urna, essa ja foi sorteada
		}
	}
	query_nist.close();


	for (std::set<int>::iterator it = used.begin(); it != used.end(); it++) {
		int idx = (*it);
		amostra.push_back(locais_votacao[idx]);
	}

	// colocar a amostra num arquivo csv
	std::ofstream arquivo_amostra;
	system("rm arquivo_amostra.csv"); // deletar arquivo caso exista para usar novo
	arquivo_amostra.open("arquivo_amostra.csv", std::ios::app);
	
	//printf("size amostra = %d\n", (int)amostra.size());
	for (int i = 0; i < (int)amostra.size(); i++) {
		std::string s = "";
		for (int j = 0; j < (int)amostra[i].size(); j++) {
			if (j != 0) {
				s += ',';
			}
			s += amostra[i][j];
		}
		if (i != 0) {
			arquivo_amostra << '\n';
		}
		//std::cout << s << std::endl;
		arquivo_amostra << s;
	}
	arquivo_amostra.close();

	return;
}
	
int main(int argc, char * argv[]) {
	if (argc == 1) {
		printf("Deve ser passado um parametro, nome da cidade onde sera realizada a amostra\n");
		return 1;
	}

	std::string cidade = argv[1]; // cidade onde sera realizada a amostra
	//std::cout << cidade << std::endl;
	std::vector <std::vector <std::string> > lista_locais; // lista dos locais de votacao em tokens
	std::ifstream locais_votacao; // arquivo com os locais de votacao
	int populacao; // quantidade de urnas na cidade
	std::string s;
	
	locais_votacao.open("locais_votacao.csv");
	getline(locais_votacao, s); // pular primeira linha, cabecalho csv
	int line = 0;
	while(getline(locais_votacao, s)) {
		line++;
		//printf("line = %d\n", line);
		std::vector<std::string> tokens = tokenize(s);
		/*if (line >= 310272 && line <= 310274) {
			for (int i = 0; i < (int)tokens.size(); i++) {
				std::cout << tokens[i] << ',';
			}
			std::cout << std::endl;
		}
		if (line > 310274) {
			break;
		} */
		if ((int)tokens.size() >= 5 && match(cidade, tokens[4])) { // o campo 4 eh o campo cidade
			lista_locais.push_back(tokens);
		}
	}
	populacao = (int)lista_locais.size();
	//printf("populacao = %d\n", populacao);
	
	/* chamar programa R para calcular com 0.99 de confianca o numero de urnas necessarias na amostra
		escrever resultado no arquivo temp.txt */
	std::string cmd = "Rscript amostra.r "; 
	cmd += std::to_string(populacao);
	cmd += " 0.99 > temp.txt";
	system(cmd.c_str()); // chamar comando pelo terminal TODO descomentar no IC onde tem R

	int necessario = get_int("temp.txt");
	system("rm temp.txt");
	printf("necessario = %d, pop = %d\n", necessario, populacao);
	
	gerar_amostra(lista_locais, necessario, cidade);
	
	return 0;
};
