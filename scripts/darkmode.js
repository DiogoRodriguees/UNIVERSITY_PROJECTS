const body = document.querySelector('body')
const figma = document.getElementById("figma")
const github = document.getElementById("github")
const resumo = document.getElementById('resumo')
const rodape = document.getElementById("rodapeId")
const IdNodeJS = document.getElementById("IdNodeJS")
const slideshow = document.querySelector("#conteinerSlides")
const btnDarkMode = document.getElementById('darkmode')

const item = document.querySelector(".item")
const textoitem = document.querySelector("#textoItem")
const listaDeitem = document.querySelectorAll("[id*=slides]")

const projetos = document.querySelector('#projetos')
const projetosTitulo = document.querySelector("#projetosTitulo")

const imgGmail = document.getElementById("imgGmail")
const imgLinkedin = document.getElementById("imgLinkedin")
const imgWhatsapp = document.getElementById("imgWhatsapp")
const imgInstagram = document.getElementById("imgInstagram")

const topicos = document.querySelectorAll('[id*=linguagensTopicos]')
const topicosTexto = document.querySelectorAll('[id*=topicosTexto]')
const topicosTitulo = document.querySelectorAll('[id*=topicosTitulo]')

const linguagensTitulo = document.querySelector('#linguangensTitulo')
const linguagensContainer = document.getElementById("linguagensId")

const imgs2 = [
    {'id' : '2', 'url': './img/SlideShow/img3_light.png'},
    {'id' : '1', 'url': './img/SlideShow/img1_light.png'},
    {'id' : '3', 'url': './img/SlideShow/img1_light.png'},
    {'id' : '4', 'url': './img/SlideShow/img2_light.png'},
]

var statuDarkMode = true
const ativaLightMode = () =>{
    body.style.background.url = ''
    body.classList.add('body-light')

    resumo.classList.add('resumo-light')
    linguagensTitulo.style.color = 'black'
    linguagensContainer.classList.add("linguagens-container-light")

    topicos.forEach((topico) => topico.classList.add('topicos-light'))
    topicosTexto.forEach((texto) => texto.style.color = 'black')
    topicosTitulo.forEach((titulo) => titulo.style.color = 'black')
    
    rodape.classList.add("rodape-light")
    projetos.classList.add('projetos-light')
    
    
    imgGmail.src = 'img/Rodape/gmail_black.png'
    imgWhatsapp.src = 'img/Rodape/whatsapp_black_slim.png'
    imgLinkedin.src = 'img/Rodape/linkedin_black_slim.png'
    imgInstagram.src = 'img/Rodape/instagram_black_slim.png'
    
    figma.classList.add('botoes-repositorio-light')
    github.classList.add('botoes-repositorio-light')

    btnDarkMode.innerHTML = 'Dark Mode'
    textoitem.classList.add('texto-item-light')
    
    conteinerItens.innerHTML = ''
    slideshow.classList.add("slideshow-light")

    loadImgs(imgs2, conteinerItens)

    return false
}
    
const ativaDarkMode = () =>{
    body.classList.remove('body-light')
    resumo.classList.remove('resumo-light')
    
    linguagensTitulo.classList.remove('.linguagens-titulo-light')
    linguagensTitulo.style.color = 'white'
    linguagensContainer.classList.remove("linguagens-container-light")

    topicos.forEach((topico) => topico.classList.remove('topicos-light'))
    topicosTexto.forEach((texto) => texto.style.color = 'white')
    topicosTitulo.forEach((titulo) => titulo.style.color = 'white')

    projetos.classList.remove('projetos-light')
    projetosTitulo.classList.remove('projetos-titulo-light')

    rodape.classList.remove("rodape-light")

    imgGmail.src = 'img/Rodape/gmail.png'
    imgWhatsapp.src = 'img/Rodape/whatsapp.png'
    imgLinkedin.src = 'img/Rodape/linkedin.png'
    imgInstagram.src = 'img/Rodape/instagram.png'

    figma.classList.remove('botoes-repositorio-light')
    github.classList.remove('botoes-repositorio-light')
    
    darkmode.innerHTML = 'Light Mode'
    conteinerItens.innerHTML = ''

    loadImgs(imgs, conteinerItens)

    return true
}

const mudarTema = () =>{
    statuDarkMode = statuDarkMode ? ativaLightMode() : ativaDarkMode();
}