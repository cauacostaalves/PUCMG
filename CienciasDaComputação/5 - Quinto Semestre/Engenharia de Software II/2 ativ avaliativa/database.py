from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker, declarative_base

# Usando SQLite para rodar imediatamente sem configurar servidor de banco
SQLALCHEMY_DATABASE_URL = "sqlite:///./pedidos.db"

# O connect_args é necessário apenas para o SQLite no FastAPI
engine = create_engine(
    SQLALCHEMY_DATABASE_URL, connect_args={"check_same_thread": False}
)
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)

Base = declarative_base()

def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()