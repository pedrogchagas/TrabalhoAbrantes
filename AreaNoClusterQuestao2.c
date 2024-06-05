# Usando o programa de PI como base para calcular a área no cluster da Questão 2

#include "mpi.h"
#include <stdio.h>
#include <math.h>
 
int main(int argc, char *argv[]) {
    int n, meu_id, num_procs, i;
    double meu_resultado, resultado, h, soma, x;
    double tempo_inicio, tempo_fim;
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_id);
 
    while (1) {
        if (meu_id == 0) {
            printf("Digite o número de intervalos: (0 para sair) ");
            scanf("%d", &n);
        }
 
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
 
        if (n == 0)
            break;
        else {
            tempo_inicio = MPI_Wtime();
            h = 1.0 / (double) n;
            soma = 0.0;
            for (i = meu_id + 1; i <= n; i += num_procs) {
                x = h * ((double) i - 0.5);
                soma += (x * x + x);
            }
            meu_resultado = h * soma;
            MPI_Reduce(&meu_resultado, &resultado, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
            tempo_fim = MPI_Wtime();
 
            if (meu_id == 0) {
                printf("Valor aproximado da integral ∫ (x^2 + x) dx: %.16f\n", resultado);
                printf("Tempo gasto: %f segundos\n", tempo_fim - tempo_inicio);
            }
        }
    }
 
    MPI_Finalize();
    return 0;
}