#Desenvolva um programa em Python que solicite ao
#usuário os valores dos lados de um retângulo e calcule e
#mostre seu perímetro e sua área.

#formula do perimetro = base + altura * 2
#formula da area = base * altura

base = float(input("digite o valor da base do retangulo: "))
altura = float(input("digite o valor da altura do retangulo: "))
perimetro = (base + altura) * 2
area = (base * altura)
print("  O perimetro do retangulo é:", perimetro, "\n  A area do retangulo é: ", area)
#Usando o "\n" para deixar o código mais limpo e pular linha
