#include <sodium.h>
#include <iostream>
#include <ctype.h>
#include <algorithm>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>

/*procura padrao pad em s e retorna a primeira posicao apos padrao */
int search(std::string &s, std::string pad);

struct qrcode {
	std::string UF; // uniao federativa
	std::string conteudo; 
	std::string hash;
	std::string assinatura;

	qrcode() {}
	qrcode(std::string UF, std::string conteudo, std::string hash, std::string assinatura) {
		this->UF = UF;
		this->conteudo = conteudo;
		this->hash = hash;	
		this->assinatura = assinatura;
	}

	/* inicializa dados do qrcode dado a string como toda a informacao do qrcode */
	void init(std::string &s) {
		// pegar UF
		UF = "";
		UF += tolower(s[0]);
		UF += tolower(s[1]);

		//std::cout << "starting here s = " << s << std::endl;
	
		// pegar hash
		int start_hash = search(s, "HASH:");
		hash = "";
		for (int i = start_hash; i < (int)s.size() && isalnum(s[i]); i++) {
			hash += s[i];
		}

		// pegar conteudo
		conteudo = "";
		int start_conteudo = search(s, "VRQR:1.4") + 1;
		for (int i = start_conteudo; i < (int)s.size() && i < start_hash - 6; i++) { // 6 eh o tamanho da string " HASH:"
			conteudo += s[i];
		}

		// pegar assinatura
		int start_assi = search(s, "ASSI:");
		for (int i = start_assi; i < (int)s.size() && isalnum(s[i]); i++) {
			assinatura += s[i];
		}	
		return;
	}	
};


/*procura padrao pad em s e retorna a primeira posicao apos padrao */
int search(std::string &s, std::string pad) {
	for (int i = 0; i < (int)s.size() - (int)pad.size(); i++) {
		int ok = 1;
		for (int j = 0; j < (int)pad.size(); j++) {
			if (s[i + j] != pad[j]) {
				ok = 0;
				break;
			}
		}
		if (ok) {
			return i + (int)pad.size();
		}
	}
	// nao achou
	return (int)s.size();
}

// Dada a string do BU inicial indica quantos BUs possuem mesma assinatura (BUs da mesma secao)
int get_num_qrcode(std::string s) {
	int start_num = search(s, "QRBU:");
	int q; // quantidade de BUs com mesma assinatura
	sscanf(s.c_str() + start_num, " %*d:%d", &q);
	return q;
}
	
int atohex(int c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
	}
	if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	}
	if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	}
	return -1;
}

std::vector<unsigned char> hex_to_bytes(const std::string &val) {
	if (val.empty() || not std::all_of(val.begin(), val.end(), ::isxdigit)) {
		if (val.empty()) {
			printf("ahdauhsduasduhasdh\n");
		}
		throw std::logic_error("Erro\n");
	}

	std::vector<unsigned char> bytes;
	for (int i = 0, n = (int)val.size(); i < n - 1; i += 2) {
		int x1 = atohex(val[i]);
		int x2 = atohex(val[i + 1]);

		if (x1 >= 0 && x2 >= 0) {
			bytes.push_back(x1 * 16 + x2);
		}
	}
	return bytes;
}

std::vector <unsigned char> recuperar_conteudo_chave(const std::string &chave_publica) {
	std::string path = "chavesqrcodeoficial/" + chave_publica; // caminho para a chave publica
	std::cout << path << std::endl;
	std::ifstream ifs(path.c_str(), std::ios::binary | std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();

	std::vector<char> resultado(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read(&resultado[0], pos);

	return std::vector<unsigned char>(resultado.begin(), resultado.end());
}

const std::vector<unsigned char> converte_string_hexadecimal_em_bytes(const std::string &hex_string) {
	return hex_to_bytes(hex_string);
}

const std::vector<unsigned char> recupera_conteudo_chave_publica(std::string UF) {
	const std::string chave_publica = "o" + UF + "qrcode.pub";
	return recuperar_conteudo_chave(chave_publica);
}

int verificar_assinatura(std::vector<unsigned char> &assinatura, std::vector<unsigned char> &dado_a_ser_validado,
				std::vector<unsigned char> &conteudo_chave_publica) {
	std::vector<unsigned char> assinatura_com_dado_assinado(assinatura.begin(), assinatura.end());
	assinatura_com_dado_assinado.insert(assinatura_com_dado_assinado.end(), dado_a_ser_validado.begin(), dado_a_ser_validado.end());

	unsigned long long tamanho_da_mensagem = dado_a_ser_validado.size();
	return crypto_sign_open(dado_a_ser_validado.data(), &tamanho_da_mensagem, assinatura_com_dado_assinado.data(), assinatura_com_dado_assinado.size(), conteudo_chave_publica.data()); 
}

void teste_valida_assinatura(struct qrcode &QR) {
	const std::string conteudoASerValidado = QR.conteudo;
	const std::string hashConteudoASerValidado = QR.hash;
	std::cout << "o" + QR.UF + "qrcode.pub" << std::endl;
	std::vector<unsigned char> conteudoDaChavePublica = recupera_conteudo_chave_publica(QR.UF);
	std::vector<unsigned char> assinatura = converte_string_hexadecimal_em_bytes(QR.assinatura);
	std::vector<unsigned char> dadoASerValidado = converte_string_hexadecimal_em_bytes(hashConteudoASerValidado);
	int resultadoDaVerificacao = verificar_assinatura(assinatura, dadoASerValidado, conteudoDaChavePublica);
	printf("resultado = %d\n", resultadoDaVerificacao);
	if (resultadoDaVerificacao == 0) {
		printf("Assinatura OK.\n");
	} else {
		printf("Erro na assinatura.\n");
		exit(1);
	}	
	return;
}
	

int main(void) {
	std::string s; // arquivo de chave publica
	std::string qrcode_info; // informacao do qr code
	std::vector <std::string> chaves_publicas; // nome dos arquivos com chaves publicas


	// criar arquivo com nomes das chaves
	/*system("ls ./chavesqrcodeoficial > temp.txt");
	std::ifstream lista_chaves_publicas;
	lista_chaves_publicas.open("temp.txt");
	while(getline(lista_chaves_publicas, s)) {
		chaves_publicas.push_back(s);
	}
	lista_chaves_publicas.close();
	system("rm temp.txt"); */

	// abrir CSV com informacao dos qrcodes
	std::ifstream lista_qrcodes;
	lista_qrcodes.open("QRCode.csv");
	getline(lista_qrcodes, qrcode_info); // pular primeira linha cabecalho CSV

	// pegar QRCodes
	while(getline(lista_qrcodes, qrcode_info)) {
		struct qrcode QR;

		QR.init(qrcode_info);
		// debug QR code
		/*printf("i = %d\n", i);
		std::cout << qrcode_info << std::endl; */
		/*printf("debug QR\n");
		std::cout << QR.UF << "++\n\n";
		std::cout << QR.hash << "++\n\n";
		std::cout << QR.assinatura << "++\n\n"; 
		std::cout << QR.conteudo << "++\n\n";
		printf("end debug\n"); */
		if (QR.assinatura != "") {
			teste_valida_assinatura(QR); 
		} else {
			printf("QR code sem assinatura\n");
		}
	}
	lista_qrcodes.close();
	return 0;
}
