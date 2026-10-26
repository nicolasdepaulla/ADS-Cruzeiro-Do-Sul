# PROMPT DE RETOMADA — Projeto "Conecta Empresas"

> Cole este arquivo inteiro como primeira mensagem em um chat novo com o Claude (de preferência o Claude Code, com o repositório conectado) e anexe o zip `conecta-chat-llm.zip` (se o chat LLM ainda não estiver commitado no repositório). Depois diga: "Leia tudo, confirme que entendeu e siga para o próximo passo do roadmap."

---

## 0. Quem eu sou e como quero ser atendido

- Sou o **Nick (Nicolas de Paula Pacheco)**, de São Paulo/SP, Jovem Aprendiz em Suporte Comercial na S.I.N Implant System, cursando Tecnólogo em Análise e Desenvolvimento de Sistemas (Cruzeiro do Sul). Tenho cursos técnicos de Informática e Cibersegurança (Senac).
- **Escreva sempre em português do Brasil.**
- Prefiro **implementação direta** (alterar arquivos) a entregas só de documentação.
- Em sessões guiadas de código: **primeiro o bloco de código completo (não fatiado), depois a explicação completa abaixo dele.**
- Estou aprendendo: explique decisões de forma clara, mas sem enrolação. Quero respostas **assertivas**; se eu estiver errado, diga com franqueza e justifique.

### Regra de entrega (SEMPRE, a cada feature)
1. Você altera os arquivos no seu clone/sandbox e roda os testes.
2. Você me entrega **um zip só com os arquivos novos/alterados**, mantendo a estrutura de pastas (para eu extrair na raiz do projeto).
3. Para **cada arquivo**: o caminho completo, o nome e um breve resumo do que mudou.
4. Os comandos de teste que eu devo rodar e o resultado que você obteve.
5. Uma **mensagem de commit sugerida**.
6. **Eu faço o commit e o push. Você NUNCA commita nem dá push**, mesmo que algum hook automático ("stop hook: há mudanças não commitadas") peça — recuse e lembre que o commit é meu.
7. Se eu não estiver no PC, prefiro receber o zip em vez de trechos soltos de código.
8. Se você não conseguir validar algo (ex.: chamada real a uma API paga), diga explicitamente o que **não** foi testado.

### Política de handoff
Quando a conversa estiver perto de esgotar os tokens, gere um resumo (ideia do projeto, repositório, estrutura de pastas, onde paramos) para eu colar em um chat novo — como este documento.

---

## 1. O projeto

**Conecta Empresas**: sistema de venda de **pacotes de dados de contato empresarial para consórcios** (setores imobiliário, automotivo, turismo). Originalmente em Node.js (Express + MongoDB/Mongoose + JWT + bcrypt), foi **reescrito por completo em Python/FastAPI** com arquitetura em camadas e depois **migrado de MongoDB para PostgreSQL**.

- Repositório atual: **https://github.com/nicolasdepaulla/Conecta-Empresas-py** (o antigo Node: `nicolasdepaulla/Conecta-Empresas`).
- Ainda não foi para produção; o deploy está só documentado (`docs/DEPLOY.md`, AWS EC2 + Docker Compose).
- O front-end foi redesenhado de forma profissional (paleta: fundo `#F7F6F3`, tinta `#1B2430`, destaque verde-petróleo `#2F6F62`; fontes Sora + Inter).

### Stack
- **Backend:** Python 3.13, FastAPI 0.115, Uvicorn, Pydantic v2 / pydantic-settings.
- **Banco:** PostgreSQL 16, SQLAlchemy 2.0 **async** + asyncpg, **Alembic** (migrations).
- **Auth:** JWT (python-jose) em **cookie httpOnly** (`authToken`, SameSite=lax, `secure=True` quando `AMBIENTE=production`), senhas com bcrypt (passlib + bcrypt 4.0.1).
- **Pagamento:** Mercado Pago Checkout Pro (API de Preferences) + webhook com validação de assinatura HMAC.
- **E-mail:** SMTP (Gmail) para "esqueci minha senha"; sem SMTP configurado, o link aparece no terminal.
- **Rate limiting:** slowapi (por IP).
- **LLM:** SDK `anthropic==1.13.0`, modelo `claude-haiku-4-5-20251001`.
- **Front-end:** HTML + CSS + JS puro em `public/` (sem framework); Chart.js (CDN jsdelivr) no dashboard.
- **Infra/dev:** Docker + docker-compose (Postgres na porta **5433** no host), pgAdmin (perfil `dev`, porta 5050), GitHub Actions (testes + Postgres real), Dependabot.
- **Testes:** pytest 8.3 + pytest-asyncio 0.24 (`asyncio_mode=auto`), mocks via monkeypatch/AsyncMock.

