//
// Created by alpachec on 06-11-17.
//

#ifndef STRING_DICTIONARIES_TERNARYTREE_H
#define STRING_DICTIONARIES_TERNARYTREE_H

#include <string>
#include <map>
#include <limits>
#include <iostream>
#include <cstring>
#include <vector>

struct TernaryNode {

  int id;
  char tag;
  int key;
  TernaryNode *central;
  TernaryNode *left;
  TernaryNode *right;

  TernaryNode(int _id, char a) : id{_id}, tag{a}, central{NULL}, left{NULL}, right{NULL} {

  }

};

template<class T>
class TernaryTree {

 public:
  TernaryTree() : n_nodes{0}, root{NULL} {};
  ~TernaryTree() {

    deleteAux(root);
    delete root;
  };

  T &search(const std::string &s) {
    int p = 0;

    TernaryNode *matchNode = searchAux(s, p, root);

    //CASO LLEGA A UN NODO VACIO SIN ENCONTRAR LA CADENA
    if (matchNode == NULL) {
      throw std::exception{};
    }
    //CASO BASE ENCUENTRA LA CADENA
    return dictionary[matchNode->key].second;
  }

  bool insert(const std::string &s, const T &obj) {

    int p = 0;

    if (root == NULL) {
      root = new TernaryNode(1, s[p]);
      createListNodes(root, s, obj, ++p);
      return true;
    }

    TernaryNode *matchNode = lookUp(s, p, obj, root);

    //CASO BASE ENCUENTRA LA CADENA
    if (p + 1 == s.length()) return false;

    //CASO LLEGA A UN NODO VACIO SIN ENCONTRAR LA CADENA
    if (p < s.length()) {
      createListNodes(matchNode, s, obj, p);
      return true;
    }


    //SI P == LENGTH ES PORQUE SE INSERTO EN EL LOOKUP

    return true;

  }

  T &operator[](const std::string &s) {

    int p = 0;
    T obj{};

    if (root == NULL) {
      root = new TernaryNode(1, s[p]);
      auto tmp = createListNodes(root, s, obj, ++p);
      return dictionary[tmp->key].second;
    }

    TernaryNode *matchNode = lookUp(s, p, obj, root);

    //CASO BASE ENCUENTRA LA CADENA
    if (p + 1 == s.length()) {
      return dictionary[matchNode->key].second;

    }

    //CASO LLEGA A UN NODO VACIO SIN ENCONTRAR LA CADENA Y ME QUEDAN ELEMENTOS POR INSERTAR
    if (p < s.length()) {
      TernaryNode *leaf = createListNodes(matchNode, s, obj, p);
      return dictionary[leaf->key].second;
    }

    //SI P == LENGTH ES PORQUE SE INSERTO EN EL LOOKUP

    return dictionary[matchNode->key].second;

  }

  std::size_t size() const { return dictionary.size(); }

  typename std::vector<std::pair<std::string, T> >::iterator begin() {// INTERFAZ DE ITERADOR
    return dictionary.begin();
  }

  typename std::vector<std::pair<std::string, T> >::iterator end() {// INTERFAZ DE ITERADOR
    return dictionary.end();
  }

  void print() {
    dfsPrintAux(root, "");
  }

 private:

  std::vector<std::pair<std::string, T> > dictionary;
  TernaryNode *root;
  int n_nodes;

