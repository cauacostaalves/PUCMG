from fastapi import FastAPI, Depends, HTTPException
from sqlalchemy.orm import Session
import models, schemas
from database import engine, get_db

# Cria as tabelas no banco automaticamente
models.Base.metadata.create_all(bind=engine)

app = FastAPI(title="Sistema de Pedidos API")

# --- PRODUTOS (CRUD) ---
@app.post("/produtos/eletronicos", response_model=schemas.ProdutoCreate)
def cadastrar_eletronico(produto: schemas.ProdutoEletronicoCreate, db: Session = Depends(get_db)):
    db_produto = models.ProdutoEletronico(**produto.dict())
    db.add(db_produto)
    db.commit()
    db.refresh(db_produto)
    return db_produto

@app.get("/produtos")
def listar_produtos(db: Session = Depends(get_db)):
    return db.query(models.Produto).all()

# --- PEDIDOS E ITENS (Lógica Principal) ---
@app.post("/pedidos", response_model=schemas.PedidoResponse)
def cadastrar_pedido(itens: list[schemas.ItemCreate], db: Session = Depends(get_db)):
    # 1. Cria o pedido zerado
    novo_pedido = models.Pedido()
    db.add(novo_pedido)
    db.flush() # Obtém o ID do pedido gerado sem commitar a transação
    
    valor_total_pedido = 0.0
    
    # 2. Processa cada item
    for item_data in itens:
        produto = db.query(models.Produto).filter(models.Produto.id == item_data.produto_id).first()
        
        if not produto:
            raise HTTPException(status_code=404, detail=f"Produto {item_data.produto_id} não encontrado")
            
        if produto.estoque < item_data.quantidade:
            raise HTTPException(status_code=400, detail=f"Estoque insuficiente para o produto: {produto.nome}")
            
        # 3. Baixa no estoque
        produto.estoque -= item_data.quantidade
        
        # 4. Calcula o valor do item (quantidade * preço unitário)
        valor_item = produto.preco * item_data.quantidade
        valor_total_pedido += valor_item
        
        # 5. Cria o registro do Item
        novo_item = models.Item(
            quantidade=item_data.quantidade,
            valorItem=valor_item,
            pedido_id=novo_pedido.id,
            produto_id=produto.id
        )
        db.add(novo_item)
        
    # 6. Atualiza o valor total do pedido e salva tudo
    novo_pedido.valorTotal = valor_total_pedido
    db.commit()
    db.refresh(novo_pedido)
    
    return novo_pedido

@app.get("/pedidos/{pedido_id}")
def consultar_pedido(pedido_id: int, db: Session = Depends(get_db)):
    pedido = db.query(models.Pedido).filter(models.Pedido.id == pedido_id).first()
    if not pedido:
        raise HTTPException(status_code=404, detail="Pedido não encontrado")
    
    # Traz os itens junto para visualização
    itens = db.query(models.Item).filter(models.Item.pedido_id == pedido_id).all()
    return {"pedido": pedido, "itens": itens}