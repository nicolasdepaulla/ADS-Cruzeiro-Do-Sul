#Escreva um programa em Python que leia um valor
#representando o gasto realizado por um cliente do restaurante
#ComaBem e visualize o valor total a ser pago, considerando
#os 10% do garçom


#Variável para coletar os gastos do cliente
gasto = float(input("Digite o valor da conta:R$"))

#Variável para taxa do garçom: 0.10 é equivalente a 10%
taxa_garcom = gasto * 0.10

#variável para captar o total gasto pelo cliente
total = gasto + taxa_garcom

#Exibindo resultado
print("O valor da conta deu:R$ ", gasto)
print("Taxa do Garçom:R$ ",taxa_garcom )
print("O total deu:R$ ",total)