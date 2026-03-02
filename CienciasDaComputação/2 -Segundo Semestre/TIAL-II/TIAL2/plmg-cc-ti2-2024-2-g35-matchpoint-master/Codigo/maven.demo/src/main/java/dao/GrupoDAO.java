package dao;

import model.Grupo;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class GrupoDAO extends DAO {

    public GrupoDAO() {
        super();
        conectar();
    }

    public void finalize() {
        close();
    }

    // Função para cadastrar um grupo no banco de dados
    public boolean cadastrar(Grupo grupo) {
        boolean status = false;
        try {
            
            String sql = "INSERT INTO grupos (nome, data, horario, descricao, local, id_esporte, jogadoresmax, criador) "
                       + "VALUES (?, ?, ?, ?, ?, ?, ?, ?) RETURNING id";
            
            PreparedStatement stmt = conexao.prepareStatement(sql);
            
            stmt.setString(1, grupo.getNome());
            stmt.setString(2, grupo.getData());
            stmt.setString(3, grupo.getHorario());
            stmt.setString(4, grupo.getDescricao());
            stmt.setString(5, grupo.getLocal());
            stmt.setInt(6, grupo.getId_esporte());
            stmt.setInt(7, grupo.getJogadoresMax());
            stmt.setString(8, grupo.getCriador());

            ResultSet rs = stmt.executeQuery();
            if (rs.next()) {
                grupo.setId(rs.getInt("id"));  
            }
            status = true;
            
        } catch (SQLException e) {
            System.err.println("Erro ao cadastrar o grupo: " + e.getMessage());
        }
        return status;
    }
}
