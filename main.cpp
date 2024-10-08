/**
 * @file main.cpp
 * 
 * @brief Test d'uso della classe bst templata
 */
#include <iostream>

#include "bst.hpp"

/**
 * @brief Funtore di ordinamento tra tipi interi
 * 
 * Ordina due interi in ordine crescente.
 */
struct compare_int
{
    bool operator()(int a, int b) const
    {
        return a < b;
    }
};

/**
 * @brief Funtore di uguaglianza tra tipi interi
 * 
 * Valuta l'uglianza tra due interi.
 */
struct equal_int
{
    bool operator()(int a, int b) const
    {
        return a == b;
    }
};

/**
 * @brief Funtore di ordinamento tra tipi char
 * 
 * Ordina due char in ordine crescente.
 */
struct compare_char
{
    bool operator()(char a, char b) const
    {
        return a < b;
    }
};

/**
 * @brief Funtore di uguaglianza tra tipi char
 * 
 * Valuta l'uglianza tra due char.
 */
struct equal_char
{
    bool operator()(char a, char b) const
    {
        return a == b;
    }
};

/**
 * @brief Funtore di ordinamento tra tipi team
 * 
 * Ordina due team in ordine crescente.
 */
struct team
{
    std::string name;
    int position;

    team(std::string n, int p) : name(n), position(p) {}
};

/**
 * @brief Funtore di ordinamento tra tipi team
 * 
 * Ordina due team in ordine crescente.
 */
struct compare_team
{
    bool operator()(team a, team b) const
    {
        return a.position < b.position;
    }
};

/**
 * @brief Funtore di uguaglianza tra tipi team
 * 
 * Valuta l'uglianza tra due team.
 */
struct equal_team
{
    bool operator()(team a, team b) const
    {
        return a.position == b.position;
    }
};

/**
 * @brief Operatore di output per team
 * 
 * Stampa il nome e la posizione della squadra.
 */
std::ostream &operator<<(std::ostream &os, const team &t)
{
    std::cout << t.position << ") " << t.name;
    return os;
}


// typedef della classe su interi di comodo
typedef bst<int, compare_int, equal_int> bst_int;

// typedef della classe su char di comodo
typedef bst<char, compare_char, equal_char> bst_char;

/**
 * @brief Funzione che esegue una serie di operazioni base diverse istanze di classi.
 * 
 * La funzione crea diverse istanze di classi bst_int, bst_char e team,
 * e esegue una serie di operazioni su di esse.
 */
void metodi_fondamentali()
{
    bst_int b;
    std::cout << b << std::endl;
    b.add(5);
    b.add(8);
    std::cout << b << std::endl;
    b.add(9);
    std::cout << b << std::endl;
    b.add(10);
    b.add(12);
    std::cout << b << std::endl;
    b.add(11);
    std::cout << b << std::endl;

    bst_int bi(b);
    std::cout << bi << std::endl;
    bst_int bi2;
    bi2 = bi;
    std::cout << bi2 << std::endl;


    bst_char bc('b');
    std::cout << bc << std::endl;
    bc.add('c');
    bc.add('a');
    std::cout << bc << std::endl;
    bst_char bc1(bc);
    std::cout << bc1 << std::endl;
    bst_char bc2;
    bc2 = bc;
    std::cout << bc2 << std::endl;

    team t1("Atalanta", 4);
    team t2("Inter", 1);
    team t3("Roma", 6);
    team t4("Milan", 2);
    bst<team, compare_team, equal_team> bt;
    bt.add(t1);
    bt.add(t2);
    bt.add(t3);
    bt.add(t4);
    std::cout << bt << std::endl;
    bst<team, compare_team, equal_team> bt1(bt);
    std::cout << bt1 << std::endl;
    bst<team, compare_team, equal_team> bt2;
    bt2 = bt;
    std::cout << bt2 << std::endl;
}

/**
 * @brief Funzione che esegue una serie di operazioni avanzate utilizzando diverse istanze di classi.
 * 
 * La funzione crea diverse istanze di classi bst_int, bst_char e team,
 * e esegue una serie di operazioni avanzate su di esse.
 */
void utilizzo()
{
    int arr[5] = {5, 4, 3, 2, 1};
    bst_int bi(arr, arr + 5);
    std::cout << "Size: " << bi.size() << std::endl;
    std::cout << bi << std::endl;
    std::cout << "Find 3: " << bi.find(3) << std::endl;
    std::cout << "Find 6: " << bi.find(6) << std::endl;

    char carr[5] = {'e', 'd', 'c', 'b', 'a'};
    bst_char bci(carr, carr + 5);
    std::cout << bci << std::endl;
    std::cout << "Size: " << bci.size() << std::endl;
    std::cout << "Find 'c': " << bci.find('c') << std::endl;
    std::cout << "Find 'f': " << bci.find('f') << std::endl;
    std::cout << "Find 0: " << bci.find(0) << std::endl;

    team t1("Juventus", 3);
    team t2("Inter", 1);
    team tarr[2] = {team("Bologna", 5), team("Milan", 2)};
    bst<team, compare_team, equal_team> bt(tarr, tarr + 2);
    std::cout << bt << std::endl;
    bt.add(t1);
    std::cout << "Size: " << bt.size() << std::endl;
    std::cout << "Find Juventus: " << bt.find(t1) << std::endl;
    std::cout << "Find Inter: " << bt.find(team("Inter", 1)) << std::endl;
    std::cout << "Find Inter: " << bt.find(t2) << std::endl;
    // std::cout << "Find Bologna: " << bt.find("Bologna") << std::endl;
    // Errore perchè find cerca nell'albero
    // il value del nodo, in questo caso il value è team non name
}

