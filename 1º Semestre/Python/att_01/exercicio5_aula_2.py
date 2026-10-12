# Escreva um programa em Python que leia a cotação do
# dólar (taxa de conversão), leia um valor em dólares e converta
# e mostre o valor equivalente em Reais.
#                      ~~~~~~~~~~~

#taxa de conversão atualmente: 1Dolar(EUA) é = R$5,33

#Definindo a cotação fixa!
valor_dolar = 5.33

#Input para que o usuário coloque o valor a ser convertido
dolar = float(input("Digite o valor a converter para Real (Somente Numero): "))

#variavel para coletar os dados da variavel"dolar"
real = dolar*valor_dolar

#Exibindo o resultado da multiplicação
print("O valor convertido foi de:R$", real)


