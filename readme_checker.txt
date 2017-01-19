A checagem das assinaturas digitais e dos hashes dos BUs � feita usando os arquvios checker.cpp, sha512.cpp, sha512.h, init_checker.sh,
todos os arquivos da pasta "chavesqrcodesoficial", a lista dos QRs em csv mais a biblioteca libsodium (https://download.libsodium.org/doc/)

Usamos a implementa��o de Olivier Gay (http://www.zedwood.com/article/cpp-sha512-function) 
do sha-512

Para gerar o binario checker compilamos desse modo:
	g++ checker.cpp sha512.cpp -std=c++11 -lsodium -o checker

Cheque na documenta��o da biblioteca libsodium como fazer a liga��o - precisamos incluir alguns valores no PATH.
Nos nossos computadores usamos:
	source init_checker

para incluir os valores necess�rios, mas pode ser que precise de outras constantes em outras m�quinas.

O caminho para as pastas est� hardcoded no c�digo

Base com c�digos QRs - usado na fun��o main
chavesqrcodesoficial - usado nas fun��es recuperar_conteudo_chave, teste_valida_assinatura

Para rodar o programa
	./checker


