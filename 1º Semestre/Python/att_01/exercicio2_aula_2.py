#Escreva um programa em Python que solicite ao usuário o
#salário atual e mostre o salário acrescido de 5% de comissão

salario = float(input("diga seu salário atual: "))
#comissão vai ser: Valor * porcentagem / 100, que vai dar o salário com a comissão
comissao = (salario * 5) / 100 
#salário final pegando a comissão
salario_final = salario + comissao
print("Seu salário com comissão vai ser: ", salario_final)