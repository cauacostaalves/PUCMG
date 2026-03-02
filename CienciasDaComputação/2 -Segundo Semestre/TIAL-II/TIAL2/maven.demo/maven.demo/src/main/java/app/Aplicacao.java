package app;

import static spark.Spark.*;
import model.Usuario;
import dao.UserDAO;

public class Aplicacao {
    public static void main(String[] args) {
        // Inicializa o servidor Spark
        port(4567);  // Definindo a porta (pode ser alterada)
        staticFiles.location("/public");  // Servir arquivos estáticos da pasta /public


        // Configuração para lidar com o POST do formulário
        post("/cadastrar", (request, response) -> {
            // Pega os dados do formulário
            String nome = request.queryParams("nome");
            String email = request.queryParams("email");
            String senha = request.queryParams("senha");

            // Cria um novo objeto de usuário
            Usuario novoUsuario = new Usuario(nome, email, senha);

            // Chama o método no DAO para salvar o usuário
            UserDAO userDAO = new UserDAO();
            userDAO.cadastrar(novoUsuario);

            // Define o tipo de resposta e redireciona para uma página de sucesso
            response.redirect("/sucesso.html");
            return null;  // O redirecionamento já trata a resposta
        });

        // Exemplo de rota para carregar a página de sucesso
        get("/sucesso.html", (request, response) -> {
            return "<h1>Cadastro realizado com sucesso!</h1>";
        });
    }
}