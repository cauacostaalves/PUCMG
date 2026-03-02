package dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import model.Usuario;

public class UserDAO extends DAO {
    
    public UserDAO() {
        super();  
        conectar();  
    }


    public void finalize() {
        close();  
    }

    public boolean cadastrar(Usuario usuario) throws Exception {
     
        String sql = "INSERT INTO usuario (nome, email, senha) VALUES (?, ?, ?)";

        try (PreparedStatement stmt = conexao.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS)) {

            stmt.setString(3, usuario.getNome());
            stmt.setString(1, usuario.getEmail());
            stmt.setString(2, usuario.getSenha()); 

            int affectedRows = stmt.executeUpdate();

            if (affectedRows > 0) {
                try (ResultSet generatedKeys = stmt.getGeneratedKeys()) {
                    if (generatedKeys.next()) {
                        usuario.setId(generatedKeys.getInt(1));  
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
            close(); 
        }
    }

    
    public Usuario verificarCredenciais(String nome, String senha) {
        Usuario user = null;
        String sql = "SELECT * FROM usuario WHERE nome = ? AND senha = ?";

        try (PreparedStatement stmt = conexao.prepareStatement(sql)) {
           
            stmt.setString(1, nome);
            stmt.setString(2, senha);

            ResultSet rs = stmt.executeQuery();

            if (rs.next()) {
                user = new Usuario();
                user.setId(rs.getInt("id"));
                user.setNome(rs.getString("nome"));
                user.setEmail(rs.getString("email"));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return user; 
    }

    public int verificarNome(String nome) {
        Usuario user = null;
        String sql = "SELECT * FROM usuario WHERE nome = ?";

        try (PreparedStatement stmt = conexao.prepareStatement(sql)) {
           
            stmt.setString(1, nome);

            ResultSet rs = stmt.executeQuery();

            if (rs.next()) {
                user = new Usuario();
                user.setId(rs.getInt("id"));
                user.setNome(rs.getString("nome"));
                user.setEmail(rs.getString("email"));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return user.getId(); 
    }
}
