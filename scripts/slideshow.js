const imgs = [
    // {'id' : '2', 'url': './img/SlideShow/img3.png'},
    // {'id' : '1', 'url': './img/SlideShow/img1.png'},
    // {'id' : '3', 'url': './img/SlideShow/img1.png'},
    // {'id' : '4', 'url': './img/SlideShow/img2.png'},
    { id: "4", url: "./img/MeusProjetos/calculadora_com_react.jpg" },
    { id: "1", url: "./img/MeusProjetos/img_projetofinancas.png" },
    { id: "5", url: "./img/MeusProjetos/img_projetofinancas.png" },
    { id: "6", url: "./img/MeusProjetos/todolista_com_javascript.png" },
];

const conteinerItens = document.querySelector('#conteiner')

const  loadImgs = (imgs, conteiner)=>{

    imgs.forEach(img => {
        if(img.id === '1'){
            conteiner.innerHTML +=
            `<div class='item'>
                <p id="textoItem">Confira algumas imagens de projetos que fiz</p>
                <img id='slides${img.id}' src='${img.url}'>
            </div>`
        }else{
            conteiner.innerHTML +=
            `<div class='item'>
                <img id='slides${img.id}' src='${img.url}'>
            </div>`
        }
    });
}

loadImgs( imgs, conteinerItens);

let listaDeItens = document.querySelectorAll('.item')

const nextSlide = () =>{
    conteinerItens.appendChild( listaDeItens[0])
    listaDeItens = document.querySelectorAll('.item')
}

const previousSlide = () =>{
    const lastItem = listaDeItens[listaDeItens.length -1]
    conteinerItens.insertBefore( lastItem, listaDeItens[0])
    listaDeItens = document.querySelectorAll('.item')
}

document.querySelector('#previous').addEventListener('click', previousSlide)
document.querySelector('#next').addEventListener('click', nextSlide)