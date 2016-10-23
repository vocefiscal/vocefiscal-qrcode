entrada = open("buentrada.txt",'r');
saida = open("busaida.txt",'w+');

for line in entrada:
    #print line
    #a = raw_input()
    strin = line
    while ( '\"' in strin):
        strin = strin.replace('\"','')
    #print strin
    saida.write(strin)
