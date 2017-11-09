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
#include <unordered_map>

struct Node;

struct edge {
  int gap;                // (salto) proxima posicion de la cadena a comparar
  Node *_node;    // Proximo nodo

  edge() : gap{0}, _node{NULL} {}

  edge(int p, Node *n) : gap{p}, _node{n} {}
};

typedef std::unordered_map<char, edge> container;

struct Node {
  Node() {};
  Node(int _id, int _key, int _range, bool _intern) : id{_id}, key_leaf{_key}, range{_range}, isIntern{_intern} {};

  int id;
  container childs;     // aristas a sub'arboles
  int key_leaf;                   // hoja de su subarbol
  int range = 0;                  // N'umero de hojas en el sub'arbol
  bool isIntern = true;           // si es interno o no
};

template<class T>
class BlindTree {

 public:
  BlindTree() : /*n{0},*/ root{new Node(1, -1, 1, true)}, n_nodes{1} {}

  ~BlindTree() {
    deleteAux(root);
  }

  void print() {
    std::string s = "";
    for (int i = 0; i < dictionary.size(); ++i) {
      std::cout << dictionary[i].first << std::endl;
    }
    auxPrint(root, s);

  }

  size_t size_of() {
    return sizeof(dictionary) + sizeWalk(root) + sizeof(n_nodes);
  }

  int nodesNumber() { return n_nodes; }

  template<typename _O>
  void traverse(_O &_o) {
    auxDFS(root, _o);
  }

  bool insert(std::string s, T obj) {

    std::pair<std::string, T> p = std::make_pair(s, obj);

    int gap = 0, pos = 0;

    // BUSCA LA CADENA EN EL ARBOL DEVUELVE EL ULTIMO NODO VISITADO [SI pos != +inf DEVUELVE EL LOCUS]
    Node *nodeMatch = lookUp(s, root, gap, -1);

    if (nodeMatch == root) {
      // SI NODE MATCH ES ROOT LO CUELGO DE LA RAIZ
      dictionary.push_back(p);
      edge e(1, new Node(++n_nodes, dictionary.size() - 1, 1, false));
      root->childs[s[0]] = e;
      return true;//(dictionary[dictionary.size() - 1]).second;
    }

    std::string str = dictionary[nodeMatch->key_leaf].first;

    // TENEMOS QUE COMPARA LA CADENA CON LA CADENA DEL DICCIONARIO Y VER EN QUE POSICION DIFIEREN
    while (str != "" && ++pos < s.length() && pos < str.length() && str[pos] == s[pos]);

    if (pos == str.length() && s.length() == str.length())
      return false; //dictionary[nodeMatch->key_leaf].second; // YA EXISTE LA CADENA


    // DEVUELVE EL LOCUS DE LA INSERCION [EL PRIMER NODO TAL QUE pos <= gap + node.gap] NOTESE QUE ESTE NODO SIEMPRE ES INTERNO
    gap = 0;

    Node *locus = lookUp(s, root, gap, pos);

    if (pos == gap
        && pos == s.length()) {// ESTE ES EL CASO EN EL QUE UNA CADENA ES PREFIJO DE OTRA. CUELGO LA HOJA CON SIGNO $
      dictionary.push_back(p);
      edge e(pos, new Node(++n_nodes, dictionary.size() - 1, 1, false));
      locus->childs['$'] = e;
      return true;//dictionary.back().second;
    }

    if (pos == gap) { // ESTE ES EL CASO EN EL QUE LA POSICION OCURRE EXACTAMENTE EN
      // DEBEMOS COLGAR UNA NUEVA HOJA DEL NODO ACTUAL LA ARISTA
      dictionary.push_back(p);
      edge e(1, new Node(++n_nodes, dictionary.size() - 1, 1, false));
      locus->childs[s[pos]] = e;
      return true;// (dictionary[dictionary.size() - 1]).second;
    }

    //if (pos > gap) { //ESTE CASO OCURREN CUANDO EL POS ESTA EN EL MEDIO DE LA ARISTA
    // DEBEMOS PARTIR LA ARISTA
    container::iterator it = locus->childs.find(s[gap]);
    // ANTIGUO SUBARBOL
    Node *old = it->second._node;
    // ANTIGUO GAP
    int oldGap = it->second.gap;
    //EL GAP DE LA ARISTA Q SE DIVIDE SERA POS - GAP
    locus->childs[s[gap]].gap = pos - gap;
    // CREAMOS UN NUEVO NODO INTERNO
    locus->childs[s[gap]]._node = new Node(++n_nodes, old->key_leaf, old->range + 1, true); //nuevo nodo interno
    //CREAMOS UNA ARISTA CON EL SUBARBOL ANTIGUO CON EL CARACTER POS EN SU HOJA EL GAP SERA GAP + OLDGAP - POS
    if (gap + oldGap - pos < 0) {
      locus->childs[s[gap]]._node->childs[dictionary[old->key_leaf].first[pos]] = edge(1, old);
    } else {
      locus->childs[s[gap]]._node->childs[dictionary[old->key_leaf].first[pos]] = edge(gap + oldGap - pos, old);
    }
    //COLGAMOS EL NUEVO NODO HOJA
    dictionary.push_back(p);
    if (pos == s.length()) {
      locus->childs[s[gap]]._node->childs['$'] = edge(1, new Node(++n_nodes, dictionary.size() - 1, 1, false));
    } else {
      locus->childs[s[gap]]._node->childs[s[pos]] = edge(1, new Node(++n_nodes, dictionary.size() - 1, 1, false));
    }

    return true;//(dictionary[dictionary.size() - 1]).second;


  };