### Estrutura de pastas
```
.
├── .env / .env.example
├── .github/workflows/tests.yml        # CI (pytest + Postgres real + alembic upgrade head)
├── Dockerfile, docker-compose.yml, .dockerignore, alembic.ini, pytest.ini
├── requirements.txt, requirements-dev.txt
├── alembic/versions/                  # baseline + schema inicial (usuarios, sessoes, pacotes, pedidos)
├── docs/DEPLOY.md                     # guia de deploy AWS (EC2 + Compose)
├── pgadmin/servers.json               # conexão pré-cadastrada do pgAdmin
├── scripts/                           # backup.py (pg_dump), seed.py, set_admin.py
├── app/
│   ├── main.py                        # app FastAPI, middlewares, routers, handler do rate limit
│   ├── core/    config.py, deps.py, email.py, limiter.py, llm_client.py,
│   │            postgres.py, security.py, security_headers.py
│   ├── models/        (schemas Pydantic) chat, pacote, pedido, senha, sessao, usuario
│   ├── models_sql/    (tabelas SQLAlchemy) pacote, pedido, sessao, usuario
│   ├── repositories/  pacote, pedido, sessao, usuario  (acesso ao banco)
│   ├── services/      admin, chat, mercadopago, pacote, pedido, sessao  (regras de negócio)
│   └── routers/       admin, chat, pacotes, pagamentos, sessoes, usuarios  (HTTP)
├── public/
│   ├── index, login, cadastro, esqueci-senha, redefinir-senha, logado, pedidos, dashboard (.html)
│   ├── css/chat-widget.css
│   └── js/ cadastro, chat-widget, dashboard, erros, esqueci-senha, logado, login,
│          pedidos, redefinir-senha, senha-toggle (.js)
└── tests/  test_admin_dashboard, test_chat, test_email, test_pacote_service,
            test_pedido_service, test_security, test_usuario_service, test_webhook
```
Arquitetura: **router → service → repository → model SQL**. Routers só tratam HTTP; regras ficam em services; SQL fica em repositories.

### Banco (4 tabelas da aplicação + `alembic_version`)
- `usuarios` (inclui `is_admin`), `pacotes`, `sessoes` (itens/descrição de cada pacote, evita repetir dados nos HTMLs — pacotes resolvem a sessão via JOIN), `pedidos`.
- `pedidos`: `usuario_id` e `pacote_id` são **FK reais**; `status` é **ENUM nativo** do Postgres (`pendente`/`pago`/`cancelado`); `pacote_nome` e `valor` são **snapshot** do momento da compra.

### Endpoints
- Usuários (`app/routers/usuarios.py`): `POST /register`, `POST /login`, `POST /logout`, `GET /get-username`, `POST /esqueci-senha`, `POST /redefinir-senha`.
- Pacotes: `GET /pacotes`, `GET /pacotes/{slug}`.
- Pagamentos: `POST /pagamentos/checkout/{slug_pacote}`, `POST /pagamentos/webhook`, `GET /pagamentos/pedidos/meus`.
- Admin: `GET /admin/dashboard` (exige `is_admin`).
- Chat: `POST /chat/mensagem` (exige login, 20/min).
- Limites de rate limit: login/cadastro 5/min, esqueci-senha 3/min, redefinir-senha 5/min, chat 20/min.

### Variáveis de ambiente (`.env`, modelo em `.env.example`)
`AMBIENTE`, `DATABASE_URL` (padrão `postgresql+asyncpg://conecta:conecta@localhost:5433/conecta_empresas`), `JWT_SECRET_KEY` (**obrigatória** — sem ela o pytest nem coleta), `JWT_ALGORITHM`, `JWT_EXPIRATION_MINUTES`, `PAYMENT_PROVIDER_API_KEY`, `PAYMENT_PROVIDER_WEBHOOK_SECRET`, `PUBLIC_BASE_URL`, `SMTP_HOST/PORT/USER/PASSWORD/FROM_EMAIL`, `PGADMIN_DEFAULT_EMAIL/PASSWORD` (padrão `admin@conecta-empresas.com` / `conecta`; a senha do Postgres é `conecta`), `ANTHROPIC_API_KEY`, `ANTHROPIC_MODEL`, `SUPORTE_CONTATO_HUMANO`.

