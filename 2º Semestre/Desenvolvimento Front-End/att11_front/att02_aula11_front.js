const botao = document.getElementById("btnMostrar")
const area = document.getElementById("area")

botao.addEventListener("click", function () {
  // evita criar parágrafos repetidos a cada clique
  if (area.querySelector("p")) return

  const p = document.createElement("p")
  p.textContent = "Você clicou no botão!"
  area.appendChild(p)
})
