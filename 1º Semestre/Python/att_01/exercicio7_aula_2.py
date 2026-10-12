#Escreva um programa em Python que obtenha uma
#temperatura em graus Celsius, calcule e mostre a respectiva
#temperatura nas escalas Fahrenheit e Kelvin. Utilize as
#fórmulas abaixo:


#capturar a temperatura em graus celsius
celsius = float(input("Diga a temperatura em graus celsius: "))

# Convertendo para fahrenheit usando a formula: tf = 1,8 * tc + 32
fahrenheit = 1.8 * celsius + 32

#Converter para Kelvin usando a formula: Tk= tc + 273
kelvin = celsius + 273

#Exibir os resultados: Usando .2f = Mostrar o numero com 2 casas decimais e deixar organizado
print(f"Graus celsius: {celsius:.2f}°C",)
print(f"Fahrenheit:{fahrenheit:.2f} °F",)
print(f"kelvin:{kelvin:.2f}K",)