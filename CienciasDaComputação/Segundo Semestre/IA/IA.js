// Função que é chamada quando o usuário seleciona uma imagem
function imagemSelecionada(imgElement) {
    const imgSrc = imgElement.src;
    alert("Você selecionou a imagem: " + imgSrc);
    
    // Enviar o endereço da imagem para a página .java
    enviarParaBackend(imgSrc);
}

// Função para enviar o endereço da imagem para o backend (página .java)
function enviarParaBackend(imgSrc) {
    fetch('/CustomVisionClient.java', { // Altere o caminho conforme necessário
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ imagem: imgSrc })
    })
    .then(response => response.json())
    .then(data => {
        console.log('Resposta do servidor:', data);
        // Se necessário, você pode tratar a resposta do servidor aqui
    })
    .catch(error => {
        console.error('Erro ao enviar para o servidor:', error);
    });
}