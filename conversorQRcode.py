def function(saida,line_split):
    #print 'Function QRCode completo: ' + str(line_split)

    DATA_ARQ = "04/10/2016";
    HORA_GER_ARQ = "20:48:13";
    FLAG = 0;
    LISTA = list();
    for i in line_split:

        if i.startswith('QRBU:') : QRBU = i.replace('QRBU:','');
        elif i.startswith('VRQR:') : VRQR = i.replace('VRQR:','');
        elif i.startswith('PROC:') : PROC = i.replace('PROC:',''); #PROC NUMERO DO PROCESSO ELEITORAL
        elif i.startswith('DTPL:') : DTPL = i.replace('DTPL:',''); #DATA DO PLEITO
        elif i.startswith('PLEI:') : PLEI = i.replace('PLEI:',''); #PLEI NUMERO DO PLEITO
        elif i.startswith('FASE:') : FASE = i.replace('FASE:',''); #FASE DOS DADOS: OFICIAL
        elif i.startswith('UNFE:') : UNFE = i.replace('UNFE:',''); #UNFE SIGLA DA UF
        elif i.startswith('MUNI:') : MUNI = i.replace('MUNI:',''); #NUM DO MUNICIPIO
        elif i.startswith('ZONA:') : ZONA = i.replace('ZONA:',''); #NUM ZONA ELEITORAL
        elif i.startswith('SECA:') : SECA = i.replace('SECA:',''); #NUM DA SEICAO ELEITORAL
        elif i.startswith('IDUE:') : IDUE = i.replace('IDUE:',''); #IDUE NUMERO DE SERIE DA URNA
        elif i.startswith('IDCA:') : IDCA = i.replace('IDCA:','');#CODIGO DA IDENTIFICACAO DA CARGA
        elif i.startswith('VERS:') : VERS = i.replace('VERS:',''); #VERSAO DO SOFTWARE
        elif i.startswith('LOCA:') : LOCA = i.replace('LOCA:',''); #NUM DO LOCAL DE VOTACAO
        elif i.startswith('APTO:') : APTO = i.replace('APTO:',''); #QNT DE ELEITORES APTOS
        elif i.startswith('COMP:') : COMP = i.replace('COMP:',''); #QNT DE ELITORES APARECEM PARA VOTAR
        elif i.startswith('FALT:') : FALT = i.replace('FALT:',''); #QNT DE ELTITORES FALTOSOS
        elif i.startswith('HBMA:') : HBMA = i.replace('HBMA:',''); #QNT DE ELEITORES HABILITADOS POR ANO DE NASCIMENTO
        elif i.startswith('DTAB:') : DTAB = i.replace('DTAB:',''); #DATA ABERTURA URNA
        elif i.startswith('HRAB:') : HRAB = i.replace('HRAB:',''); #HORA ABETURA URNA
        elif i.startswith('DTFC:') : DTFC = i.replace('DTFC:',''); #DATA FECHAMENTO URNA
        elif i.startswith('HRFC:') : HRFC = i.replace('HRFC:',''); #HORA FECHAMENTO URNA
        elif i.startswith('IDEL:') : IDEL = i.replace('IDEL:',''); #CODIGO DA ELEICAO
        elif i.startswith('CARG:') :  #CARG CODIGO DO CARGO

            CARG = i.replace('CARG:','')
            FLAG = (FLAG+1)
            FLAG = FLAG%2

        elif i.startswith('MAJO:') : MAJO = i.replace('MAJO:',''); #NUM DE VOTOS CARGO MAJORITARIO
        elif i.startswith('PROP:') : PROP = i.replace('PROP:',''); #NUM VOTOS CARGO PROPORCIONAIS
        elif i.startswith('TIPO:') : TIPO = i.replace('TIPO:',''); #TIPO 0-MAJORITARIO 1-PROPORCIONAL 2-CONSULTA
        elif i.startswith('VERC:') : VERC = i.replace('VERC:',''); #VERSAO DO PACOTE DE DADOS DE LEGENDA PARA O CARGO
        elif i.startswith('PART:') : PART = i.replace('PART:','') #NUMERO DO PARTIDO
        elif i.startswith('LEGP:') : LEGP = i.replace('LEGP:',''); #QNT DE VOTOS APURADOS DE LEGENDA PARA O PARTIDO
        elif i.startswith('TOTP:') : TOTP = i.replace('TOTP:',''); #TOTAL DE VOTOS APURADOS PARA O CARGO
        elif i.startswith('NOMI:') : NOMI = i.replace('NOMI:',''); #TOTAL DE VOTOS DE LEGENDA PARA O CARGO
        elif i.startswith('LEGC:') : LEGC = i.replace('LEGC:',''); #TOTAL DE VOTOS APURADOS PARA O CARGO
        elif i.startswith('BRAN:') : BRAN = i.replace('BRAN:',''); #TOTAL DE VOTOS APURADOS PARA O CARGO
        elif i.startswith('NULO:') : NULO = i.replace('NULO:',''); #TOTAL DE VOTOS APURADOS PARA O CARGO
        elif i.startswith('TOTC:') :

            TOTC = i.replace('TOTC:','') #TOTAL DE VOTOS APURADOS PARA O CARGO
            for j in LISTA:
                k = j.split(':') #NUMERO DO CAND #VOTOS

                #UF;Cidade;Num municipio;Urna;Zona;Secao;cargo;Candidato;Votos;Brancos;Nulos
                saida.write( line_split[0] + ';' + line_split[1] + ';' + MUNI + ';' +IDUE + ';' + ZONA + ';' + SECA + ';' + CARG + ';' + k[0] + ';' + k[1] + ';' + BRAN + ';' + NULO + '\n');
            LISTA = list()
            FLAG = (FLAG+1)
            FLAG = FLAG%2

        elif i.startswith('HASH:') : HASH = i.replace('HASH:',''); #total de votos para o cargo
        elif i.startswith('ASSI:') : ASSI = i.replace('ASSI:','');
        elif FLAG == 1 : #NUMERO DO CANDIDATO E QNT DE VOTOS PARA O CANDIDATO
            CANDIDATO = i
            LISTA.append(CANDIDATO)

    #a = raw_input();

    return;

