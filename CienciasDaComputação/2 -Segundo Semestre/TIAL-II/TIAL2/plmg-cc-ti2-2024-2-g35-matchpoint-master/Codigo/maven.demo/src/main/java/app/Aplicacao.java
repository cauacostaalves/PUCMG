package app;

import static spark.Spark.*;

import com.google.gson.Gson;

import model.Grupo;
import model.Usuario;
import dao.GrupoDAO;
import dao.UserDAO;

public class Aplicacao {
    public static void main(String[] args) {
        
        port(4567);  
        staticFiles.location("/public");  

        // USUÁRIOS
        post("/cadastrar", (request, response) -> {
            
            String nome = request.queryParams("nome");
            String email = request.queryParams("email");
            String senha = request.queryParams("senha");

            Usuario novoUsuario = new Usuario(nome, email, senha);

            UserDAO userDAO = new UserDAO();
            userDAO.cadastrar(novoUsuario);

            response.redirect("/Login.html");  
            return null;
        });

        post("/login", (request, response) -> {
            String usuario = request.queryParams("usuario");
            String senha = request.queryParams("senha");
        
            UserDAO userDAO = new UserDAO();
            Usuario user = userDAO.verificarCredenciais(usuario, senha);
        
            if (user != null) {
                response.status(200);
                response.type("application/json");
                return "{\"usuario\": \"" + user.getNome() + "\", \"email\": \"" + user.getEmail() + "\"}";
            } else {
                response.status(401);
                response.type("application/json");
                return "{\"error\": \"Login falhou\"}";
            }
        });
        
        get("/Login.html", (request, response) -> {
            String error = request.queryParams("error");
            if (error != null) {
                return "<h1>Login falhou! Por favor, tente novamente.</h1>";
            }
            return "<h1>Por favor, faça login.</h1>";
        });

        // CADASTRO DO GRUPO
        post("/cadastrarGrupo", (req, res) -> {
            res.type("application/json"); 
            
            String requestBody = req.body(); 
            
            
            Gson gson = new Gson();
            Grupo novoGrupo = gson.fromJson(requestBody, Grupo.class); 

            GrupoDAO grupoDAO = new GrupoDAO(); 

            try {
                boolean sucesso = grupoDAO.cadastrar(novoGrupo); 
                
                if (sucesso) {
                    res.status(201); 
                    return gson.toJson(novoGrupo); 
                } else {
                    res.status(500); 
                    return "{\"message\":\"Erro ao cadastrar o grupo.\"}"; 
                }
            } catch (Exception e) {
                res.status(500); 
                return "{\"message\":\"Erro ao processar a solicitação.\"}"; 
            } finally {
                grupoDAO.finalize(); 
            }
        });
    }
}
