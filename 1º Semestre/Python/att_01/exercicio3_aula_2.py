#Escreva um programa em Python que solicite ao usuário a
#distância entre duas cidades e o tempo de viagem. O
#programa deverá calcular e exibir a velocidade média de um
#carro que vai de uma cidade para outra. Utilize a formula:

distancia = float(input("digite a distancia entre as duas cidades que deseja ir (Km) : "))
#Forçando o usuário a digitar somente numero
tempo = float(input("digite o tempo da viagem entre as cidades (apenas numero) : "))

#pegando essas duas informações e fazendo a formula: Velocidade = distancia / tempo
Velocidade_media = distancia / tempo

print("A velocidade média é: ", Velocidade_media)