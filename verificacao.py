from collections import defaultdict
qr = open("QRconvertido.csv",'r');
e = open("bu.txt",'r'); #arquivo de bu's devem retirar aspas duplas de seus elementos
f = open("Erro.txt",'w+');
f.write('Erros no bu \n');

line = qr.readline();
line = qr.readline();
line2 = e.readline();


list_qr = list();

while(line != ''): #leitura ate EOF
    if line.startswith('AL'): # como os bu separam por regioes brasileiras, colocar a UF aqui
        list_qr.append(line.split(';'));
    line = qr.readline();

list_e = list();
dict_e = defaultdict(list);
while(line2 != ''): #leitura ate EOF
    aux = line2.split(';');
    dict_e[(aux[25])].append([aux[12],aux[25],aux[7],aux[8],aux[5],aux[21],aux[23]]); # para bu's como SP deve-se separar em hash's diferentes
    line2 = e.readline();


#QR list padrao UF;Cidade;Num municipio;Urna;Zona;Secao;Candidato;Votos;Brancos;Nulos

flag = 1;
for i in list_qr:
    for j in dict_e[(i[3])]:
        if( long(i[2]) == long(j[0]) ): #num municio
            if ( i[3] == j[1] ): #num da urna
                if ( int(i[4]) == int(j[2]) ): #zona
                    if( int(i[5]) == int(j[3]) ): #secao
                        if( int(i[6]) == int(j[4]) ): #cargo
                            if( int(i[7]) == int(j[5]) ): #num candidato
                                if( int(i[8]) == int(j[6]) ): #qnt votos do candidato
                                    flag = 0;
                                    
                                else:
                                    print 'erro votos em:' + str(i);
                                    f.write( 'Erro qnt de votos \n');
                                    f.write(str(i) + '\n') ;
                                    f.write(str(j) + '\n');

    if flag == 1:
        print 'erro em:' + str(i);
        f.write( 'Erro em: \n' );
        f.write(str(i) + '\n');
    flag = 1;

print 'Efetuado'

qr.close();
e.close();
f.close();
