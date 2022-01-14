/*


    TEMA 4 - Varga Alexandra-Ioana - grupa 30223



*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Profiler.h"

using namespace std;

Profiler p("TEMA 4");
/*


Complexitate:  O(n*log k)
Justificare:
Se realizeaza interclasarea a k liste ordonate cu cate n/k  elemente => n elemente ordonate in final
!!! Cazul pt n-variabil si k fix (5, 10, 100) => Complexitate O(n)
Folosim minHeap-ul construit bottom-up - de complexitate O(n*log k)
!!! Cazul pentru n=10000 cu k-variabil => Complexitate O(log k)

COMPLEXITATE FINALA PER ALGORITM - O(n*log k)

Pentru un numar mai mare de liste se fac mai multe operatii deoarece algoritmul de heapify este executat de mai multe ori
pe un numar mai mare de elemente astfel ca:

Din reprezentarea grafica se observa:
-pentru 5 liste algoritmul este mai eficient decat pentru 10 si 100 liste
-pentru 10 liste algoritmul este mai eficient decat pentru 100 de liste dar mai putin eficient decat pentru 5 liste
-pentru 100 liste algoritmul este mai ineficient decat pentru 5 si 10 liste

*/

typedef struct node 
{
    node* next;
    int key;
}
NodeT;
/*
 
 Structura care reprezinta un vector care stocheaza si numarul listei din care se vor extrage elemente

*/
typedef struct vect 
{
    int val;
    int index;
}
Vect;

/*

Inserare la sfarsitul listei

*/
void insertLast(NodeT** first, int givenKey) 
{    
    NodeT* p = (NodeT*)malloc(sizeof(NodeT));
    p->key = givenKey;
    p->next = NULL;

    if ((*first) != NULL)
    {
        NodeT* q = (NodeT*)malloc(sizeof(NodeT));
        q = (*first);
        while (q->next != NULL)
            q = q->next;
        q->next = p;
        q = p;
    }
    else
    {
        *first = p;
    }
}
/*
 
 Stergere la inceputul listei

 */
int deleteFirst(NodeT** first)
{
    int val;
    NodeT* p = *first;
    (*first) = (*first)->next;
    val = p->key;
    free(p);

    return val;
}
//afisare lista 
void printList(NodeT* first)
{
    while (first != NULL)
    {
        printf("%d ", first->key);
        first = first->next;
    }
    printf("\n");
}
/*
 
 Initializare vector de liste vida cu valori random
 Daca mode=1, in vectorul de liste arr punem k liste sortate fiecare continand n/k elemente
 Daca mode=2, in vectorul de liste arr punem k liste cu n elemente luand cate un elemnt si
 introducandu-l in cate o lista pana ramanem fara elemente de introdus - pentru caz cu 10000 elemente

 */
void valoriListe(NodeT* arr[], int k, int n, int mode) 
{
    if (mode == 1) 
    {
        int a[10000];
        for (int i = 0; i < k; i++) {
            arr[i] = NULL;
        }
        for (int i = 0; i < k; i++) {
            FillRandomArray(a, n / k, 0, n * k, false, 1);
            for (int j = 0; j < n / k; j++) {
                insertLast(&arr[i], a[j]);
            }
        }
    }
    else if (mode == 2) 
    {
        int a[10000];
        FillRandomArray(a, n, 0, n * k, false, 1);
        for (int i = 0; i < k; i++) {
            arr[i] = NULL;
        }
        int index = 0;
        for (int i = 0; i < n; i++) {
            insertLast(&arr[index], a[i]);
            index++;
            if (index == k)
                index = 0;
        }
    }
}
/*

Afisare vector de liste

*/
void printListeVector(NodeT* arr[],int k)
{
    for (int i = 0; i < k; i++)
    {
        NodeT* p = (NodeT*)malloc(sizeof(NodeT));
        p = arr[i];
        printList(p);
    }
    printf("\n");
}
/*
 
 Eliberarea listei result dupa ce am interclasat listele si dorim sa interclasam din nou alte liste
 pentru a nu se introduca in continuarea numerelor deja introduse

 */
