//
// Created by alpachec on 28-10-17.
//

#ifndef STRING_DICTIONARIES_BLINDTREE_H
#define STRING_DICTIONARIES_BLINDTREE_H

#include <string>
#include <map>
#include <vector>
#include <limits>
#include <iostream>
#include <cstring>

struct Node;

struct edge {
  int gap;                // (salto) proxima posicion de la cadena a comparar
  Node *_node;    // Proximo nodo

  edge() : gap{0}, _node{NULL} {}

  edge(int p, Node *n) : gap{p}, _node{n} {}
};

struct Node {
  Node() {};
  Node(int _id, int _key, int _range, bool _intern) : id{_id}, key_leaf{_key}, range{_range}, isIntern{_intern} {};

  int id;
  std::map<char, edge> childs;     // aristas a sub'arboles
  int key_leaf;                   // hoja de su subarbol
  int range = 0;                  // N'umero de hojas en el sub'arbol
  bool isIntern = true;           // si es interno o no
};

template<class T>
class BlindTree {

 public:
  BlindTree() : n{0}, root{new Node(1, -1, 1, true)}, n_nodes{1} {}

  ~BlindTree() {}

  bool insert(std::string s, T obj) {

    std::pair<std::string, T> p = std::make_pair(s, obj);

    int gap = 0, pos = 0;

    // BUSCA LA CADENA EN EL ARBOL DEVUELVE EL ULTIMO NODO VISITADO [SI pos != +inf DEVUELVE EL LOCUS]
    Node *nodeMatch = lookUp(s, root, gap, std::numeric_limits<int>::max());

    if (nodeMatch == root) {
      // SI NODE MATCH ES ROOT LO CUELGO DE LA RAIZ
      dictionary.push_back(p);
      edge e(1, new Node(++n_nodes, dictionary.size() - 1, 1, false));
      root->childs[s[0]] = e;
      return true;
    }

    std::string str = dictionary[nodeMatch->key_leaf].first;

    // TENEMOS QUE COMPARA LA CADENA CON LA CADENA DEL DICCIONARIO Y VER EN QUE POSICION DIFIEREN
    while (str != "" && ++pos < s.length() && pos < str.length() && str[pos] == s[pos]);

    if (pos == str.length() && s.length() == str.length()) return false; // YA EXISTE LA CADENA

    // DEVUELVE EL LOCUS DE LA INSERCION [EL PRIMER NODO TAL QUE pos <= gap + node.gap] NOTESE QUE ESTE NODO SIEMPRE ES INTERNO
    gap = 0;

    Node *locus = lookUp(s, root, gap, pos);

    if (pos == gap || (pos > gap && locus == nodeMatch)) {
      // DEBEMOS COLGAR UNA NUEVA HOJA DEL NODO ACTUAL LA ARISTA
      dictionary.push_back(p);
      edge e(pos, new Node(++n_nodes, dictionary.size() - 1, 1, false));
      locus->childs[s[pos]] = e;
      return true;
    }

    // DEBEMOS DIVIDIR LA ARISTA [pos >= gap] CREAMOS UN NUEVO NODO INTERNO Y COLGAMOES
    // ENCONTRAR LA ARISTA A DIVIDIR

    dictionary.push_back(p);

    std::map<char, edge>::iterator it = locus->childs.find(s[gap]);

    char a = it->first;
    edge oldedge = it->second;
    Node *temp = oldedge._node;

    locus->childs[a]._node = new Node(++n_nodes, temp->key_leaf, temp->range + 1, true); //nuevo nodo interno
    locus->childs[a].gap = pos - gap;

    locus->childs[a]._node->childs[dictionary[temp->key_leaf].first[pos]] = edge(1, temp); // antiguo subarbol
    Node *leaf = new Node(++n_nodes, dictionary.size() - 1, 1, false); // nueva hoja
    locus->childs[a]._node->childs[s[pos]] = edge(1, leaf);

  };

  void sufixArray(std::vector<int> &sfxA) { sufixArrayAux(root, sfxA); }

