CREATE DATABASE sistema_pedidos;
USE sistema_pedidos;

CREATE TABLE produtos (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nome VARCHAR(100),
    preco FLOAT,
    estoque INT,
    tipo VARCHAR(50)
);

CREATE TABLE produtos_eletronicos (
    id INT PRIMARY KEY,
    voltagem VARCHAR(10),
    FOREIGN KEY(id) REFERENCES produtos(id)
);

CREATE TABLE produtos_pereciveis (
    id INT PRIMARY KEY,
    dataValidade DATE,
    FOREIGN KEY(id) REFERENCES produtos(id)
);

CREATE TABLE pedidos (
    id INT AUTO_INCREMENT PRIMARY KEY,
    data DATETIME,
    valorTotal FLOAT
);

CREATE TABLE itens (
    codigoItem INT AUTO_INCREMENT PRIMARY KEY,
    quantidade INT,
    valorItem FLOAT,
    pedido_id INT,
    produto_id INT,
    FOREIGN KEY(pedido_id) REFERENCES pedidos(id),
    FOREIGN KEY(produto_id) REFERENCES produtos(id)
);