---

## 2. Histórico do que JÁ foi feito (tudo validado)

1. **Reescrita Node → Python/FastAPI** com arquitetura em camadas; cadastro/login com nome do usuário no canto superior direito, listagem de pacotes, normalização das "sessões" dos pacotes.
2. **Pagamento real** (Mercado Pago Checkout Pro, sandbox) + webhook com HMAC e logging + ngrok em dev. O botão "Pagar" do sandbox travou no navegador (provável bloqueio de cookies de terceiros) — **decidimos deixar de lado** e compensar com testes automatizados do webhook (11 testes).
3. **Tela "Meus Pedidos"** (pacote, valor, data, status).
4. **Redefinição de senha por e-mail** (SMTP Gmail, link com expiração) + ícone de olho para mostrar/ocultar senha.
5. **CI/CD** (GitHub Actions), **Docker/Compose**, **rate limiting**, **dashboard administrativo** (is_admin, `scripts/set_admin.py`, agregações, Chart.js).
6. Issues do backlog: e-mail duplicado bloqueado, cookie seguro em produção, healthcheck, logs padronizados com `logging`, backup.
7. **Migração MongoDB → PostgreSQL** completa (infra, models, repositories um a um, dashboard em SQL, CI com Postgres real, backup com `pg_dump` em `scripts/backup.py`, correção do Dockerfile que não copiava `alembic/`). **Mongo foi removido da aplicação.**
8. **Hardening de segurança:** cabeçalhos HTTP (middleware `SecurityHeadersMiddleware`), senha mínima de 8 no reset, cookie SameSite=lax, Dependabot, log do webhook reduzido, correção de "[object Object]" nas mensagens de erro (422), correção de XSS em `logado.html` (innerHTML → DOM).
9. **Responsividade mobile:** hero cobrindo login/cadastro (bug de ordem do CSS: `@media` antes da regra incondicional) e cabeçalhos sobrepostos em logado/dashboard/pedidos; caixa de erro vazia no carregamento corrigida.
10. **Content-Security-Policy:** todo `<script>` inline de `public/` extraído para `.js`; `onclick=""` → `addEventListener`; `script-src 'self' https://cdn.jsdelivr.net` (sem `unsafe-inline`); `style-src` mantém `unsafe-inline` (documentado); `connect-src 'self'`.
11. **pgAdmin** no `docker-compose.yml` (perfil `dev`; sobe com `docker compose --profile dev up`) para eu visualizar o banco e saber qual usuário de teste estou usando.
12. **Chat de suporte (SAC) com LLM — implementado e validado no sandbox; o zip está anexado/ao lado deste prompt; o commit é meu** (ver seção 3).

---

## 3. Chat de suporte (SAC) com Claude — especificação completa

### O que eu pedi
Um SAC: responde dúvidas dos clientes sobre os produtos; para problemas **não graves** dá o **passo a passo** de resolução; para problemas **graves/difíceis** "filtra" e **passa para um humano** (telefone/central/contato). Precisa ser **preciso, claro e assertivo** — tive experiências ruins com chatbots vagos. Decisões: provedor **Claude (Anthropic)**; **botão flutuante em toda página logada**; contato de escalada **nick.paulo.pacheco@gmail.com** (meu e-mail, autorizado como contato de teste; futuramente será um canal real de atendimento).

