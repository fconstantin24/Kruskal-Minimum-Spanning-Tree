#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Muchie
{
    int start;
    int end;
    int cost;
};

struct Graf
{
    // n -> numarul de noduri
    // m -> numarul de muchii
    // muchii -> vector de muchii
    int n, m;
    struct Muchie* muchii;
};

struct Graf* creazaGraf(int n, int m)
{
    struct Graf* graf_nou = (struct Graf*)(malloc(sizeof(struct Graf)));
    graf_nou->n = n;
    graf_nou->m = m;

    graf_nou->muchii = (struct Muchie*)malloc(m * sizeof(struct Muchie));

    return graf_nou;
}

struct subset
{
    int parent;
    int rank;
};

int find(struct subset subsets[], int i)
{
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);

    return subsets[i].parent;
}

void Union(struct subset subsets[], int x, int y)
{
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;

    else
    {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

int myComp(const void* a, const void* b)
{
    struct Muchie* a1 = (struct Muchie*)a;
    struct Muchie* b1 = (struct Muchie*)b;
    return a1->cost > b1->cost;
}

void KruskalMST(struct Graf* graf)
{
    int n = graf->n;
    struct Muchie *result = (struct Muchie*)malloc(n * sizeof(struct Muchie));
    int e = 0;
    int i = 0;

    // sorteaza muchiile descrescator dupa cost
    qsort(graf->muchii, graf->m, sizeof(graf->muchii[0]), myComp);

    struct subset* subsets = (struct subset*)malloc(n * sizeof(struct subset));

    for (int v = 0; v < n; ++v)
    {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    while (e < n - 1 && i < graf->m)
    {
        struct Muchie next_muchii = graf->muchii[i++];

        int x = find(subsets, next_muchii.start);
        int y = find(subsets, next_muchii.end);
        if (x != y)
        {
            result[e++] = next_muchii;
            Union(subsets, x, y);
        }
    }
    printf("Arborele minim de acoperire consta in urmatoarele muchii\n");
    int minimumCost = 0;
    for (i = 0; i < e; ++i)
    {
        printf("%d - %d, cost = %d\n", result[i].start + 1, result[i].end + 1, result[i].cost);
        minimumCost += result[i].cost;
    }
    printf("Costul minim al arborelui de acoperire : %d", minimumCost);

    free(subsets);
    free(result);
    return;
}

int main()
{
    int n;
    int m;

    FILE *input_file = fopen("fin.txt", "r");
    fscanf(input_file, "%d %d", &n, &m);
    struct Graf* graf = creazaGraf(n, m);
    for (int i = 0; i < m; i++)
    {
        int start;
        int end;
        int cost;
        fscanf(input_file, "%d %d %d", &start, &end, &cost);

        graf->muchii[i].start = start - 1;
        graf->muchii[i].end = end - 1;
        graf->muchii[i].cost = cost;
    }
    fclose(input_file);

    KruskalMST(graf);
    free(graf->muchii);
    free(graf);

    return 0;
}
