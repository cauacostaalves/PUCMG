package dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import model.Usuario;

public class UserDAO extends DAO {
    
    // Construtor que já conecta ao banco
    public UserDAO() {
        super();  // Chama o construtor da classe DAO
        conectar();  // Conecta automaticamente ao banco ao criar uma instância de UserDAO
    }

    // Método que será chamado quando o objeto for destruído, fechando a conexão
    public void finalize() {
        close();  // Fecha a conexão quando o objeto é finalizado
    }

    // Método para cadastrar um novo usuário no banco de dados
    public boolean cadastrar(Usuario usuario) throws Exception {
        // SQL de inserção
        String sql = "INSERT INTO usuario (nome, email, senha) VALUES (?, ?, ?)";

        try (PreparedStatement stmt = conexao.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS)) {
            // Define os parâmetros da query
            stmt.setString(1, usuario.getNome());
            stmt.setString(2, usuario.getEmail());
            stmt.setString(3, DAO.toMD5(usuario.getSenha()));  // Criptografando a senha com MD5 (ou SHA-256)

            // Executa a query e verifica se o usuário foi inserido
            int affectedRows = stmt.executeUpdate();

            // Verifica se o usuário foi inserido e tenta recuperar o ID gerado
            if (affectedRows > 0) {
                try (ResultSet generatedKeys = stmt.getGeneratedKeys()) {
                    if (generatedKeys.next()) {
                        usuario.setId(generatedKeys.getInt(1));  // Atribui o ID gerado ao usuário
                        System.out.println("Usuário cadastrado com sucesso, ID: " + usuario.getId());
                    } else {
                        throw new SQLException("Falha ao criar o usuário, nenhum ID foi obtido.");
                    }
                }
            }
            return true;
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        } finally {
            close();  // Fecha a conexão
        }
    }
}