e = open("planilhaQRcode.csv",'r'); # o arquivo de entrada deve estar em ordem crescente de de linhas de acordo com filtros de colunas
s = open("QRconvertido.csv",'wb'); # gera um arquivo de saida com os elementos do qr code separados
import collections

count_line = 0;
line = e.readline();count_line+=1;
line = e.readline();count_line+=1;
s.write('UF;Cidade;Num municipio;Urna;Zona;Secao;Candidato;Votos;Brancos;Nulos\n')


while(line != ''): #leitura ate EOF
    line = line.rstrip();
    line_split = line.split(';');
    #FORMATO RECEBIDO
    #UF#CIDADE#URNA#DATA-LEITURA-UTC#ZONA#SECAO#TURNO#QRCODE

    #DADOS DA PARTE QRCODE SAO SEPARADOS
    qr_split = line_split[7].split(' ');

    qr = qr_split[0];
    inicio = qr_split[0][5];
    fim =  qr_split[0][7];

    if qr.startswith( 'QRBU:1' ) :
        line_split.pop(); #RETIRADA DO ULTIMO ELEMENTO: STRING QRCODE, PARA INSERIR A LISTA QR JA SEPARADA


        if int(fim) > 1:

            qr_split.pop();#line_split.pop(); # HA PROXIMA LINHA A SER CONCATENADA PORTANTO RETIRA O ULTIMO ELEMENTO HASH
			#LE A PROXIMA LINHA PARA CONTINUAR O QR CODE
            line2 = e.readline(); line2 = line2.rstrip(); count_line+=1;
            line_split2 = line2.split(';');

            if set(line_split[0:6]) == set(line_split2[0:6]) : #CHECK PARA TER CERTEZA QUE AS LINHAS SE REFEREM AO MESMO QRCODE
                temp = line_split2[7].split(' ');
                temp.pop(0);
                temp.pop(0);

                qr_split.extend(temp); #CONACTENA O QRCODE

            else:
                print 'QR incompleto em: ' + str(count_line);a = raw_input();

        if int(fim) > 2:

            qr_split.pop();
            line2 = e.readline(); line2 = line2.rstrip(); count_line+=1;
            line_split2 = line2.split(';');

            if set(line_split[0:6]) == set(line_split2[0:6]) :

                temp = line_split2[7].split(' ');
                temp.pop(0);
                temp.pop(0);

                qr_split.extend(temp);

            else:
                print 'QR incompleto em: ' + str(count_line);a = raw_input();

        if int(fim) > 3:

            qr_split.pop();
            line2 = e.readline(); line2 = line2.rstrip(); count_line+=1;
            line_split2 = line2.split(';');

            if set(line_split[0:6]) == set(line_split2[0:6]) :

                temp = line_split2[7].split(' ');
                temp.pop(0);
                temp.pop(0);

                qr_split.extend(temp);

            else:
                print 'QRCode incompleto em:' + ' ' + str(count_line);a = raw_input();
        line_split.extend(qr_split);
        function(s,line_split); #FUNCTION REALIZA A SEPARACAO DOS DADOS COLHIDOS E ESCREVE EM ARQUIVO O QUE FOR NECESSARIO
        line = e.readline();count_line+=1;
    else:
        print 'QRCode incompleto em:' + ' ' + str(count_line);
        print line_split[0:];a = raw_input();
        line = e.readline();count_line+=1;

e.close();
s.close();