  T &operator[](std::string s) {
    T obj{};
    std::pair<std::string, T> p = std::make_pair(s, obj);

    int gap = 0, pos = 0;

    // BUSCA LA CADENA EN EL ARBOL DEVUELVE EL ULTIMO NODO VISITADO [SI pos != +inf DEVUELVE EL LOCUS]
    Node *nodeMatch = lookUp(s, root, gap, std::numeric_limits<int>::max());

    if (nodeMatch == root) {
      // SI NODE MATCH ES ROOT LO CUELGO DE LA RAIZ
      dictionary.push_back(p);
      edge e(1, new Node(++n_nodes, dictionary.size() - 1, 1, false));
      root->childs[s[0]] = e;
      return (dictionary[dictionary.size() - 1]).second;
    }

    std::string str = dictionary[nodeMatch->key_leaf].first;

    // TENEMOS QUE COMPARA LA CADENA CON LA CADENA DEL DICCIONARIO Y VER EN QUE POSICION DIFIEREN
    while (str != "" && ++pos < s.length() && pos < str.length() && str[pos] == s[pos]);

    if (pos == str.length() && s.length() == str.length())
      return dictionary[nodeMatch->key_leaf].second; // YA EXISTE LA CADENA

    // DEVUELVE EL LOCUS DE LA INSERCION [EL PRIMER NODO TAL QUE pos <= gap + node.gap] NOTESE QUE ESTE NODO SIEMPRE ES INTERNO
    gap = 0;

    Node *locus = lookUp(s, root, gap, pos);

    if (pos == gap || (pos > gap && locus == nodeMatch)) {
      // DEBEMOS COLGAR UNA NUEVA HOJA DEL NODO ACTUAL LA ARISTA
      dictionary.push_back(p);
      edge e(pos, new Node(++n_nodes, dictionary.size() - 1, 1, false));
      locus->childs[s[pos]] = e;
      return (dictionary[dictionary.size() - 1]).second;
    }

    // DEBEMOS DIVIDIR LA ARISTA [pos >= gap] CREAMOS UN NUEVO NODO INTERNO Y COLGAMOES
    // ENCONTRAR LA ARISTA A DIVIDIR


    dictionary.push_back(p);

    std::map<char, edge>::iterator it = locus->childs.find(s[gap]);

    char a = it->first;
    edge oldedge = it->second;
    Node *temp = oldedge._node;

    locus->childs[a]._node = new Node(++n_nodes, temp->key_leaf, temp->range + 1, true); //nuevo nodo interno
    locus->childs[a].gap = pos - gap;

    locus->childs[a]._node->childs[dictionary[temp->key_leaf].first[pos]] = edge(1, temp); // antiguo subarbol
    Node *leaf = new Node(++n_nodes, dictionary.size() - 1, 1, false); // nueva hoja
    locus->childs[a]._node->childs[s[pos]] = edge(1, leaf);

    return (dictionary[dictionary.size() - 1]).second;
  }

  T &search(const std::string &s) {
    int g = 0;
    Node *matchNode = lookUp(s, root, g, -1);

    if (matchNode != NULL && strcmp(dictionary[matchNode->key_leaf].first.c_str(), s.c_str()) == 0)
      return dictionary[matchNode->key_leaf].second;

    throw std::exception{};
  }

  std::size_t size() const { return dictionary.size(); }

  typename std::vector<std::pair<std::string,
                                 T> >::iterator begin() { return dictionary.begin(); }; // INTERFAZ DE ITERADOR
  typename std::vector<std::pair<std::string, T> >::iterator end() { return dictionary.end(); };// INTERFAZ DE ITERADOR


 private:
  int n;
  int n_nodes = 0;
  std::vector<std::pair<std::string, T> > dictionary;  // diccionario de llaves
  Node *root = NULL;

  Node *lookUp(std::string s, Node *_node, int &gap, int sum) {
    if (_node == NULL) return NULL;

    if (_node->childs.empty())
      // SI ES UNA HOJA RETORNAMOS LA HOJA
      return _node;

    std::map<char, edge>::iterator it = _node->childs.find(s[gap]);

    if (it == _node->childs.end()) {
      // SI NO PODEMOS SEGUIR BAJANDO RETORNAMOS EL NODO ACTUAL;
      return _node;
    }

    if (sum != -1) {
      _node->range++;
      if ((gap + it->second.gap > sum || (sum <= s.length() && !(it->second._node->isIntern))))
        // RETORNAREMOS EL LOCUS Y POR REFERENCIA EL HIJO EN NODE
        return _node;
    }

    gap += it->second.gap;

    // SINO SEGUIR BAJANDO;
    return lookUp(s, it->second._node, gap, sum);
  }

  void sufixArrayAux(Node *node, std::vector<int> &sfxA) {
    if (!node->isIntern) {
      sfxA.push_back(node->key_leaf);
      return;
    }
    std::map<char, edge>::iterator it = node->childs.begin();
    int n = node->childs.size();
    for (int i = 0; i < n && it != node->childs.end(); ++i) {
      sufixArrayAux(it->second._node, sfxA);
      ++it;
    }
  }

};

#endif //STRING_DICTIONARIES_BLINDTREE_H
