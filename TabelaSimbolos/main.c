#include <stdio.h>
#include "simbolos.h"

int main() {
    // Inserindo símbolos simulando declarações no compilador
    inserirSimbolo("x", "int", "global", 1);
    inserirSimbolo("y", "void", "main", 5);
    inserirSimbolo("a", "int", "func1", 10);
    inserirSimbolo("x", "int", "func1", 11); // 'x' em escopo diferente, ok

    // Tentativa de duplicata no mesmo escopo
    inserirSimbolo("x", "int", "global", 20); // Deve emitir aviso

    // Buscando simbolos
    Simbolo *s = buscarSimbolo("a", "func1");
    if (s) {
        printf("Encontrado simbolo: %s (tipo %s, escopo %s, linha %d)\n",
            s->nome, s->tipo, s->escopo, s->linha);
    } else {
        printf("Simbolo nao encontrado!\n");
    }

    // Imprimir tabela inteira
    imprimirTabela();

    liberarTabela();

    return 0;
}