void deleteList(NodeT** first)
{
    NodeT* p = *first;
    NodeT* next;
    while (p)
    {
        next = p->next;
        free(p);
        p = next;
    }
    *first = NULL;
}
/*
Pentru a selecta de fiecare data elementul minim din listele ce trebuie interclasate la fiecare pas folosim 

*/
void heapify(Vect* vect[], int k, int i, int size, const char *s) 
{
    Operation operatii = p.createOperation(s, size);
    int smallest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < k && vect[l]->val < vect[smallest]->val)
        smallest = l;

    if (r < k && vect[r]->val < vect[smallest]->val)
        smallest = r;

    if (smallest != i)
    {
        operatii.count(3);

        int aux = vect[i]->val;
        vect[i]->val = vect[smallest]->val;
        vect[smallest]->val = aux;

        int auxi = vect[i]->index;
        vect[i]->index = vect[smallest]->index;
        vect[smallest]->index = auxi;

        heapify(vect, k, smallest, size,s);
    }
}

void interclasareCuHeap(int k, int n, int size, int mode, const char *s) 
{
    Operation operatii = p.createOperation(s, size);
    NodeT* arr[10000];
    NodeT* result = NULL;
    Vect* vect[10000];
    //alocare dinamica structura
    for (int i = 0; i < k; i++)
        vect[i] = (Vect*)malloc(sizeof(Vect));
    //initializare valori vector de liste
    valoriListe(arr, k, n, mode);
    //printListeVector(arr, k);
    printListeVector(arr, k);
    //extragem primul element din fiecare lista
    for (int i = 0; i < k; i++)
    {
        operatii.count();
        vect[i]->val = deleteFirst(&arr[i]);
        vect[i]->index = i;
    }
    //construiesc minHeap de tipBottom up
    for (int i = k / 2 - 1; i >= 0; i--)
    {
        heapify(vect, k, i, size, s);
    }
    //construiesc la fiecare pas minHeap-ul si extrag minimele ce vor fi adaugate la vectorul final
    //interclasat
    for (int i = 0; i < n; i++) 
    {
        //pentru a determina de fiecare data elementul minim 
        heapify(vect, k, 0, size,s);
        //punem elementul de pe pozitia 0 din vect in rezultatul final, acesta fiind cel mai mic element,
        ///apoi din lista din care facea parte minimul luam urmatorul element si il punem in vect
        insertLast(&result, vect[0]->val);
        if (arr[vect[0]->index] != NULL)
        {
            operatii.count();
            vect[0]->val = deleteFirst(&arr[vect[0]->index]);
        }
        else
        {
            operatii.count();
            vect[0]->val = INT_MAX;
        }
    }
    
    printList(result);
    deleteList(&result);
}
void demo()
{
    int n = 25;
    int k = 5;
    NodeT* arr[10000];
    interclasareCuHeap(k, n, n, 1,"Miau");
    interclasareCuHeap(k, n, n, 1,"Miau");
}
void perf()
{

    for (int n = 100; n <= 10000; n += 100) 
    {
        interclasareCuHeap(5, n, n, 1,"TOTAL_5_LISTE");
    }
    for (int n = 100; n <= 10000; n += 100) 
    {
        interclasareCuHeap(10, n, n, 1,"TOTAL_10_LISTE");
    }
    for (int n = 100; n <= 10000; n += 100) 
    {
        interclasareCuHeap(100, n, n, 1,"TOTAL_100_LISTE");
    }
    p.createGroup("TOTAL_OP_N_VARIABIL", "TOTAL_5_LISTE", "TOTAL_10_LISTE", "TOTAL_100_LISTE");

    for (int k = 10; k <= 500; k += 10) 
    {
        interclasareCuHeap(k, 10000, k, 2, "TOTAL_OP_K_VARIABIL_PT_10000_ELEMENTE");
    }
    p.showReport();

}
int main()
{
    demo();
    //perf();

    return 0;
}
