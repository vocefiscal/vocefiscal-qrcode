#######################################
########################################
###### Altere o N e a confiança 
###### abaixo e cole no "Console" do R
###### (no meu note de 6GB de memoria, nao roda para N>15000)

sink('/dev/null') ## suprimir output de comandos
args = commandArgs(trailingOnly=TRUE) ## pegar argumentos passados como parametro
if (length(args) == 0 || length(args) == 1) {
  stop("Devem ser passados 2 parametros, tamanho da populacao e confianca", call.=FALSE)
}


N<-as.numeric(args[1])          #### tamanho da população de urnas
confianca<-as.numeric(args[2])  #### Confiança

priori<-numeric(N)
priori.0<- 0.5    #### priori 
priori[1:N]<-seq(N,1)/sum(seq(1,N))*(1-priori.0)
sum(priori)

theta<-matrix(0,N,N)   #### montando a matriz de probabilidades
for (i in 1:N){ theta[i,]<-(N-seq(1,N)-(i-1))/(N-(i-1)) }
theta[which(theta<0)]<-0
for (j in 1:N){ theta[,j]<-cumprod(theta[,j]) }

p0<-numeric(N)
for (k in 1:N){
   p0[k]<-priori.0 + sum(priori*theta[k,])
}
p0

p.final<-priori.0/p0
p.final

plot(seq(1,N),p.final,pch=16,main="Confiança x Amostra", xlab="Tamanho da Amostra", ylab="Confiança")
a<-min(which(p.final>=confianca))
points(c(a,a),c(0,confianca),col=2,type="l")
points(c(0,a),c(confianca,confianca),col=2,type="l")

legend(a+1,confianca,a,bty="n",text.col=4)

sink() ## liberar output de comandos
cat(a) ## amostra minima necessaria
####################################################
####################################################
