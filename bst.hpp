#ifndef BST_HPP
#define BST_HPP

#include <iterator>
#include <cstddef>
#include <iostream>
#include <cassert>
#include <algorithm>

/**
 * @brief Implementazione di un albero binario di ricerca.
 *
 * La classe `bst` implementa un albero binario di ricerca, in cui ogni nodo contiene un valore di tipo `T`.
 * L'ordine dei nodi è determinato dalla funzione di confronto `Comp` e dalla funzione di uguaglianza `Equal`.
 * L'albero supporta operazioni come l'aggiunta di un nodo, la ricerca di un valore e la stampa degli elementi.
 *
 * @tparam T Il tipo di valore contenuto nei nodi dell'albero.
 * @tparam Comp Il tipo di funzione di confronto per ordinare i nodi.
 * @tparam Equal Il tipo di funzione di uguaglianza per confrontare i valori dei nodi.
 */
template <typename T, typename Comp, typename Equal>
class bst
{
    /**
     * @brief Struttura che rappresenta un nodo di un albero binario di ricerca.
     *
     * @tparam T Il tipo di dato contenuto nel nodo.
     */
    struct node
    {
        T value;      //< valore del nodo
        node *left;   //< puntatore al nodo figlio sinistro
        node *right;  //< puntatore al nodo figlio destro
        node *parent; //< puntatore al nodo genitore

        /**
         * @brief Costruttore di default della struttura node.
         *
         * @post left == nullptr
         * @post right == nullptr
         * @post parent == nullptr
         */
        node() : left(nullptr), right(nullptr), parent(nullptr)
        {
#ifndef NDEBUG
            std::cout << "node::node()" << std::endl;
#endif
        };

        /**
         * @brief Costruttore della struttura node.
         *
         * @param v Il valore del nodo.
         * @param l Il puntatore al nodo figlio sinistro.
         * @param r Il puntatore al nodo figlio destro.
         * @param p Il puntatore al nodo genitore.
         *
         * @post value == v
         * @post left == l
         * @post right == r
         * @post parent == p
         */
        node(const T &v, node *l, node *r, node *p) : value(v), left(l), right(r), parent(p) {};

        /**
         * @brief Costruttore della struttura node.
         *
         * @param v Il valore del nodo.
         *
         * @post value == v
         * @post left == nullptr
         * @post right == nullptr
         * @post parent == nullptr
         */
        node(const T &v) : value(v), left(nullptr), right(nullptr), parent(nullptr) {};
    };

    node *_root;        //< puntatore alla radice dell'albero
    unsigned int _size; //< numero di nodi dell'albero
    Comp _compare;      //< funtore per il confronto tra i valori dei nodi
    Equal _equal;       //< funtore per l'uguaglianza tra i valori dei nodi

public:
    /**
     * @brief Classe che rappresenta un albero binario di ricerca.
     *
     * Questa classe implementa un albero binario di ricerca, in cui ogni nodo
     * contiene un valore e due puntatori ai suoi figli sinistro e destro.
     *
     * @post _root == nullptr
     * @post _size == 0
     */
    bst() : _root(nullptr), _size(0)
    {
#ifndef NDEBUG
        std::cout << "bst::bst()" << std::endl;
#endif
    };

    /**
     * @brief Costruttore della classe bst.
     *
     * Questo costruttore crea un nuovo albero binario di ricerca (bst)
     * con un nodo radice iniziale.
     *
     * @param value Il valore del nodo radice.
     *
     * @throw Eccezione standard in caso di errore nella creazione del nodo radice.
     */
    bst(const T &value) : _size(1)
    {
        try
        {
            _root = new node(value);
#ifndef NDEBUG
            std::cout << "bst::bst(node value " << value << " )" << std::endl;
#endif
        }
        catch (...)
        {
            destroy_tree(_root);
            throw;
        }
    };

    /**
     * @brief Copia ricorsivamente i nodi di un'altra istanza di bst nel bst corrente.
     *
     * @param c L'istanza di bst da cui copiare i nodi.
     * @param root Il nodo radice dell'albero da copiare.
     *
     * @throw Eccezione generica se si verifica un errore durante la copia.
     */
    void copyRic(bst &c, const node *root)
    {
        try
        {
            if (root != nullptr)
            {
                c.add(root->value);
                copyRic(c, root->left);
                copyRic(c, root->right);
            }
        }
        catch (...)
        {
            destroy_tree(_root);
            throw;
        }
    }