/**
 * @brief Funzione che esegue una serie di operazioni tramite gli iteratori
 * 
 * La funzione crea diverse istanze di classi bst_int, bst_char e team,
 * e esegue una serie di operazioni sugli iteratori delle classi.
 */
void iteratori()
{
    int arr[7] = {57, 22, 77, 11, 42, 65, 90};
    bst_int bi(arr, arr + 7);
    std::cout << bi << std::endl;
    bst_int::const_iterator i, ie;
    for (i = bi.begin(), ie = bi.end(); i != ie; i++)
    {
        std::cout << *i << std::endl;
    }

    char carr[7] = {'e', 'd', 'c', 'b', 'a', 'f', 'g'};
    const bst_char bci(carr, carr + 7);
    std::cout << bci << std::endl;
    bst_char::const_iterator ci, ce;
    for (ci = bci.begin(), ce = bci.end(); ci != ce; ci++)
    {
        std::cout << *ci << std::endl;
    }

    team tarr[4] = {team("Juventus", 3), team("Inter", 1), team("Milan", 2), team("Roma", 6)};
    const bst<team, compare_team, equal_team> bt(tarr, tarr + 4);
    std::cout << bt << std::endl;
    bst<team, compare_team, equal_team>::const_iterator ti, te;
    for (ti = bt.begin(), te = bt.end(); ti != te; ti++)
    {
        std::cout << *ti << std::endl;
    }
}

/**
 * @brief Funzione che esegue una serie di operazioni di creazione di sottoalberi
 * 
 * La funzione crea diverse istanze di classi bst_int, bst_char e team,
 * e crea dei sottoalberi a partire da un nodo specifico.
 */
void subtree()
{
    int arr[7] = {57, 22, 77, 11, 42, 65, 90};
    bst_int bi(arr, arr + 7);
    std::cout << bi << std::endl;
    bst_int bi1 = bi.subtree(57);
    std::cout << bi1 << std::endl;
    bst_int bi2 = bi.subtree(22);
    std::cout << bi2 << std::endl;
    bst_int bi3 = bi.subtree(90);
    std::cout << bi3 << std::endl;
    bst_int bi4 = bi.subtree(104);
    std::cout << bi4 << std::endl;

    char carr[7] = {'e', 'd', 'c', 'b', 'a', 'f', 'g'};
    bst_char bci(carr, carr + 7);
    std::cout << bci << std::endl;
    bst_char bci1 = bci.subtree('e');
    std::cout << bci1 << std::endl;
    bst_char bci2 = bci.subtree('d');
    std::cout << bci2 << std::endl;
    bst_char bci3 = bci.subtree('g');
    std::cout << bci3 << std::endl;
    bst_char bci4 = bci.subtree('z');
    std::cout << bci4 << std::endl;

    team tarr[4] = {team("Juventus", 3), team("Inter", 1), team("Milan", 2), team("Roma", 6)};
    bst<team, compare_team, equal_team> bt(tarr, tarr + 4);
    std::cout << bt << std::endl;
    bst<team, compare_team, equal_team> bt1 = bt.subtree(team("Juventus", 3));
    std::cout << bt1 << std::endl;
    bst<team, compare_team, equal_team> bt2 = bt.subtree(team("Inter", 1));
    std::cout << bt2 << std::endl;
    bst<team, compare_team, equal_team> bt3 = bt.subtree(team("Roma", 6));
    std::cout << bt3 << std::endl;
    bst<team, compare_team, equal_team> bt4 = bt.subtree(team("Napoli", 5));
    std::cout << bt4 << std::endl;
    bst<team, compare_team, equal_team> bt5 = bt.subtree(team(
}

/**
 * @brief Funtore di numeri interi pari
 * 
 * Valuta se un numero intero è pari.
 */
struct is_even
{
    bool operator()(int a) const
    {
        return a % 2 == 0;
    }
};

/**
 * @brief Funtore di numeri interi maggiori di 50
 * 
 * Valuta se un numero intero è maggiore di 50.
 */
struct grt50
{
    bool operator()(int a) const
    {
        return a > 50;
    }
};

/**
 * @brief Funtore di vocali
 * 
 * Valuta se un carattere è una vocale.
 */
struct is_vowel
{
    bool operator()(char a) const
    {
        return a == 'a' || a == 'e' || a == 'i' || a == 'o' || a == 'u';
    }
};

/**
 * Funzione che stampa le squadre che andranno in champions league in base alla loro posizione.
 */
struct cmpl
{
    bool operator()(team a) const
    {
        return a.position < 5;
    }
};

/**
 * Funzione che stampa gli elementi dell'albero che soddisfano il predicato P.
 */
void printif()
{
    int arr[7] = {57, 22, 77, 11, 42, 65, 90};
    bst_int bi(arr, arr + 7);
    std::cout << bi << std::endl;
    std::cout << "Even numbers:" << std::endl;
    printIF(bi, is_even());
    std::cout << "Greater than 50:" << std::endl;
    printIF(bi, grt50());

    char carr[7] = {'e', 'd', 'c', 'b', 'a', 'f', 'g'};
    bst_char bci(carr, carr + 7);
    std::cout << bci << std::endl;
    std::cout << "Vowels:" << std::endl;
    printIF(bci, is_vowel());

    team tarr[4] = {team("Juventus", 3), team("Inter", 1), team("Milan", 2), team("Roma", 6)};
    bst<team, compare_team, equal_team> bt(tarr, tarr + 4);
    std::cout << bt << std::endl;
    std::cout << "Club in champions league:" << std::endl;
    printIF(bt, cmpl());
}

int main(int argc, char *argv[])
{
    metodi_fondamentali();

    utilizzo();

    iteratori();

    subtree();

    printif();

    return 0;
}