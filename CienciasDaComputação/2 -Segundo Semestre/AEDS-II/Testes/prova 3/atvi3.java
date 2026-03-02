
public boolean pesquisar(String nome) {
    return pesquisarRec(nome, raiz);
}

public boolean pesquisarRec(String nome, No i){
    if(nome.charAt(0) == i.character){
        return pesquisarT1(nome,i.t1);
    }else if(nome.charAt(0) < i.character){
        return pesquisarRec(nome, i.esq);
    }else{
        return pesquisarRec(nome, i.dir);
    }
}

public boolean pesquisarT1(String nome, T1 t1){
    char 
}

public boolean pesquisarT2(String nome, T2 t2){
    for(CelulaT2 x: t2.celulalast2){
        for(Celula i = x.inicio; i != x.fim; i = i.prox){
            if(i.palavra.equals(nome)){
                return true;
            }
        }
    }
    return false;
}