    /**
     * @brief Copy constructor
     *
     * Costruttore di copia della classe bst.
     *
     * @param other L'albero binario di ricerca da copiare.
     */
    bst(const bst &other) : _root(nullptr), _size(0)
    {
        try
        {
            copyRic(*this, other._root);
        }
        catch (...)
        {
            destroy_tree(_root);
            throw;
        }
#ifndef NDEBUG
        std::cout << "bst::bst(const bst &other)" << std::endl;
#endif
    }

    /**
     * @brief Operatore assegnazione
     *
     * Questo operatore assegna un'altra istanza di bst all'istanza corrente.
     *
     * @param other L'albero binario di ricerca da copiare.
     * @return reference all'istanza di bst corrente.
     */
    bst &operator=(const bst &other)
    {
        if (this != &other)
        {
            bst tmp(other);
            swap(tmp);
#ifndef NDEBUG
            std::cout << "bst::operator=(const bst &other)" << std::endl;
#endif
        }

        return *this;
    }

    /**
     * @brief Distruttore della classe bst.
     *
     * Questo distruttore si occupa di deallocare tutti i nodi dell'albero binario di ricerca.
     *
     * @post _root == nullptr
     */
    ~bst()
    {
        destroy_tree(_root);
        _size = 0;

#ifndef NDEBUG
        std::cout << "bst::~bst()" << std::endl;
#endif
    }

    /**
     * @brief Costruisce un albero binario di ricerca a partire da una sequenza di elementi.
     *
     * Questo costruttore crea un albero binario di ricerca a partire da una sequenza di elementi
     * definita dall'iteratore di inizio e l'iteratore di fine.
     *
     * @tparam Iter Il tipo dell'iteratore.
     * @param begin L'iteratore di inizio della sequenza.
     * @param end L'iteratore di fine della sequenza.
     *
     * @throw Eccezione generata durante l'inserimento degli elementi.
     */
    template <typename Iter>
    bst(Iter begin, Iter end) : _root(nullptr), _size(0)
    {
        try
        {
            Iter it = begin;
            for (; it != end; ++it)
            {
                add(*it);
            }
        }
        catch (...)
        {
            destroy_tree(_root);
            throw;
        }
#ifndef NDEBUG
        std::cout << "bst::bst(Iter begin, Iter end)" << std::endl;
#endif
    }

    /**
     * @brief Aggiunge un valore all'albero binario di ricerca.
     *
     * Questo metodo aggiunge un nuovo nodo contenente il valore specificato all'albero binario di ricerca.
     * Se il valore è già presente nell'albero, il nodo non viene aggiunto.
     *
     * @param value Il valore da aggiungere all'albero.
     *
     * @throw Eccezione generica se si verifica un errore durante l'aggiunta del nodo.
     */
    void add(const T &value)
    {
        try
        {
            node *temp = new node(value);
            if (_root == nullptr)
            {
                _root = temp;
                _size++;
#ifndef NDEBUG
                std::cout << "bst::add() root = " << value << std::endl;
#endif
            }
            else
            {
                node *curr = _root;
                node *parent;
                while (curr != nullptr)
                {
                    parent = curr;
                    if (_equal(value, curr->value))
                    {
#ifndef NDEBUG
                        std::cout << "bst::add() equal, delete value" << std::endl;
#endif
                        delete temp;
                        return;
                    }
                    if (_compare(value, curr->value))
                    {
                        curr = curr->left;
#ifndef NDEBUG
                        std::cout << "bst::add() go to the left" << std::endl;
#endif
                    }
                    else
                    {
                        curr = curr->right;
#ifndef NDEBUG
                        std::cout << "bst::add() go to the  right" << std::endl;
#endif
                    }
                }
                if (_compare(value, parent->value))
                {
                    temp->parent = parent;
                    parent->left = temp;
                }
                else
                {
                    temp->parent = parent;
                    parent->right = temp;
                }

                _size++;
#ifndef NDEBUG
                std::cout << "bst::add() add leaf = " << value << std::endl;
#endif
            }
        }
        catch (...)
        {
            destroy_tree(_root);
            throw;
        }
    }

    /**
     * @brief Scambia il contenuto di due alberi binari di ricerca.
     *
     * Questa funzione scambia il contenuto dell'albero binario di ricerca
     * corrente con un altro albero binario di ricerca specificato.
     *
     * @param other L'albero binario di ricerca con cui scambiare il contenuto.
     */
    void swap(bst &other)
    {
        std::swap(_root, other._root);
        std::swap(_size, other._size);
    }

