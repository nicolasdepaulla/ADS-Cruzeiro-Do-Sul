#Escreva um programa em Python que calcule as duas
#raízes de uma equação de 2º grau ax²+bx+c, conhecendo os
#valores dos coeficientes da mesma (a, b, c). Suponha que as
#raízes são reais. Lembre-se que para calcular as duas raízes:

#Pegando os valores de A B C
a = float(input("Digite o valor de a: "))
b = float(input("Digite o valor de b: "))
c = float(input("Digite o valor de c: "))

#Fazendo a formula de delta: b² -4.a.c
delta = b**2 -4*a*c

# delta**0.5 calcula a raiz quadrada de delta
# -b muda o sinal do valor de b
# (2*a) representa o denominador da fórmula de Bhaskara
raiz1 = (-b + delta**0.5) / (2*a)
raiz2 = (-b - delta**0.5) / (2*a)

#Exibindo os valores de delta e das raizes
print("Valor de Delta é: ", delta)
print("Valor da primeira Raiz é: ", raiz1)
print("Valor da segunda Raiz é: ", raiz2)

