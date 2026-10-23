function calcularQuadrado(numero) {
  return numero * numero
}

document.getElementById("btnCalcular").addEventListener("click", function () {
  const numero = parseFloat(document.getElementById("numero").value)
  const resultado = document.getElementById("resultado")

  if (isNaN(numero)) {
    resultado.textContent = "Digite um número válido."
    return;
  }

  resultado.textContent = "O quadrado do número é: " + calcularQuadrado(numero)
})