    /**
     * @brief Restituisce la dimensione dell'albero.
     *
     * @return La dimensione dell'albero.
     */
    unsigned int size() const
    {
        return _size;
    }

    /**
     * @brief Trova un valore nell'albero binario di ricerca.
     *
     * Questo metodo cerca un valore specifico nell'albero binario di ricerca.
     * Restituisce true se il valore viene trovato, altrimenti restituisce false.
     *
     * @param value Il valore da cercare nell'albero.
     * @return True se il valore viene trovato, altrimenti false.
     */
    bool find(const T &value) const
    {
        node *curr = _root;
        while (curr != nullptr)
        {
            if (_equal(value, curr->value))
                return true;
            if (_compare(value, curr->value))
                curr = curr->left;
            else
                curr = curr->right;
        }
        return false;
    }

    /**
     * @brief Restituisce un sottoalbero con radice nel nodo contenente il valore specificato.
     *
     * Se il valore specificato non viene trovato nel BST, viene restituito un BST vuoto.
     *
     * @param value Il valore da cercare nel BST.
     * @return Un BST che rappresenta il sottoalbero con radice nel nodo contenente il valore specificato.
     * @throw Nessuna eccezione viene generata da questa funzione.
     */
    bst subtree(const T &value)
    {
        if (!find(value))
        {
#ifndef NDEBUG
            std::cout << "bst::subtree() value " << value << " not found" << std::endl;
#endif
            return bst();
        }
        else
        {
            bst b;
            node *curr = _root;
            while (!_equal(value, curr->value))
            {
                if (_compare(value, curr->value))
                    curr = curr->left;
                else
                    curr = curr->right;
            }
#ifndef NDEBUG
            std::cout << "bst::subtree() value " << value << " found" << std::endl;
#endif
            copyRic(b, curr);
            return b;
        }
    }

    /**
     * @brief Distrugge l'albero a partire dal nodo specificato.
     *
     * Questa funzione ricorsiva distrugge l'albero a partire dal nodo specificato.
     * Viene chiamata ricorsivamente per distruggere i sottoalberi sinistro e destro,
     * quindi imposta il puntatore del nodo a nullptr e dealloca la memoria.
     *
     * @param leaf Il nodo radice dell'albero da distruggere.
     *
     * @post _root == nullptr
     */
    void destroy_tree(node *leaf)
    {
        if (leaf != nullptr)
        {
            destroy_tree(leaf->left);
            destroy_tree(leaf->right);
            delete leaf;
        }
    }

    /**
     * @brief Stampa i valori dei nodi dell'albero in ordine.
     *
     * Questa funzione stampa i valori dei nodi dell'albero in ordine, visitando prima il sottoalbero sinistro,
     * poi il nodo corrente e infine il sottoalbero destro.
     *
     * @param root Il puntatore alla radice dell'albero.
     * @param os Lo stream di output su cui stampare i valori dei nodi.
     */
    static void print(node *root, std::ostream &os)
    {
        if (root != nullptr)
        {
            print(root->left, os);
            os << root->value << ' ';
            print(root->right, os);
        }
    }

    /**
     * Funzione GOBLALE che implementa l'operatore di stream.
     * La funzione è templata sull'albero binario di ricerca ed è messa
     * friend per accedere ai dati privati dell'albero.
     *
     * @param os Lo stream di output su cui stampare i valori dei nodi.
     * @param b L'albero binario di ricerca da stampare.
     * @return Lo stream di output su cui sono stati stampati i valori dei nodi.
     */
    friend std::ostream &operator<<(std::ostream &os, const bst &b)
    {
        node *curr = b._root;

        print(curr, os);

        return os;
    }

    /**
     * Classe che rappresenta un iteratore costante per la classe bst.
     * Fornisce un'interfaccia per iterare in modo costante sugli elementi di un oggetto bst.
     */
    class const_iterator
    {
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef const T *pointer;
        typedef const T &reference;

        /**
         * @brief Costruttore di default.
         */
        const_iterator() : n(nullptr) {}

        /**
         * @brief Costruttore di copia.
         *
         * @param other L'iteratore da copiare.
         */
        const_iterator(const const_iterator &other) : n(other.n) {}