  TernaryNode *lookUp(const std::string &s, int &p, const T &obj, TernaryNode *_node) {

    if (_node == NULL) { // EL ARBOL ESTA VACIO
      return _node;
    }

    // ESTE CASO ES CUANDO LA CADENA ES PREFIJO DE OTRA
    if (p == s.length()) {

      if (_node->tag == '$') {
        p--;
        return _node;
      } else {

        //BUSCAR A LA IZQUIERDA EL SIGNO DE PESO

        TernaryNode *current = _node;

        while (current->left != NULL && current->left->tag != '$')
          current = current->left;

        if (current->left == NULL) { // SI NO EXISTE LO INSERTO
          current->left = new TernaryNode(++n_nodes, '$');
          dictionary.push_back(std::make_pair(s, obj));
          current->left->key = dictionary.size() - 1;

        } else {
          p--;
          // SI EXISTE ENTONCES LA CADENA YA ESTA INSERTADA
        }
        return current->left;
      };

    }

    // COMPARAR CON LA ETIQUETA DEL NODO
    if (s[p] < _node->tag) {

      if (_node->left == NULL) {
        // SI EL NODO ES VACIO LO INSERTO
        _node->left = new TernaryNode(++n_nodes, s[p]);
        int t = ++p;
        p = s.length();
        return createListNodes(_node->left, s, obj, t);

        /*if(p == s.length()){
            _node->left->central  = new TernaryNode(++n_nodes, '$');
            dictionary.push_back( std::make_pair(s,obj) );
            _node->left->central->key = dictionary.size()-1;
            return _node->left->central;
        }

        return _node->left ;*/
      }
      //IR A HIJO IZQUIERDO
      return lookUp(s, p, obj, _node->left);
    }
    if (s[p] > _node->tag) {
      //IR A HIJO DERECHO
      if (_node->right == NULL) {
        // SI EL NODO ES VACIO LO INSERTO
        _node->right = new TernaryNode(++n_nodes, s[p]);
        int t = ++p;
        p = s.length();
        return createListNodes(_node->right, s, obj, t);
        /*if(p == s.length()){

            _node->right->central = new TernaryNode(++n_nodes, '$');
            dictionary.push_back( std::make_pair(s,obj) );
            _node->right->central->key = dictionary.size()-1;
        }
        return _node->right ;*/
      }

      return lookUp(s, p, obj, _node->right);
    }

    /*if(_node->central->tag == '$'){
        return _node;
    }*/

    return lookUp(s, ++p, obj, _node->central);
  }

  TernaryNode *createListNodes(TernaryNode *_node, const std::string &s, const T &obj, int p) {

    TernaryNode *current = _node;

    for (int i = p; i < s.length(); ++i) {

      current->central = new TernaryNode(++n_nodes, s[i]);
      current = current->central;

    }
    current->central = new TernaryNode(++n_nodes, '$');
    current = current->central;
    dictionary.push_back(std::make_pair(s, obj));
    current->key = dictionary.size() - 1;
    return current;
  }

  TernaryNode *searchAux(std::string s, int &p, TernaryNode *node) {

    if (node == NULL) return NULL;

    if (p == s.length()) { // MATCH DE CADENA
      if (node->tag != '$') {
        //BUSCAR A LA IZQUIERDA EL SIGNO DE PESO
        TernaryNode *current = node;
        while (current->left != NULL && current->left->tag != '$')
          current = current->left;
        return current->left;
      } else {
        return node;
      }
    }

    if (s[p] < node->tag)
      return searchAux(s, p, node->left);
    if (s[p] > node->tag)
      return searchAux(s, p, node->right);

    return searchAux(s, ++p, node->central);
  }

  void dfsPrintAux(TernaryNode *node, std::string s) {
    if (node == NULL)return;
    std::cout << s << "" << node->tag << std::endl;
    dfsPrintAux(node->left, s + "*");
    dfsPrintAux(node->central, s + "*");
    dfsPrintAux(node->right, s + "*");

  }

  void deleteAux(TernaryNode *node) {

    if (node == NULL)
      return;

    if (node->left != NULL) {
      deleteAux(node->left);
      delete node->left;
    }
    if (node->central != NULL) {
      deleteAux(node->central);
      delete node->central;
    }
    if (node->right != NULL) {
      deleteAux(node->right);
      delete node->right;
    }

  }

};

#endif //STRING_DICTIONARIES_TERNARYTREE_H