  void sufixArray(std::vector<int> &sfxA) { sufixArrayAux(root, sfxA); }

  T &operator[](std::string s) {

    T obj{};

    std::pair<std::string, T> p = std::make_pair(s, obj);

    int gap = 0, pos = 0;

    // BUSCA LA CADENA EN EL ARBOL DEVUELVE EL ULTIMO NODO VISITADO [SI pos != +inf DEVUELVE EL LOCUS]
    Node *nodeMatch = lookUp(s, root, gap, -1);

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

    if (pos == str.length() && s.length() == str.length()) {
      return dictionary[nodeMatch->key_leaf].second; // YA EXISTE LA CADENA
    }

    // DEVUELVE EL LOCUS DE LA INSERCION [EL PRIMER NODO TAL QUE pos <= gap + node.gap] NOTESE QUE ESTE NODO SIEMPRE ES INTERNO
    gap = 0;

    Node *locus = lookUp(s, root, gap, pos);

    if (pos == gap
        && pos == s.length()) {// ESTE ES EL CASO EN EL QUE UNA CADENA ES PREFIJO DE OTRA. CUELGO LA HOJA CON SIGNO $
      dictionary.push_back(p);
      edge e(pos, new Node(++n_nodes, dictionary.size() - 1, 1, false));
      locus->childs['$'] = e;
      return dictionary.back().second;
    }

    if (pos == gap) { // ESTE ES EL CASO EN EL QUE LA POSICION OCURRE EXACTAMENTE EN
      // DEBEMOS COLGAR UNA NUEVA HOJA DEL NODO ACTUAL LA ARISTA
      dictionary.push_back(p);
      edge e(1, new Node(++n_nodes, dictionary.size() - 1, 1, false));
      locus->childs[s[pos]] = e;
      return (dictionary[dictionary.size() - 1]).second;
    }

    //if (pos > gap) { //ESTE CASO OCURREN CUANDO EL POS ESTA EN EL MEDIO DE LA ARISTA
    // DEBEMOS PARTIR LA ARISTA
    container::iterator it = locus->childs.find(s[gap]);
    // ANTIGUO SUBARBOL
    Node *old = it->second._node;
    // ANTIGUO GAP
    int oldGap = it->second.gap;
    //EL GAP DE LA ARISTA Q SE DIVIDE SERA POS - GAP
    locus->childs[s[gap]].gap = pos - gap;
    // CREAMOS UN NUEVO NODO INTERNO
    locus->childs[s[gap]]._node = new Node(++n_nodes, old->key_leaf, old->range + 1, true); //nuevo nodo interno
    //CREAMOS UNA ARISTA CON EL SUBARBOL ANTIGUO CON EL CARACTER POS EN SU HOJA EL GAP SERA GAP + OLDGAP - POS
    if (gap + oldGap - pos < 0) {
      locus->childs[s[gap]]._node->childs[dictionary[old->key_leaf].first[pos]] = edge(1, old);
    } else {
      locus->childs[s[gap]]._node->childs[dictionary[old->key_leaf].first[pos]] = edge(gap + oldGap - pos, old);
    }
    //COLGAMOS EL NUEVO NODO HOJA
    dictionary.push_back(p);
    if (pos == s.length()) {
      locus->childs[s[gap]]._node->childs['$'] = edge(1, new Node(++n_nodes, dictionary.size() - 1, 1, false));
    } else {
      locus->childs[s[gap]]._node->childs[s[pos]] = edge(1, new Node(++n_nodes, dictionary.size() - 1, 1, false));
    }

    return (dictionary[dictionary.size() - 1]).second;
  }

