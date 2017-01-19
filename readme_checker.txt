A checagem das assinaturas digitais e dos hashes dos BUs é feita usando os arquvios checker.cpp, sha512.cpp, sha512.h, init_checker.sh,
todos os arquivos da pasta "chavesqrcodesoficial", a lista dos QRs em csv mais a biblioteca libsodium (https://download.libsodium.org/doc/)

Usamos a implementação de Olivier Gay (http://www.zedwood.com/article/cpp-sha512-function) 
do sha-512

Para gerar o binario checker compilamos desse modo:
	g++ checker.cpp sha512.cpp -std=c++11 -lsodium -o checker

Cheque na documentação da biblioteca libsodium como fazer a ligação - precisamos incluir alguns valores no PATH.
Nos nossos computadores usamos:
	source init_checker

para incluir os valores necessários, mas pode ser que precise de outras constantes em outras máquinas.

O caminho para as pastas está hardcoded no código

Base com códigos QRs - usado na função main
chavesqrcodesoficial - usado nas funções recuperar_conteudo_chave, teste_valida_assinatura

Para rodar o programa
	./checker


