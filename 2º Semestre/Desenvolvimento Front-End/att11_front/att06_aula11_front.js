document.getElementById("formRetangulo").addEventListener("submit", function (evento) {
  evento.preventDefault(); // evita recarregar a página

  const base = parseFloat(document.getElementById("base").value);
  const altura = parseFloat(document.getElementById("altura").value);
  const resultado = document.getElementById("resultado");

  if (isNaN(base) || isNaN(altura)) {
    resultado.textContent = "Preencha base e altura com números válidos.";
    return;
  }

  resultado.textContent = "A área do retângulo é de: " + (base * altura);
});