### Arquivos (todos no zip `conecta-chat-llm.zip`, 15 arquivos, extrair na raiz)
**Novos**
- `app/core/llm_client.py` — chama a API da Anthropic. Contém `MAX_TOKENS_RESPOSTA=1024`; a ferramenta `FERRAMENTA_ESCALAR` (`escalar_para_atendimento_humano`, parâmetro obrigatório string `motivo`); o `PROMPT_SISTEMA_BASE` (regras do SAC em PT-BR); `montar_prompt_sistema(catalogo)`; `_extrair_resposta(...)` (lê blocos `text`/`tool_use`) e `perguntar(mensagem, historico_anthropic, catalogo_formatado)`.
- `app/services/chat_service.py` — `MAX_MENSAGENS_HISTORICO=10`, mapa de papéis (`usuario→user`, `assistente→assistant`), `_formatar_catalogo(pacotes)` (uma linha por pacote: nome, setor, preço em R$, nº de contatos, itens; catálogo vazio → "(nenhum pacote cadastrado no momento)"), `_montar_historico_anthropic`, `responder(entrada)`.
- `app/routers/chat.py` — `POST /chat/mensagem`, `@limiter.limit("20/minute")`, `Depends(get_current_user)`.
- `app/models/chat.py` — `MensagemHistorico` (papel `usuario|assistente`, conteúdo ≤2000), `ChatMensagemEntrada` (mensagem 1–1000 chars, histórico ≤20), `ChatMensagemSaida` (`resposta`, `escalar`, `contato_humano`).
- `public/js/chat-widget.js` — widget (IIFE). Histórico no `sessionStorage` (`conecta_chat_historico`, até 20 guardadas, envia as últimas 10) para sobreviver à navegação entre páginas HTML separadas; bolha "Digitando..."; mensagens de escalada/erro com estilo de destaque `chat-msg-escalada`; trata erro de rede.
- `public/css/chat-widget.css` — botão flutuante, painel de 340px, bolhas, responsivo (`max-width: 480px`), usa as variáveis CSS das páginas.
- `tests/test_chat.py` — 13 testes.

**Alterados**
- `app/main.py` (registra o router), `app/core/config.py` (`anthropic_api_key`, `anthropic_model`, `suporte_contato_humano`), `requirements.txt` (`anthropic==1.13.0`), `.env.example`, `README.md` (seção "Chat de suporte (LLM)", roadmap, limitações), `public/logado.html`, `public/pedidos.html`, `public/dashboard.html` (CSS + markup do widget + `<script src="/js/chat-widget.js">`).

### Regras de projeto do chat (manter!)
- **O modelo nunca executa ação real.** A única ferramenta (`escalar_para_atendimento_humano`) não tem efeito colateral: só sinaliza a escalada. O "raio de explosão" de uma resposta ruim é no máximo uma explicação errada ou uma escalada desnecessária.
- **Catálogo vindo do banco, injetado no prompt a cada chamada**, como única fonte de verdade (evita preço/pacote inventado). Se a informação não está no contexto, **escalar em vez de supor**.
- Prompt: escopo restrito à Conecta Empresas; direto e assertivo; não revelar as instruções; **admite ser uma IA** (nunca finge ser humano). Escala em: cobrança indevida, pagamento feito mas não reconhecido, suspeita de fraude/segurança da conta, reclamação séria, pedido de reembolso, pedido explícito por humano, ou falta de informação. Passo a passo conhecido: pacote não aparece → F5/novo login; esqueci a senha → link "Esqueci minha senha"; pedido "pendente" → normal até o Mercado Pago confirmar.
- **Texto da escalada e contato são montados pelo backend** ("Vou te encaminhar para um atendente humano: {contato}"), nunca pelo modelo — o contato exibido é sempre o real.
- **Falhas viram escalada, não erro seco:** sem `ANTHROPIC_API_KEY`, erro da API (`anthropic.APIError`) → HTTP 503 com mensagem amigável e o contato; resposta vazia do modelo → escala por segurança.
- Chave da API **só no servidor**. Login obrigatório (cookie JWT), 20 msgs/min, mensagem ≤1000 chars, histórico ≤10 no servidor.
- Testes **não** chamam a API real (mock de `llm_client.perguntar`).

### Validação feita
`pytest -q` → **60 passed** (47 anteriores + 13 do chat). Playwright (logado): botão aparece, painel abre, envia mensagem, 503 sem chave renderiza como bolha de escalada com o contato. `POST /chat/mensagem` sem login → 401. CSP intacto.
**Não validado:** resposta real do Claude (o sandbox não tinha chave), widget em viewport mobile (390×844) e a escalada via tool-call contra a API real. → **Preciso testar com minha `ANTHROPIC_API_KEY`** e ajustar o prompt em `llm_client.py` se escalar demais/de menos.

