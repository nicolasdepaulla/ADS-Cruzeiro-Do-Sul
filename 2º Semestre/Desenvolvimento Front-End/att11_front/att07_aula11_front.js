function calcularMedia(a, b, c) {
  return (a + b + c) / 3;
}

document.getElementById("btnMedia").addEventListener("click", function () {
  const n1 = parseFloat(document.getElementById("n1").value);
  const n2 = parseFloat(document.getElementById("n2").value);
  const n3 = parseFloat(document.getElementById("n3").value);
  const resultado = document.getElementById("resultado");

  if (isNaN(n1) || isNaN(n2) || isNaN(n3)) {
    resultado.textContent = "Preencha os três números.";
    return;
  }

  resultado.textContent = "A média é de: " + calcularMedia(n1, n2, n3).toFixed(2);
});
