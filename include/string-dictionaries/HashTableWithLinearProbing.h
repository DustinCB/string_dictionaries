//
// Created by Dustin Cobas Batista on 02-Nov-17.
//

#ifndef STRING_DICTIONARIES_HASHTABLELINEARPROBING_H
#define STRING_DICTIONARIES_HASHTABLELINEARPROBING_H

#include <utility>
#include <vector>

/**
 * Tabla hash con exploración lineal.
 *
 * @tparam _Value Tipo del valor a contener.
 * @tparam _Hash Función hash para cadena de caracteres a usar con las llaves.
 */
template<typename _Value, typename _Hash = std::hash<std::string>>
class HashTableWithLinearProbing {
 public:
  HashTableWithLinearProbing() {}
  HashTableWithLinearProbing(std::size_t _size) : SIZE{_size} {}

  using _Key = std::string;


  /**
   * Función para insertar un elemento con su valor a la estructura
   *
   * @param _key Llave (cadena de caracteres)
   * @param _value Valor (Tipo plantilla dado a la clase)
   * @return true si el elemento no existía y es insertado, false en caso contrario
   */
  bool insert(const _Key &_key, const _Value &_value) {
    if (currentSize == SIZE)
      return false;

    auto i = hash(_key) % SIZE;
    while (table[i].first && table[i].second.first != _key) {
      i = (i + 1) % SIZE;
    }

    if (table[i].first)
      return false;

    table[i].second.second = _value;
    if (!table[i].first) {
      table[i].second.first = _key;
      table[i].first = true;
      currentSize++;
    }

    return true;
  }


  /**
   * Función para buscar un elemento
   * @param _key Llave
   * @return true si el elemento existe en la estructura, false en caso contrario
   */
  bool search(const _Key &_key) const {
    auto i = hash(_key) % SIZE;
    while (table[i].first && table[i].second.first != _key) {
      i = (i + 1) % SIZE;
    }

    return !(!table[i].first || table[i].second.first != _key);
  }


  /**
   * Operador para acceder al valor de un elemento
   * @param _key LLave
   * @return Referencia al valor asociado a la llave dada
   */
  _Value &operator[](const _Key &_key) {
    auto s = SIZE + 1;
    auto i = hash(_key) % SIZE;
    while (--s && table[i].first && table[i].second.first != _key) {
      i = (i + 1) % SIZE;
    }

    if (!s)
      throw std::exception{};

    if (!table[i].first) {
      table[i].second.first = _key;
      table[i].first = true;
      currentSize++;
    }

    return table[i].second.second;
  }


  /**
   * Función para recorrer la estructura y ejecutar la operación dada sobre cada par <llave, valor>
   * @tparam _O Tipo plantilla de la operación
   * @param _o Operación a ejecutar sobre cada par <llave, valor>
   */
  template<typename _O>
  void traverse(_O &_o) {
    for (auto &&item : table) {
      if (item.first) {
        _o(item.second);
      }
    }
  }


  /**
   * Función que devuelve la cantidad de elementos contenidos en la estructura
   * @return Cantidad de elementos
   */
  std::size_t size() const {
    return currentSize;
  }


  /**
   * Función que retorna la cantidad máxima de elementos que pueden ser almacenados en la estructura
   * @return Cantidad máxima de elementos
   */
  std::size_t capacity() const {
    return table.size();
  }

 private:
  using InnerContainer = std::vector<std::pair<bool, std::pair<_Key, _Value>>>;

 public:
  /**
   * Iterador para poder recorrer la clase como un contenedor de la STL (begin, end).
   *
   * El iterador debe manejar los saltos presentes en la tabla, es decir, las posiciones sin elementos.
   */
  class Iterator {
   public:
    Iterator(const typename InnerContainer::iterator &_it,
             const typename InnerContainer::iterator &_eit) : it{_it},
                                                              eit{_eit} {}

    Iterator(const Iterator &_it) : it{_it.it}, eit{_it.eit} {}

    std::pair<_Key, _Value> &operator*() { return it->second; }

    Iterator &operator++() {
      while (it != eit && !(++it)->first) {}
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp(*this);
      operator++();
      return tmp;
    }

    bool operator==(const Iterator &rhs) const { return it == rhs.it; }

    bool operator!=(const Iterator &rhs) const { return it != rhs.it; }

   private:
    typename InnerContainer::iterator it;
    typename InnerContainer::iterator eit;
  };


  /**
   * Iterador al primer elemento
   * @return Iterador al primer elemento
   */
  Iterator begin() {
    if (table.empty())
      return Iterator{table.end(), table.end()};

    auto it = table.begin();
    while (!it->first) {
      ++it;
    }

    return Iterator{it, table.end()};
  }


  /**
   * Iterador al último elemento
   * @return Iterador al último elemento
   */
  Iterator end() {
    return Iterator{table.end(), table.end()};
  }


 private:
  const std::size_t SIZE = 50000;
  InnerContainer table{SIZE};
  _Hash hash;
  std::size_t currentSize = 0;
};

#endif //STRING_DICTIONARIES_HASHTABLELINEARPROBING_H
