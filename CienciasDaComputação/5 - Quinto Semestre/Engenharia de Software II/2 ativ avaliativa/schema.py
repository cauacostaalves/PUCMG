from pydantic import BaseModel
from typing import List, Optional
from datetime import date, datetime

# --- Produtos ---
class ProdutoCreate(BaseModel):
    nome: str
    preco: float
    estoque: int

class ProdutoEletronicoCreate(ProdutoCreate):
    voltagem: str

class ProdutoPerecivelCreate(ProdutoCreate):
    dataValidade: date

# --- Itens e Pedidos ---
class ItemCreate(BaseModel):
    produto_id: int
    quantidade: int

class PedidoResponse(BaseModel):
    id: int
    data: datetime
    valorTotal: float

    class Config:
        from_attributes = True