        /**
         * @brief Operatore di assegnazione.
         *
         * @param other L'iteratore da assegnare.
         * @return Un riferimento a se stesso.
         */
        const_iterator &operator=(const const_iterator &other)
        {
            n = other.n;
            return *this;
        }

        /**
         * @brief Distruttore.
         */
        ~const_iterator() {}

        /**
         * @brief Operatore di dereferenziazione.
         *
         * @return Il riferimento costante all'elemento puntato dall'iteratore.
         */
        reference operator*() const
        {
            return n->value;
        }

        /**
         * @brief Operatore di accesso ai membri.
         *
         * @return Il puntatore costante all'elemento puntato dall'iteratore.
         */
        pointer operator->() const
        {
            return &(n->value);
        }

        /**
         * @brief Operatore di post-incremento.
         *
         * @return Un iteratore costante che punta all'elemento precedente.
         */
        const_iterator operator++(int)
        {
            const_iterator tmp(*this);
            if (n->right == nullptr)
            {
#ifndef NDEBUG
                std::cout << "n->right == nullptr" << std::endl;
#endif
                while (n->parent != nullptr && n->parent->right == n)
                {
                    n = n->parent;
#ifndef NDEBUG
                    std::cout << "n->parent->right == n" << std::endl;
#endif
                }
                n = n->parent;
                return tmp;
            }
            else
            {
#ifndef NDEBUG
                std::cout << "n->right != nullptr" << std::endl;
#endif
                n = n->right;
                while (n->left != nullptr)
                {
                    n = n->left;
                }
                return tmp;
            }
        }

        /**
         * @brief Operatore di pre-incremento.
         *
         * @return Un riferimento a se stesso.
         */
        const_iterator &operator++()
        {
            if (n->right == nullptr)
            {
                while (n->parent != nullptr && n->parent->right == n)
                {
                    n = n->parent;
                }
                n = n->parent;
                return *this;
            }
            else
            {
                n = n->right;
                while (n->left != nullptr)
                {
                    n = n->left;
                }
                return *this;
            }
        }

        /**
         * @brief Operatore di uguaglianza.
         *
         * @param other L'iteratore da confrontare.
         * @return True se gli iteratori sono uguali, false altrimenti.
         */
        bool operator==(const const_iterator &other) const
        {
            return n == other.n;
        }

        /**
         * @brief Operatore di disuguaglianza.
         *
         * @param other L'iteratore da confrontare.
         * @return True se gli iteratori sono diversi, false altrimenti.
         */
        bool operator!=(const const_iterator &other) const
        {
            return !(other == *this);
        }

    private:
        const node *n;

        friend class bst;

        /**
         * @brief Costruttore privato.
         *
         * @param n Il puntatore al nodo dell'iteratore.
         */
        const_iterator(const node *n) : n(n) {}
    };

    /**
     * @brief Restituisce un iteratore costante che punta all'inizio dell'albero binario di ricerca.
     *
     * @return Un iteratore costante che punta all'elemento minimo dell'albero binario di ricerca.
     */
    const_iterator begin() const
    {
        node *min = _root;
        while (min->left != nullptr)
        {
            min = min->left;
        }
        return const_iterator(min);
    }

    /**
     * @brief Restituisce un iteratore costante che punta alla fine della struttura dati.
     *
     * @return Un iteratore costante che punta alla fine della struttura dati.
     */
    const_iterator end() const
    {
        return const_iterator(nullptr);
    }
};

/**
 * Funzione GLOBALE che stampa a schermo i soli valori
 * dei nodi di un albero binario di ricerca
 * che soddisfano un predicato specificato dall'utente.
 *
 * @tparam T Il tipo degli elementi nell'albero binario di ricerca.
 * @tparam Comp Il funtore di confronto per ordinare gli elementi nell'albero.
 * @tparam Equal Il funtore di confronto per verificare l'uguaglianza tra gli elementi.
 * @tparam P Il tipo del predicato da utilizzare per filtrare gli elementi.
 *
 * @param b L'albero binario di ricerca da stampare.
 * @param pred Il predicato da utilizzare per filtrare gli elementi.
 */
template <typename T, typename Comp, typename Equal, typename P>
void printIF(const bst<T, Comp, Equal> &b, P pred)
{
    typename bst<T, Comp, Equal>::const_iterator i, ie;
    for (i = b.begin(), ie = b.end(); i != ie; ++i)
    {
        if (pred(*i))
        {
            std::cout << *i << std::endl;
        }
    }
}

#endif