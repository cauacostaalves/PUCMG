public boolean isMax(double valor){
    int altura = GetAltura(raiz);
    int qtdnos = QtdNos(raiz);
    if(altura <= (valor * Math.log2(qtdnos))){
        return true;
    }else{
        return false;
    }
}

public int GetAltura(No i){
    if(i == null){
        return -1;
    }else{
        return 1 + Math.max(GetAltura(i.esq), GetAltura(i.dir));
    }
}

public int QtdNos(No i){
    if(i == null){
        return 0;
    }else{
        return 1 + QtdNos(i.esq) + QtdNos(i.dir)
    }
}
