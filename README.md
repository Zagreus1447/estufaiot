# estufaiot
Estufa iot feita para o evento do Techtalk na trilha de rede promovido pelo Pet da engenharia da computação de Sobral

##Montagem

Segue abaixo a montagem como foi realizado.

![Monatagem](images/estufaiot2.PNG)

No lugar do led vermelho foi ligado a ventoinha mas a plataforma que tinha a imagem não tinha ela então substitui só para fica melhor a exibição.
Os dht usados foram o dht11 não o 12 que veem com um pino de NC então o azul e o marrom correspondem ao data e alimentação segue a mesma, o nosso dht já estava conectado a placa dele assim facilitando a montagem.

##descricao 

O projeto é um estufa que controla a abertura da exaustão com base na temperatura interna e externa, ela utiliza dois sensores de temperatura e umidade que são o dht11 junto a placa dele e temos IOT o controle dele do feito através do blynk, para abri e fechar o teto utilizamos um servo moto e para comunicação com o blynk usamos o ESP32 e para fazer o acesso do sensores e controlar o servo usamos o  Arduino R3.

![Projeto](images/montagem1.png)

