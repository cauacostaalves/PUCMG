package Teste;

import dao.DAO;
import dao.UserDAO;
import model.Usuario;

public class javaTeste {
    public static void main(String[] args) throws Exception {
    	 DAO dao = new DAO();
    	    if (dao.conectar()) {
    	        System.out.println("Conexão bem-sucedida!");
    	    } else {
    	        System.out.println("Falha na conexão.");
    	    }
        UserDAO userDAO = new UserDAO();
       
        Usuario novoUsuario = new Usuario(0, "teste@example.com", "senha123", "Teste", "Endereço Teste");

        boolean resultado = userDAO.insert(novoUsuario);

        if (resultado) {
            System.out.println("Usuário adicionado com sucesso! ID: " + novoUsuario.getId());
        } else {
            System.out.println("Falha ao adicionar usuário.");
        }
    }
}