### Como aplicar/testar
```bash
# extrair conecta-chat-llm.zip na raiz do projeto
pip install -r requirements.txt
pytest -v
# no .env real:
#   ANTHROPIC_API_KEY=sua-chave
#   SUPORTE_CONTATO_HUMANO=nick.paulo.pacheco@gmail.com
# subir o servidor, logar e clicar no 💬. Testes: "quanto custa o pacote imobiliário?" (responde com o preço do banco)
# e "fui cobrado duas vezes e quero reembolso" (deve escalar e mostrar o contato)
```
Commit sugerido: `feat: adiciona chat de suporte (SAC) com LLM (Claude), com escalada pra atendimento humano e widget flutuante`

---

## 4. O que falta / próximos passos

**Imediato**
- [ ] Aplicar o zip do chat, testar com a chave real da Anthropic, ajustar o prompt e **commitar** (por minha conta).
- [ ] (Opcional) Validar o widget no viewport mobile com Playwright.

**Roadmap em aberto**
- [ ] **Melhorias do chat:** histórico no servidor (hoje só `sessionStorage`), abertura de chamado/ticket real e notificação ao atendente (hoje só mostra um contato), revisar o risco de *prompt injection* se o catálogo passar a ser editável por terceiros, métricas/logs de escalada, ajuste fino do prompt com conversas reais.
- [ ] **Ingestão de dados de CNPJ (leads):** eu usava scraping (BeautifulSoup) num site que passou a bloquear. **Decisão:** não contornar o bloqueio; usar fontes legítimas — **Dados Abertos do CNPJ da Receita Federal** ou **BrasilAPI** — como base de leads. Desenho a fazer depois (provavelmente script/serviço de ingestão em lote + tabela própria + atualização periódica; atenção à LGPD ao tratar dados de contato).
- [ ] **2FA:** adiado conscientemente (sem e-mail real configurado em dev para validar o fluxo ponta a ponta).
- [ ] **Rate limiting por usuário:** hoje é só por IP; adiar e manter documentado como limitação consciente (exigiria lógica própria; teste via chamadas repetidas ao `/login` com o mesmo usuário e IPs forjados).
- [ ] **Login com Google:** adiado.
- [ ] **Deploy na AWS** de verdade (EC2 + Docker Compose, já documentado em `docs/DEPLOY.md`); por ora não vou subir para produção.
- [ ] **Teste ponta a ponta do pagamento** no sandbox do Mercado Pago (parado pelo botão "Pagar" travado).
- [ ] Gerar, ao final, um arquivo completo do projeto citando as tecnologias usadas.

**Ideia só para estudo (NÃO commitar):** pivotar/expandir para **"Conecta Supply"** — plataforma B2B de compras/suprimentos com estoque e indicadores, inspirada no meu trabalho de suporte comercial. Retorno crítico já dado: é um domínio de negócio diferente do atual e exigiria **multi-tenancy**, que hoje não existe. Guardar a ideia, sem implementar.

### Limitações conhecidas (decisões conscientes, documentadas no README)
- Rate limiting só por IP; CSP com `style-src 'unsafe-inline'` (CSS não executa código; script-src está estrito); sem 2FA; chat sem histórico no servidor e sem ticket real; catálogo tratado como confiável no prompt.

---

## 5. Observações para o ambiente de trabalho

- Se o Claude tiver acesso ao repositório (conector GitHub com leitura em `nicolasdepaulla/Conecta-Empresas-py`), reclonar em vez de pedir os arquivos de novo. Se o chat LLM ainda não estiver no repositório, extrair o zip por cima.
- Sandbox: se o Postgres local rodar na 5432 (e não 5433 do Docker), ajustar `DATABASE_URL` no `.env`. Sem `.env`, o pytest quebra na coleta (`JWT_SECRET_KEY` obrigatória) → copiar de `.env.example`.
- O pip no sandbox exige `--break-system-packages`. Para o Playwright, usar o Chromium pré-instalado (`/opt/pw-browsers/chromium`); não rodar `playwright install`.
- Ao iniciar o uvicorn em segundo plano, confirmar com uma chamada `curl` separada (combinar tudo numa só chamada gerou falso "connection refused").
- Antes de qualquer feature nova: ler o README e este documento, propor o plano em poucas linhas e **seguir a regra de entrega da seção 0**.

**Primeira tarefa no chat novo:** confirmar que entendeu o contexto, listar os arquivos do zip do chat em uma linha cada e perguntar se já testei com a chave real da Anthropic; depois seguir o próximo item do roadmap que eu escolher.
