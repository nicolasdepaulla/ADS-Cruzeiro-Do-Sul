document.getElementById("btnCalcular").addEventListener("click", function () {
  const ano = parseInt(document.getElementById("ano").value, 10);
  const anoAtual = new Date().getFullYear();

  if (isNaN(ano) || ano > anoAtual) {
    alert("Digite um ano de nascimento válido.");
    return;
  }

  alert("Você tem " + (anoAtual - ano) + " anos.");
});
