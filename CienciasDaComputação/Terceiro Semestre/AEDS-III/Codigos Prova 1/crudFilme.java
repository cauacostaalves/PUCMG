import java.util.*;

class Filme{
    int id;
    public Filme(int i){
        this.id = i;
    }
    public Filme(){
        this.id = 0;
    }

    public Byte toByteArray(){
        
    } 
    public int setID(int id){
        this.id = id;
        return id;
    }

    public int create(Filme filme) throws Exception {

        //Lê o cabeçalho
        arq.seek(0);
        int id = arq.readInt() + 1; // pegando o ultimo id e adicionando 1 do novo filme
        int qtdeRegistros = arq.readInt(); 

        // Grava o novo registro
        filme.setID(id);
        arq.seek(arq.length()); // indo para o final do arq 
        byte [] x = filme.toByteArray(); // passando o conteudo do filme para byte
        int tam = (int) x.length;
        arq.write(' '); //lapide
        arq.writeInt(tam); // tamanho do registro
        arq.write(x); // o registro do filme

        // Atualizar o cabeçalho
        arq.seek(0);
        arq.writeInt(id); // ultimo id usado
        arq.writeInt(qtdeRegistros + 1); // registro de filmes validos 

        return id;
    }
}

public class crudFilme{
    public static void main(String[] args){
        Scanner sc = new Scanner(System.in);
        
        sc.close();
    }
}