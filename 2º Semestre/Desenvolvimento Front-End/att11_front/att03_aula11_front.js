/*
  Pergunta: o que acontece quando os botões são acionados em ordens diferentes?

  - Se "vermelho" ou "negrito" forem clicados ANTES de "mostrar mensagem",
    o parágrafo ainda não existe. Numa versão ingênua (que procura o <p>
    direto no clique) isso gera erro (null). Aqui guardamos o estado
    (cor e negrito) em variáveis, então o estilo é aplicado assim que a
    mensagem aparecer.
  - Depois que o parágrafo existe, vermelho e negrito são independentes:
    um não desfaz o outro, e a ordem entre eles não muda o resultado final
    (texto vermelho E em negrito).
  - Clicar de novo em "mostrar mensagem" não duplica o parágrafo.
*/
const area = document.getElementById("area")
let paragrafo = null
let vermelho = false
let negrito = false

function aplicarEstilos() {
  if (!paragrafo) return
  paragrafo.style.color = vermelho ? "red" : ""
  paragrafo.style.fontWeight = negrito ? "bold" : ""
}

document.getElementById("btnMostrar").addEventListener("click", function () {
  if (!paragrafo) {
    paragrafo = document.createElement("p")
    paragrafo.textContent = "Água mole em pedra dura, tanto bate até que fura."
    area.appendChild(paragrafo)
  }
  aplicarEstilos()
});

document.getElementById("btnVermelho").addEventListener("click", function () {
  vermelho = true
  aplicarEstilos()
})

document.getElementById("btnNegrito").addEventListener("click", function () {
  negrito = true
  aplicarEstilos()
})