  //T &search(const std::string &s) {
  bool search(const std::string &s) {
    int g = 0;
    Node *matchNode = lookUp(s, root, g, -1);

    if (matchNode != NULL && matchNode->key_leaf >= 0 && dictionary[matchNode->key_leaf].first == s.c_str())
      return true;//return dictionary[matchNode->key_leaf].second;

    return false;//throw std::exception{};
  }

  std::size_t size() const { return dictionary.size(); }

  typename std::vector<std::pair<std::string,
                                 T> >::iterator begin() { return dictionary.begin(); }; // INTERFAZ DE ITERADOR
  typename std::vector<std::pair<std::string, T> >::iterator end() { return dictionary.end(); };// INTERFAZ DE ITERADOR


 private:
  //int n;
  int n_nodes = 0;
  std::vector<std::pair<std::string, T> > dictionary;  // diccionario de llaves
  Node *root = NULL;

  Node *lookUp(std::string s, Node *_node, int &gap, int sum) {
    if (_node == NULL) return NULL;

    if (_node->childs.empty())
      // SI ES UNA HOJA RETORNAMOS LA HOJA
      return _node;

    if (gap >= s.length() && _node->childs.find('$') != _node->childs.end()) {
      container::iterator it = _node->childs.find('$');
      return it->second._node;
    }

    if (gap >= s.length())
      return _node;

    container::iterator it = _node->childs.find(s[gap]);

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
    container::iterator it = node->childs.begin();
    int n = node->childs.size();
    for (int i = 0; i < n && it != node->childs.end(); ++i) {
      sufixArrayAux(it->second._node, sfxA);
      ++it;
    }
  }

  void deleteAux(Node *node) {

    container::iterator it = node->childs.begin();
    while (it != node->childs.end()) {
      deleteAux(it->second._node);
      delete it->second._node;
      it++;
    }

    node->childs.clear();
  }
  void auxPrint(Node *node, std::string &s) {

    std::cout << s << node->id << std::endl;

    if (!node->isIntern)
      std::cout << "cadena: " + dictionary[node->key_leaf].first << std::endl;

    std::map<char, edge>::iterator it = node->childs.begin();
    for (int i = 0; i < node->childs.size(); ++i) {
      std::cout << s << s << " (" << it->first << "," << it->second.gap << ") -> " << it->second._node->id << std::endl;
      it++;
    }
    it = node->childs.begin();
    std::string s1 = s + "-";
    for (int i = 0; i < node->childs.size(); ++i) {
      auxPrint(it->second._node, s1);
      it++;
    }
  }

  size_t sizeWalk(Node *node) {
    size_t size = sizeof(node->id)
        + sizeof(node->key_leaf)
        + sizeof(node->range)
        + sizeof(node->isIntern)
        + sizeof(node->childs);

    container::iterator it = node->childs.begin();
    for (int i = 0; i < node->childs.size(); ++i) {
      size += sizeWalk(it->second._node);
      ++it;
    }

    return size;
  }

  template<class O>
  void auxDFS(Node *node, O &obj) {
    if (!node->isIntern) {
      obj(dictionary[node->key_leaf]);
    }

    container::iterator it = node->childs.begin();
    for (int i = 0; i < node->childs.size(); ++i) {
      auxDFS(it->second._node, obj);
      it++;
    }

  }

};

#endif //STRING_DICTIONARIES_BLINDTREE_H
