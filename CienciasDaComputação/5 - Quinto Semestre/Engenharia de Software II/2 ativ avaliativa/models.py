from sqlalchemy import Column, Integer, String, Float, ForeignKey, DateTime, Date
from sqlalchemy.orm import relationship
from datetime import datetime
from database import Base

class Produto(Base):
    __tablename__ = "produtos"
    id = Column(Integer, primary_key=True, index=True)
    nome = Column(String(100), index=True)
    preco = Column(Float)
    estoque = Column(Integer)
    tipo = Column(String(50)) # Coluna discriminadora para herança

    __mapper_args__ = {
        "polymorphic_identity": "produto",
        "polymorphic_on": tipo,
    }

class ProdutoEletronico(Produto):
    __tablename__ = "produtos_eletronicos"
    id = Column(Integer, ForeignKey("produtos.id"), primary_key=True)
    voltagem = Column(String(10))

    __mapper_args__ = {
        "polymorphic_identity": "eletronico",
    }

class ProdutoPerecivel(Produto):
    __tablename__ = "produtos_pereciveis"
    id = Column(Integer, ForeignKey("produtos.id"), primary_key=True)
    dataValidade = Column(Date)

    __mapper_args__ = {
        "polymorphic_identity": "perecivel",
    }

class Pedido(Base):
    __tablename__ = "pedidos"
    id = Column(Integer, primary_key=True, index=True)
    data = Column(DateTime, default=datetime.utcnow)
    valorTotal = Column(Float, default=0.0)
    
    # Relacionamento OneToMany
    itens = relationship("Item", back_populates="pedido", cascade="all, delete-orphan")

class Item(Base):
    __tablename__ = "itens"
    codigoItem = Column(Integer, primary_key=True, index=True)
    quantidade = Column(Integer)
    valorItem = Column(Float)
    
    # Foreign Keys
    pedido_id = Column(Integer, ForeignKey("pedidos.id"))
    produto_id = Column(Integer, ForeignKey("produtos.id"))
    
    # Relacionamentos ManyToOne
    pedido = relationship("Pedido", back_populates="itens")
    produto = relationship("Produto")