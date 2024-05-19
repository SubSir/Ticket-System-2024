#ifndef SJTU_FILE_STORAGE_HPP
#define SJTU_FILE_STORAGE_HPP
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
#ifndef SJTU_EXCEPTIONS_HPP
#define SJTU_EXCEPTIONS_HPP

namespace sjtu {

class exception {
protected:
  const std::string variant = "";
  std::string detail = "";

public:
  exception() {}
  exception(const exception &ec) : variant(ec.variant), detail(ec.detail) {}
  virtual std::string what() { return variant + " " + detail; }
};

class index_out_of_bound : public exception {
  /* __________________________ */
};

class runtime_error : public exception {
  /* __________________________ */
};

class invalid_iterator : public exception {
  /* __________________________ */
};

class container_is_empty : public exception {
  /* __________________________ */
};
} // namespace sjtu

#endif

#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template <typename T> class vector {
public:
  /**
   * you can see RandomAccessIterator at CppReference for help.
   */
  class const_iterator;
  class iterator {
    // The following code is written for the C++ type_traits library.
    // Type traits is a C++ feature for describing certain properties of a type.
    // For instance, for an iterator, iterator::value_type is the type that the
    // iterator points to.
    // STL algorithms and containers may use these type_traits (e.g. the
    // following typedef) to work properly. In particular, without the following
    // code,
    // @code{std::sort(iter, iter1);} would not compile.
    // See these websites for more information:
    // https://en.cppreference.com/w/cpp/header/type_traits
    // About value_type:
    // https://blog.csdn.net/u014299153/article/details/72419713 About
    // iterator_category: https://en.cppreference.com/w/cpp/iterator
  public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::output_iterator_tag;

  private:
    size_t *size;
    T **ptr;
    size_t index;

  public:
    /**
     * return a new iterator which pointer n-next elements
     * as well as operator-
     */
    friend vector;
    iterator(T **p = nullptr, size_t i = 0, size_t *q = nullptr)
        : ptr(p), index(i), size(q) {}
    iterator operator+(const int &n) const {
      if (*size < index + n)
        throw index_out_of_bound();
      return iterator(ptr, index + n, size);
    }
    iterator operator-(const int &n) const {
      if (index < n)
        throw index_out_of_bound();
      return iterator(ptr, index - n, size);
    }
    // return the distance between two iterators,
    // if these two iterators point to different vectors, throw
    // invaild_iterator.
    int operator-(const iterator &rhs) const {
      if (size != rhs.size)
        throw invalid_iterator();
      return index - rhs.index;
    }
    iterator &operator+=(const int &n) {
      if (*size < index + n)
        throw index_out_of_bound();
      index += n;
      return *this;
    }
    iterator &operator-=(const int &n) {
      if (index < n)
        throw index_out_of_bound();
      index -= n;
      return *this;
    }
    iterator operator++(int) {
      if (index == *size)
        throw index_out_of_bound();
      iterator tmp = *this;
      index++;
      return tmp;
    }
    iterator &operator++() {
      if (index == *size)
        throw index_out_of_bound();
      index++;
      return *this;
    }
    iterator operator--(int) {
      if (index == 0)
        throw index_out_of_bound();
      iterator tmp = *this;
      index--;
      return tmp;
    }
    iterator &operator--() {
      if (index == 0)
        throw index_out_of_bound();
      index--;
      return *this;
    }
    T &operator*() const {
      if (index < 0 || index >= *size)
        throw index_out_of_bound();
      return *ptr[index];
    }

    /**
     * a operator to check whether two iterators are same (pointing to the same
     * memory address).
     */
    friend const_iterator;
    bool operator==(const iterator &rhs) const {
      return ptr == rhs.ptr && index == rhs.index;
    }
    bool operator==(const const_iterator &rhs) const {
      return ptr == rhs.ptr && index == rhs.index;
    }
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const {
      return ptr != rhs.ptr || index != rhs.index;
    }
    bool operator!=(const const_iterator &rhs) const {
      return ptr != rhs.ptr || index != rhs.index;
    }
  };

  class const_iterator {
  public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::output_iterator_tag;

  private:
    size_t const *size;
    T *const *ptr;
    size_t index;

  public:
    /**
     * return a new iterator which pointer n-next elements
     * as well as operator-
     */
    friend vector;
    const_iterator(T *const *p = nullptr, size_t i = 0,
                   size_t const *q = nullptr)
        : ptr(p), index(i), size(q) {}
    const_iterator operator+(const int &n) const {
      if (*size < index + n)
        throw index_out_of_bound();
      return const_iterator(ptr, index + n, size);
    }
    const_iterator operator-(const int &n) const {
      if (index < n)
        throw index_out_of_bound();
      return const_iterator(ptr, index - n, size);
    }
    // return the distance between two iterators,
    // if these two iterators point to different vectors, throw
    // invaild_iterator.
    int operator-(const iterator &rhs) const {
      if (size != rhs.size)
        throw invalid_iterator();
      return index - rhs.index;
    }
    const_iterator &operator+=(const int &n) {
      if (*size < index + n)
        throw index_out_of_bound();
      index += n;
      return *this;
    }
    const_iterator &operator-=(const int &n) {
      if (index < n)
        throw index_out_of_bound();
      index -= n;
      return *this;
    }
    const_iterator operator++(int) {
      if (index == *size)
        throw index_out_of_bound();
      iterator tmp = *this;
      index++;
      return tmp;
    }
    const_iterator &operator++() {
      if (index == *size)
        throw index_out_of_bound();
      index++;
      return *this;
    }
    const_iterator operator--(int) {
      if (index == 0)
        throw index_out_of_bound();
      const_iterator tmp = *this;
      index--;
      return tmp;
    }
    const_iterator &operator--() {
      if (index == 0)
        throw index_out_of_bound();
      index--;
      return *this;
    }
    T &operator*() const {
      if (index < 0 || index >= *size)
        throw index_out_of_bound();
      return *ptr[index];
    }

    /**
     * a operator to check whether two iterators are same (pointing to the same
     * memory address).
     */
    friend iterator;
    bool operator==(const iterator &rhs) const {
      return ptr == rhs.ptr && index == rhs.index;
    }
    bool operator==(const const_iterator &rhs) const {
      return ptr == rhs.ptr && index == rhs.index;
    }
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const {
      return ptr != rhs.ptr || index != rhs.index;
    }
    bool operator!=(const const_iterator &rhs) const {
      return ptr != rhs.ptr || index != rhs.index;
    }
  };
  T **data;
  size_t capacity;
  size_t current_size;
  vector(int init_capacity = 1)
      : data(new T *[init_capacity]), capacity(init_capacity), current_size(0) {
  }
  vector(const vector &other) {
    data = new T *[other.capacity];
    capacity = other.capacity;
    current_size = other.current_size;
    for (int i = 0; i < current_size; i++) {
      data[i] = new T(*other.data[i]);
    }
  }
  ~vector() {
    for (int i = 0; i < current_size; i++)
      delete data[i];
    delete[] data;
  }
  vector &operator=(const vector &other) {
    if (this == &other)
      return *this;
    for (int i = 0; i < current_size; i++)
      delete data[i];
    delete[] data;
    data = new T *[other.capacity];
    capacity = other.capacity;
    current_size = other.current_size;
    for (int i = 0; i < current_size; i++) {
      data[i] = new T(*other.data[i]);
    }
    return *this;
  }
  void resize() {
    T **tmp = new T *[capacity * 2];
    for (int i = 0; i < current_size; i++) {
      tmp[i] = data[i];
    }
    delete[] data;
    data = tmp;
    capacity *= 2;
  }
  void desize() {
    T **tmp = new T *[capacity / 2];
    for (int i = 0; i < current_size; i++) {
      tmp[i] = data[i];
    }
    delete[] data;
    data = tmp;
    capacity /= 2;
  }
  /**
   * assigns specified element with bounds checking
   * throw index_out_of_bound if pos is not in [0, size)
   */
  T &at(const size_t &pos) {
    if (pos < 0 || pos >= current_size)
      throw index_out_of_bound();
    return *data[pos];
  }
  const T &at(const size_t &pos) const {
    if (pos < 0 || pos >= current_size)
      throw index_out_of_bound();
    return *data[pos];
  }
  /**
   * assigns specified element with bounds checking
   * throw index_out_of_bound if pos is not in [0, size)
   * !!! Pay attentions
   *   In STL this operator does not check the boundary but I want you to do.
   */
  T &operator[](const size_t &pos) {
    if (pos < 0)
      throw index_out_of_bound();
    while (current_size <= pos) {
      default_push_back();
    }
    return *data[pos];
  }
  const T &operator[](const size_t &pos) const {
    if (pos < 0 || pos >= current_size)
      throw index_out_of_bound();
    return *data[pos];
  }
  /**
   * access the first element.
   * throw container_is_empty if size == 0
   */
  const T &front() const {
    if (current_size == 0)
      throw container_is_empty();
    return *data[0];
  }
  /**
   * access the last element.
   * throw container_is_empty if size == 0
   */
  const T &back() const {
    if (current_size == 0)
      throw container_is_empty();
    return *data[current_size - 1];
  }
  /**
   * returns an iterator to the beginning.
   */
  iterator begin() { return iterator(&(*data), 0, &current_size); }
  const_iterator cbegin() const {
    return const_iterator(&(*data), 0, &current_size);
  }
  /**
   * returns an iterator to the end.
   */
  iterator end() { return iterator(&(*data), current_size, &current_size); }
  const_iterator cend() const {
    return const_iterator(&(*data), current_size, &current_size);
  }
  /**
   * checks whether the container is empty
   */
  bool empty() const { return current_size == 0; }
  /**
   * returns the number of elements
   */
  size_t size() const { return current_size; }
  /**
   * clears the contents
   */
  void clear() {
    for (int i = 0; i < current_size; i++)
      delete data[i];
    current_size = 0;
    delete[] data;
    data = new T *[1];
    capacity = 1;
  }
  /**
   * inserts value before pos
   * returns an iterator pointing to the inserted value.
   */
  iterator insert(iterator pos, const T &value) {
    if (current_size == capacity)
      resize();
    for (int i = current_size; i > pos.index; i--) {
      data[i] = data[i - 1];
    }
    data[pos.index] = new T(value);
    current_size++;
    return pos;
  }
  /**
   * inserts value at index ind.
   * after inserting, this->at(ind) == value
   * returns an iterator pointing to the inserted value.
   * throw index_out_of_bound if ind > size (in this situation ind can be size
   * because after inserting the size will increase 1.)
   */
  iterator insert(const size_t &ind, const T &value) {
    if (ind > current_size)
      throw index_out_of_bound();
    if (current_size == capacity)
      resize();
    for (int i = current_size; i > ind; i--) {
      data[i] = data[i - 1];
    }
    data[ind] = new T(value);
    current_size++;
    return iterator(&data, ind, &current_size);
  }
  /**
   * removes the element at pos.
   * return an iterator pointing to the following element.
   * If the iterator pos refers the last element, the end() iterator is
   * returned.
   */
  iterator erase(iterator pos) {
    delete data[pos.index];
    for (int i = pos.index; i < current_size - 1; i++) {
      data[i] = data[i + 1];
    }
    current_size--;
    if (current_size < capacity / 4) {
      desize();
    }
    return pos;
  }
  /**
   * removes the element with index ind.
   * return an iterator pointing to the following element.
   * throw index_out_of_bound if ind >= size
   */
  void erase(const size_t &ind) {
    if (ind >= current_size)
      throw index_out_of_bound();
    delete data[ind];
    for (int i = ind; i < current_size - 1; i++) {
      data[i] = data[i + 1];
    }
    current_size--;
    if (current_size < capacity / 4) {
      desize();
    }
    return;
  }
  /**
   * adds an element to the end.
   */
  void push_back(const T &value) {
    if (current_size == capacity)
      resize();
    data[current_size++] = new T(value);
  }
  void default_push_back() {
    if (current_size == capacity)
      resize();
    data[current_size++] = new T();
  }
  /**
   * remove the last element from the end.
   * throw container_is_empty if size() == 0
   */
  void pop_back() {
    if (current_size == 0)
      throw container_is_empty();
    delete data[--current_size];
    if (current_size < capacity / 4) {
      desize();
    }
  }
  template <typename Compare>
  void merge(T **arr, int left, int mid, int right, T **tmp, Compare cmp) {
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
      if (cmp(*arr[i], *arr[j])) {
        tmp[k++] = arr[i++];
      } else {
        tmp[k++] = arr[j++];
      }
    }
    while (i <= mid) {
      tmp[k++] = arr[i++];
    }
    while (j <= right) {
      tmp[k++] = arr[j++];
    }
    for (int p = left; p <= right; p++) {
      arr[p] = tmp[p];
    }
  }
  template <typename Compare>

  void mergeSort(T **arr, int left, int right, T **tmp, Compare cmp) {
    if (left >= right) {
      return;
    }
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid, tmp, cmp);
    mergeSort(arr, mid + 1, right, tmp, cmp);
    merge(arr, left, mid, right, tmp, cmp);
  }

  template <typename Compare> void sort(Compare cmp) {
    T **tmp = new T *[capacity];
    mergeSort(data, 0, current_size - 1, tmp, cmp);
    delete[] tmp;
  }
};

} // namespace sjtu

#endif
/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
namespace sjtu {

template <class Key, class T, class Compare = std::less<Key>> class map {
public:
  /**
   * the internal type of data.
   * it should have a default constructor, a copy constructor.
   * You can use sjtu::map as value_type by typedef.
   */
  typedef pair<const Key, T> value_type;

  /**
   * see BidirectionalIterator at CppReference for help.
   *
   * if there is anything wrong throw invalid_iterator.
   *     like it = map.begin(); --it;
   *       or it = map.end(); ++end();
   */
  struct node;
  class const_iterator;
  class iterator {
  private:
    node *cur;
    map *_map;

  public:
    using key_type = Key;
    using value = T;
    iterator() {
      cur = nullptr;
      _map = nullptr;
    }
    iterator(node *c, map *m) {
      cur = c;
      _map = m;
    }
    iterator(const iterator &other) {
      cur = other.cur;
      _map = other._map;
    }
    iterator operator++(int) {
      iterator _tmp = *this;
      if (cur == nullptr or cur->data == nullptr)
        throw invalid_iterator();
      if (cur->right) {
        cur = cur->right;
        while (cur->left)
          cur = cur->left;
      } else {
        node *tmp = cur->father;
        while (tmp and cur == tmp->right) {
          cur = tmp;
          tmp = tmp->father;
        }
        cur = tmp;
      }
      return _tmp;
    }
    iterator &operator++() {
      if (cur == nullptr or cur->data == nullptr)
        throw invalid_iterator();
      if (cur->right) {
        cur = cur->right;
        while (cur->left)
          cur = cur->left;
      } else {
        node *tmp = cur->father;
        while (tmp and cur == tmp->right) {
          cur = tmp;
          tmp = tmp->father;
        }
        cur = tmp;
      }
      return *this;
    }
    iterator operator--(int) {
      iterator _tmp = *this;
      if (cur == nullptr)
        throw invalid_iterator();
      if (cur->left) {
        cur = cur->left;
        while (cur->right)
          cur = cur->right;
      } else {
        node *tmp = cur->father;
        while (tmp and cur == tmp->left) {
          cur = tmp;
          tmp = tmp->father;
        }
        if (tmp == nullptr)
          throw invalid_iterator();
        cur = tmp;
      }
      return _tmp;
    }
    iterator &operator--() {
      if (cur == nullptr)
        throw invalid_iterator();
      if (cur->left) {
        cur = cur->left;
        while (cur->right)
          cur = cur->right;
      } else {
        node *tmp = cur->father;
        while (tmp and cur == tmp->left) {
          cur = tmp;
          tmp = tmp->father;
        }
        if (tmp == nullptr)
          throw invalid_iterator();
        cur = tmp;
      }
      return *this;
    }
    /**
     * a operator to check whether two iterators are same (pointing to the same
     * memory).
     */
    value_type &operator*() const {
      if (cur == nullptr)
        throw invalid_iterator();
      return *(cur->data);
    }
    iterator &operator=(const iterator &other) {
      if (this == &other)
        return *this;
      cur = other.cur;
      _map = other._map;
      return *this;
    }
    bool operator==(const iterator &rhs) const { return cur == rhs.cur; }
    bool operator==(const const_iterator &rhs) const { return cur == rhs.cur; }
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const { return cur != rhs.cur; }
    bool operator!=(const const_iterator &rhs) const { return cur != rhs.cur; }

    /**
     * for the support of it->first.
     * See
     * <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/>
     * for help.
     */
    value_type *operator->() const noexcept {
      if (cur == nullptr or cur->data == nullptr)
        throw invalid_iterator(); // NOLINT
      return cur->data;
    }
    friend const_iterator;
    friend map;
  };
  class const_iterator {
    // it should has similar member method as iterator.
    //  and it should be able to construct from an iterator.
  private:
    node *cur;
    const map *_map;

  public:
    using key_type = Key;
    using value = T;
    friend map;
    friend iterator;
    const_iterator() {
      cur = nullptr;
      _map = nullptr;
    }
    const_iterator(node *nod, const map *m) {
      cur = nod;
      _map = m;
    }
    const_iterator(const const_iterator &other) {
      cur = other.cur;
      _map = other._map;
    }
    const_iterator(const iterator &other) {
      cur = other.cur;
      _map = other._map;
    }
    const_iterator operator++(int) {
      const_iterator _tmp = *this;
      if (cur == nullptr or cur->data == nullptr)
        throw invalid_iterator();
      if (cur->right) {
        cur = cur->right;
        while (cur->left)
          cur = cur->left;
      } else {
        node *tmp = cur->father;
        while (tmp and cur == tmp->right) {
          cur = tmp;
          tmp = tmp->father;
        }
        cur = tmp;
      }
      return _tmp;
    }
    const_iterator &operator++() {
      if (cur == nullptr or cur->data == nullptr)
        throw invalid_iterator();
      if (cur->right) {
        cur = cur->right;
        while (cur->left)
          cur = cur->left;
      } else {
        node *tmp = cur->father;
        while (tmp and cur == tmp->right) {
          cur = tmp;
          tmp = tmp->father;
        }
        cur = tmp;
      }
      return *this;
    }
    const_iterator operator--(int) {
      const_iterator _tmp = *this;
      if (cur == nullptr)
        throw invalid_iterator();
      if (cur->left) {
        cur = cur->left;
        while (cur->right)
          cur = cur->right;
      } else {
        node *tmp = cur->father;
        while (tmp and cur == tmp->left) {
          cur = tmp;
          tmp = tmp->father;
        }
        if (tmp == nullptr)
          throw invalid_iterator();
        cur = tmp;
      }
      return _tmp;
    }
    const_iterator &operator--() {
      if (cur == nullptr)
        throw invalid_iterator();
      if (cur->left) {
        cur = cur->left;
        while (cur->right)
          cur = cur->right;
      } else {
        node *tmp = cur->father;
        while (tmp and cur == tmp->left) {
          cur = tmp;
          tmp = tmp->father;
        }
        if (tmp == nullptr)
          throw invalid_iterator();
        cur = tmp;
      }
      return *this;
    }
    /**
     * a operator to check whether two iterators are same (pointing to the same
     * memory).
     */
    const value_type &operator*() const {
      if (cur == nullptr)
        throw invalid_iterator();
      return *(cur->data);
    }
    const_iterator &operator=(const const_iterator &other) {
      if (this == &other)
        return *this;
      cur = other.cur;
      _map = other._map;
      return *this;
    }
    bool operator==(const iterator &rhs) const { return cur == rhs.cur; }
    bool operator==(const const_iterator &rhs) const { return cur == rhs.cur; }
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const { return cur != rhs.cur; }
    bool operator!=(const const_iterator &rhs) const { return cur != rhs.cur; }

    /**
     * for the support of it->first.
     * See
     * <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/>
     * for help.
     */
    const value_type *operator->() const noexcept {
      if (cur == nullptr or cur->data == nullptr)
        throw invalid_iterator(); // NOLINT
      return cur->data;
    }
  };

  struct node {
    int height;
    value_type *data;
    node *left, *right, *father;
    node(value_type *d = nullptr, int h = 1, node *l = nullptr,
         node *r = nullptr, node *f = nullptr)
        : data(d), height(h), left(l), right(r), father(f) {}
    node(const node &other) {
      data = new value_type(*other.data);
      height = other.height;
    }
    ~node() { delete data; }
  };
  node *root = nullptr;
  size_t sz = 0;
  node *copy_tree(node *des, node *src) {
    if (src == nullptr)
      return nullptr;
    if (src->data == nullptr)
      des = new node();
    else
      des = new node(*src);
    des->left = copy_tree(des->left, src->left);
    if (des->left)
      des->left->father = des;
    des->right = copy_tree(des->right, src->right);
    if (des->right)
      des->right->father = des;
    return des;
  }
  void clear_tree(node *&cur) {
    if (cur == nullptr)
      return;
    clear_tree(cur->left);
    clear_tree(cur->right);
    delete cur;
    cur = nullptr;
  }
  node *find_node(const Key &key) const {
    node *cur = root;
    while (cur) {
      if (cur->data == nullptr or compare(key, cur->data->first))
        cur = cur->left;
      else if (compare(cur->data->first, key))
        cur = cur->right;
      else
        return cur;
    }
    return nullptr;
  }
  bool compare(const Key &a, const Key &b) const { return Compare()(a, b); }
  bool compare(const Key *a, const Key *b) const {
    if (a == nullptr)
      return false; // nullptr is the largest
    if (b == nullptr)
      return true;
    return Compare()(*a, *b);
  }
  bool compare(value_type *a, value_type *b) const {
    if (a == nullptr)
      return false; // nullptr is the largest
    if (b == nullptr)
      return true;
    return Compare()(a->first, b->first);
  }
  bool compare(node *a, node *b) {
    if (a == nullptr)
      return false; // nullptr is the largest
    if (b == nullptr)
      return true;
    return Compare()(*(a->data->first), *(b->data->first));
  }
  map() {
    root = new node();
    sz = 0;
  }
  map(const map &other) {
    root = copy_tree(root, other.root);
    sz = other.sz;
  }
  map &operator=(const map &other) {
    if (this == &other)
      return *this;
    clear_tree(root);
    root = copy_tree(root, other.root);
    sz = other.sz;
    return *this;
  }
  ~map() { clear_tree(root); }
  T &at(const Key &key) {
    node *cur = find_node(key);
    if (cur == nullptr)
      throw index_out_of_bound();
    return cur->data->second;
  }
  const T &at(const Key &key) const {
    node *cur = find_node(key);
    if (cur == nullptr)
      throw index_out_of_bound();
    return cur->data->second;
  }
  T &operator[](const Key &key) {
    node *cur = find_node(key);
    if (cur == nullptr) {
      value_type *p = new value_type(key, T());
      insertNode(root, p, cur);
      root->father = nullptr;
    }
    return cur->data->second;
  }
  /**
   * behave like at() throw index_out_of_bound if such key does not exist.
   */
  const T &operator[](const Key &key) const {
    node *cur = find_node(key);
    if (cur == nullptr)
      throw index_out_of_bound();
    return cur->data->second;
  }
  /**
   * return a iterator to the beginning
   */
  iterator begin() {
    node *cur = root;
    while (cur->left)
      cur = cur->left;
    return iterator(cur, this);
  }
  const_iterator cbegin() const {
    node *cur = root;
    while (cur->left)
      cur = cur->left;
    return const_iterator(cur, this);
  }
  /**
   * return a iterator to the end
   * in fact, it returns past-the-end.
   */
  iterator end() {
    node *cur = root;
    while (cur->right)
      cur = cur->right;
    return iterator(cur, this);
  }
  const_iterator cend() const {
    node *cur = root;
    while (cur->right)
      cur = cur->right;
    return const_iterator(cur, this);
  }

  /**
   * checks whether the container is empty
   * return true if empty, otherwise false.
   */
  bool empty() const { return sz == 0; }
  /**
   * returns the number of elements.
   */
  size_t size() const { return sz; }

  /**
   * clears the contents
   */
  void clear() {
    clear_tree(root);
    root = new node();
    sz = 0;
  }
  int getHeight(node *nod) {
    if (nod == nullptr) {
      return 0;
    }
    return nod->height;
  }

  int getBalanceFactor(node *nod) {
    if (nod == nullptr) {
      return 0;
    }
    return getHeight(nod->left) - getHeight(nod->right);
  }

  void updateHeight(node *nod) {
    if (nod == nullptr) {
      return;
    }
    nod->height = std::max(getHeight(nod->left), getHeight(nod->right)) + 1;
  }

  node *rotateLeft(node *nod) {
    node *newRoot = nod->right;
    nod->right = newRoot->left;
    if (nod->right)
      nod->right->father = nod;
    newRoot->left = nod;
    newRoot->father = nod->father;
    if (nod)
      nod->father = newRoot;
    updateHeight(nod);
    updateHeight(newRoot);
    return newRoot;
  }

  node *rotateRight(node *nod) {
    node *newRoot = nod->left;
    nod->left = newRoot->right;
    if (nod->left)
      nod->left->father = nod;
    newRoot->right = nod;
    newRoot->father = nod->father;
    if (nod)
      nod->father = newRoot;
    updateHeight(nod);
    updateHeight(newRoot);
    return newRoot;
  }

  node *balance(node *nod) {
    if (nod == nullptr) {
      return nullptr;
    }
    updateHeight(nod);
    int balanceFactor = getBalanceFactor(nod);
    if (balanceFactor > 1) {
      if (getBalanceFactor(nod->left) < 0) {
        nod->left = rotateLeft(nod->left);
        if (nod->left) {
          nod->left->father = nod;
        }
      }
      return rotateRight(nod);
    } else if (balanceFactor < -1) {
      if (getBalanceFactor(nod->right) > 0) {
        nod->right = rotateRight(nod->right);
        if (nod->right) {
          nod->right->father = nod;
        }
      }
      return rotateLeft(nod);
    }
    return nod;
  }

  void insertNode(node *&nod, value_type *value, node *&answer) {
    node *cur, *pre;
    bool left;
    cur = nod;
    while (cur) {
      pre = cur;
      if (compare(value, cur->data)) {
        cur = cur->left;
        left = true;
      } else if (compare(cur->data, value)) {
        cur = cur->right;
        left = false;
      } else {
        answer = cur;
        return;
      }
    }
    sz++;
    answer = new node(value);
    if (left) {
      pre->left = answer;
      answer->father = pre;
    } else {
      pre->right = answer;
      answer->father = pre;
    }
    cur = pre;
    pre = pre->father;
    int old;
    while (pre) {
      if (cur == pre->left) {
        old = cur->height;
        pre->left = balance(cur);
        cur = pre->left;
        if (cur)
          cur->father = pre;
        if (old == pre->left->height)
          return;
      } else {
        old = cur->height;
        pre->right = balance(cur);
        cur = pre->right;
        if (cur)
          cur->father = pre;
        if (old == pre->right->height)
          return;
      }
      cur = pre;
      pre = pre->father;
    }
    nod = cur;
  }

  node *findMinNode(node *nod) {
    if (nod == nullptr) {
      return nullptr;
    }
    while (nod->left != nullptr) {
      nod = nod->left;
    }
    return nod;
  }

  node *removeMinNode(node *nod) {
    if (nod == nullptr) {
      return nullptr;
    }
    if (nod->left == nullptr) {
      return nod->right;
    }
    nod->left = removeMinNode(nod->left);
    if (nod->left)
      nod->left->father = nod;
    return balance(nod);
  }

  node *removeNode(node *nod, const Key &key) {
    if (nod == nullptr) {
      return nullptr;
    }
    if (nod->data == nullptr or compare(key, nod->data->first)) {
      nod->left = removeNode(nod->left, key);
      if (nod->left)
        nod->left->father = nod;
    } else if (compare(nod->data->first, key)) {
      nod->right = removeNode(nod->right, key);
      if (nod->right)
        nod->right->father = nod;
    } else {
      node *leftChild = nod->left;
      node *rightChild = nod->right;
      delete nod;
      sz--;
      if (rightChild == nullptr) {
        return leftChild;
      }
      if (leftChild == nullptr) {
        return rightChild;
      }
      node *minNode = findMinNode(rightChild);
      minNode->right = removeMinNode(rightChild);
      if (minNode->right)
        minNode->right->father = minNode;
      minNode->left = leftChild;
      if (minNode->left)
        minNode->left->father = minNode;
      return balance(minNode);
    }
    return balance(nod);
  }

  /**
   * insert an element.
   * return a pair, the first of the pair is
   *   the iterator to the new element (or the element that prevented the
   * insertion), the second one is true if insert successfully, or false.
   */
  pair<iterator, bool> insert(const value_type &_value) {
    node *answer;
    answer = find_node(_value.first);
    if (answer != nullptr)
      return pair<iterator, bool>(iterator(answer, this), false);
    value_type *p = new value_type(_value);
    insertNode(root, p, answer);
    root->father = nullptr;
    return pair<iterator, bool>(iterator(answer, this), true);
  }
  /**
   * erase the element at pos.
   *
   * throw if pos pointed to a bad element (pos == this->end() || pos points
   * an element out of this)
   */
  void erase(iterator pos) {
    if (pos._map != this or pos == end())
      throw invalid_iterator();
    root = removeNode(root, pos.cur->data->first);
    root->father = nullptr;
  }
  /**
   * Returns the number of elements with key
   *   that compares equivalent to the specified argument,
   *   which is either 1 or 0
   *     since this container does not allow duplicates.
   * The default method of check the equivalence is !(a < b || b > a)
   */
  size_t count(const Key &key) const {
    node *cur = find_node(key);
    return cur == nullptr ? 0 : 1;
  }
  /**
   * Finds an element with key equivalent to key.
   * key value of the element to search for.
   * Iterator to an element with key equivalent to key.
   *   If no such element is found, past-the-end (see end()) iterator is
   * returned.
   */
  iterator find(const Key &key) {
    node *cur = find_node(key);
    return cur == nullptr ? end() : iterator(cur, this);
  }
  const_iterator find(const Key &key) const {
    node *cur = find_node(key);
    return cur == nullptr ? cend() : const_iterator(cur, this);
  }
};

} // namespace sjtu

#endif

using namespace sjtu;
template <class T, int degree = 60> class BPT {
public:
  struct node {
    bool is_leaf;
    T key[degree];
    int size;
    int son[degree];
    int next;
    node() {
      std::memset(this, 0, sizeof(node));
      is_leaf = false;
      next = -1;
    }
  };
  struct buffer {
    int pos = 0;
    node *x = nullptr;
    buffer *pre = nullptr, *next = nullptr;
  } *head = nullptr, *tail = nullptr;
  int length = 0;
  const int Maxlength = 600;
  map<int, buffer *> buffer_map;
  node *root = nullptr;
  fstream file;
  vector<node *> line;
  vector<int> _line;
  vector<node *> stack;
  vector<T *> stack_key;
  vector<T *> replace_key;
  vector<int> pos;
  std::string filename;
  BPT(const std::string &filename = "") {
    this->filename = filename;
    initial();
  }
  ~BPT() { exit(); }
  void exit() {
    while (length) {
      pop();
    }
    delete root;
    file.close();
  }
  void initial() {
    root = nullptr;
    ifstream file2("BPT" + filename);
    if (!file2.good()) {
      file.open("BPT" + filename, ios::out);
      file.close();
      file.open("BPT" + filename, ios::in | ios::out);
    } else {
      file.open("BPT" + filename, ios::in | ios::out);
      read(0, root);
      file2.close();
    }
  }
  void clear() {
    exit();
    buffer_map.clear();
    line.clear();
    _line.clear();
    stack.clear();
    stack_key.clear();
    replace_key.clear();
    pos.clear();
    file.open("BPT" + filename, ios::out);
    file.close();
    initial();
  }
  void pop() {
    if (length == 0)
      return;
    length--;
    buffer *tmp = tail;
    buffer_map.erase(buffer_map.find(tail->pos));
    file.seekp(tmp->pos);
    file.write(reinterpret_cast<char *>(tmp->x), sizeof(node));
    if (head == tail) {
      delete tail->x;
      delete tail;
      head = tail = nullptr;
      return;
    }
    tail = tail->pre;
    if (tail != nullptr)
      tail->next = nullptr;
    delete tmp->x;
    delete tmp;
  }
  void write(int pos, node *x) {
    if (buffer_map.find(pos) != buffer_map.end()) {
      buffer *tmp = buffer_map[pos];
      tmp->x->is_leaf = x->is_leaf;
      tmp->x->size = x->size;
      for (int i = 0; i < x->size; i++) {
        tmp->x->key[i] = x->key[i];
        tmp->x->son[i] = x->son[i];
      }
      tmp->x->next = x->next;
      if (tmp == head) {
        return;
      }
      tmp->pre->next = tmp->next;
      if (tmp != tail)
        tmp->next->pre = tmp->pre;
      else
        tail = tmp->pre;
      tmp->next = head;
      head->pre = tmp;
      head = tmp;
      return;
    }
    file.seekp(pos);
    file.write(reinterpret_cast<char *>(x), sizeof(node));
  }
  int append(node *x) {
    file.seekp(0, ios::end);
    int t = file.tellp();
    if (buffer_map.find(t) != buffer_map.end()) {
      buffer *p = buffer_map[t];
      p->x->is_leaf = x->is_leaf;
      p->x->size = x->size;
      for (int i = 0; i < x->size; i++) {
        p->x->key[i] = x->key[i];
        p->x->son[i] = x->son[i];
      }
      p->x->next = x->next;
      if (p == head) {
        return t;
      }
      p->pre->next = p->next;
      if (p != tail) {
        p->next->pre = p->pre;
      } else {
        tail = p->pre;
      }
      p->next = head;
      head->pre = p;
      head = p;
      return t;
    }
    file.write(reinterpret_cast<char *>(x), sizeof(node));
    return t;
  }
  void read(int pos, node *&x) {
    if (buffer_map.find(pos) != buffer_map.end()) {
      buffer *t = buffer_map[pos];
      x = new node(*(t->x));
      if (t == head) {
        return;
      }
      t->pre->next = t->next;
      if (t != tail) {
        t->next->pre = t->pre;
      } else {
        tail = t->pre;
      }
      t->next = head;
      head->pre = t;
      head = t;
      return;
    }
    x = new node();
    file.seekg(pos);
    file.read(reinterpret_cast<char *>(x), sizeof(node));
    buffer *tmp = new buffer;
    tmp->pos = pos;
    tmp->x = new node(*x);
    tmp->next = head;
    if (head != nullptr) {
      head->pre = tmp;
    }
    head = tmp;
    if (tail == nullptr) {
      tail = tmp;
    }
    length++;
    buffer_map.insert(pair<int, buffer *>(pos, tmp));
    if (length == Maxlength) {
      pop();
    }
  }
  void insert(T &x) {
    line.clear();
    _line.clear();
    if (root == nullptr) {
      root = new node();
      root->is_leaf = true;
      root->key[0] = x;
      root->size = 1;
      write(0, root);
      return;
    }
    int top = 0;
    _line.push_back(0);
    read(0, line[0]);
    while (!line[top]->is_leaf) {
      int i = 0, r = line[top]->size, mid;
      while (i < r) {
        mid = (i + r) / 2;
        if (line[top]->key[mid] > x)
          r = mid;
        else
          i = mid + 1;
      }
      if (i > 0)
        i--;
      _line[top + 1] = line[top]->son[i];
      top++;
      read(_line[top], line[top]);
    }
    int i = 0, r = line[top]->size, mid;
    while (i < r) {
      mid = (i + r) / 2;
      if (line[top]->key[mid] > x)
        r = mid;
      else
        i = mid + 1;
    }
    for (int j = line[top]->size; j > i; j--) {
      line[top]->key[j] = line[top]->key[j - 1];
    }
    line[top]->key[i] = x;
    line[top]->size++;
    int tmp_top = top;
    while (i == 0 and top > 0) {
      while (i < line[top - 1]->size and x > line[top - 1]->key[i])
        i++;
      line[top - 1]->key[i] = x;
      write(_line[top - 1], line[top - 1]);
      top--;
    }
    top = tmp_top;
    if (line[top]->size < degree) {
      write(_line[top], line[top]);
      delete line[top];
      while (top--) {
        delete line[top];
      }
      return;
    }
    node *new_node = new node();
    new_node->is_leaf = true;
    for (int i = 0; i < degree / 2; i++) {
      new_node->key[i] = line[top]->key[i + (degree + 1) / 2];
      new_node->son[i] = line[top]->son[i + (degree + 1) / 2];
    }
    new_node->size = degree / 2;
    line[top]->size -= degree / 2;
    new_node->next = line[top]->next;
    int pos = append(new_node);
    T y = new_node->key[0];
    delete new_node;
    line[top]->next = pos;
    if (top == 0) {
      int old_root = append(line[0]);
      node *new_root = new node();
      new_root->is_leaf = false;
      new_root->key[0] = line[0]->key[0];
      new_root->key[1] = y;
      new_root->size = 2;
      new_root->son[0] = old_root;
      new_root->son[1] = pos;
      write(0, new_root);
      delete new_root;
      delete line[top];
      while (top--) {
        delete line[top];
      }
      return;
    }
    write(_line[top], line[top]);
    delete line[top];
    top--;
    i = 0, r = line[top]->size;
    while (i < r) {
      mid = (i + r) / 2;
      if (line[top]->key[mid] > y)
        r = mid;
      else
        i = mid + 1;
    }
    for (int j = line[top]->size; j > i; j--) {
      line[top]->key[j] = line[top]->key[j - 1];
      line[top]->son[j] = line[top]->son[j - 1];
    }
    line[top]->key[i] = y;
    line[top]->son[i] = pos;
    line[top]->size++;
    tmp_top = top;
    while (i == 0 and top > 0) {
      while (i < line[top - 1]->size and y > line[top - 1]->key[i])
        i++;
      line[top - 1]->key[i] = y;
      write(_line[top - 1], line[top - 1]);
      top--;
    }
    top = tmp_top;
    if (line[top]->size < degree) {
      write(_line[top], line[top]);
      delete line[top];
      while (top--) {
        delete line[top];
      }
      return;
    }
    while (top > 0 and line[top]->size == degree) {
      node *new_node = new node();
      for (int i = 0; i < degree / 2; i++) {
        new_node->key[i] = line[top]->key[i + (degree + 1) / 2];
        new_node->son[i] = line[top]->son[i + (degree + 1) / 2];
      }
      new_node->size = degree / 2;
      line[top]->size -= degree / 2;
      new_node->next = line[top]->next;
      pos = append(new_node);
      line[top]->next = pos;
      write(_line[top], line[top]);
      delete line[top];
      top--;
      y = new_node->key[0];
      delete new_node;
      i = 0, r = line[top]->size;
      while (i < r) {
        mid = (i + r) / 2;
        if (line[top]->key[mid] > y)
          r = mid;
        else
          i = mid + 1;
      }
      for (int j = line[top]->size; j > i; j--) {
        line[top]->key[j] = line[top]->key[j - 1];
        line[top]->son[j] = line[top]->son[j - 1];
      }
      line[top]->key[i] = y;
      line[top]->son[i] = pos;
      line[top]->size++;
      tmp_top = top;
      while (i == 0 and top > 0) {
        while (i < line[top - 1]->size and y > line[top - 1]->key[i])
          i++;
        line[top - 1]->key[i] = y;
        write(_line[top - 1], line[top - 1]);
        top--;
      }
      top = tmp_top;
    }
    if (top == 0 and line[0]->size == degree) {
      node *new_node = new node();
      for (int i = 0; i < degree / 2; i++) {
        new_node->key[i] = line[top]->key[i + (degree + 1) / 2];
        new_node->son[i] = line[top]->son[i + (degree + 1) / 2];
      }
      new_node->size = degree / 2;
      line[top]->size -= degree / 2;
      new_node->next = line[top]->next;
      pos = append(new_node);
      line[top]->next = pos;
      int old_root = append(line[top]);
      y = new_node->key[0];
      delete new_node;
      node *new_root = new node();
      new_root->is_leaf = false;
      new_root->key[0] = line[0]->key[0];
      new_root->key[1] = y;
      new_root->size = 2;
      new_root->son[0] = old_root;
      new_root->son[1] = pos;
      write(0, new_root);
      delete new_root;
    } else if (line[top]->size < degree) {
      write(_line[top], line[top]);
    }
    delete line[top];
    while (top--) {
      delete line[top];
    }
  }

  void erase(T &x) {
    line.clear();
    _line.clear();
    stack.clear();
    stack_key.clear();
    pos.clear();
    replace_key.clear();
    if (root == nullptr) {
      return;
    }
    int top = 0;
    _line[top] = 0;
    read(0, line[0]);
    while (!line[top]->is_leaf) {
      int i = 0, r = line[top]->size, mid;
      while (i < r) {
        mid = (i + r) / 2;
        if (line[top]->key[mid] > x)
          r = mid;
        else
          i = mid + 1;
      }
      if (i > 0)
        i--;
      _line[top + 1] = line[top]->son[i];
      top++;
      read(_line[top], line[top]);
    }
    int tail = 0, head = 0;
    stack[0] = line[top];
    stack_key[0] = new T(x);
    pos[tail++] = top;
    while (head < tail) {
      node *tmp_node = stack[head];
      T *tmp_t = stack_key[head];
      int tmp_pos = pos[head];
      T *tmp_replace = replace_key[head];
      head++;
      int i = 0, r = tmp_node->size, mid;
      while (i < r) {
        mid = (i + r) / 2;
        if (tmp_node->key[mid] >= *tmp_t)
          r = mid;
        else
          i = mid + 1;
      }
      if (i == tmp_node->size or tmp_node->key[i] != *tmp_t) {
        delete line[top];
        while (top--) {
          delete line[top];
        }
        for (int i = 0; i < tail; i++) {
          delete stack_key[i];
          delete replace_key[i];
        }
        return;
      }
      if (tmp_replace == nullptr) {
        for (int j = i; j < tmp_node->size - 1; j++) {
          tmp_node->key[j] = tmp_node->key[j + 1];
          tmp_node->son[j] = tmp_node->son[j + 1];
        }
        tmp_node->size--;
        if (tmp_node->size == 0 and tmp_pos == 0) {
          delete root;
          root = nullptr;
          delete line[top];
          while (top--) {
            delete line[top];
          }
          for (int i = 0; i < tail; i++) {
            delete stack_key[i];
            delete replace_key[i];
          }
          return;
        }
        if (tmp_node->size == 0 and tmp_pos > 0) {
          stack[tail] = line[tmp_pos - 1];
          stack_key[tail] = new T(*tmp_t);
          pos[tail] = tmp_pos - 1;
          replace_key[tail] = nullptr;
          tail++;
          node *tmp = nullptr;
          int i = 0, r = line[tmp_pos - 1]->size, mid;
          while (i < r) {
            mid = (i + r) / 2;
            if (line[tmp_pos - 1]->key[mid] >= *tmp_t)
              r = mid;
            else
              i = mid + 1;
          }
          if (i > 0) {
            read(line[tmp_pos - 1]->son[i - 1], tmp);
            tmp->next = tmp_node->next;
            write(line[tmp_pos - 1]->son[i - 1], tmp);
          } else {
            int depth = 0;
            node *tmp2 = nullptr;
            int tmp_pos2 = 0;
            read(0, tmp2);
            while (depth < tmp_pos) {
              int i = 0, r = tmp2->size, mid;
              while (i < r) {
                mid = (i + r) / 2;
                if (tmp2->key[mid] >= tmp_node->key[0])
                  r = mid;
                else
                  i = mid + 1;
              }
              i--;
              if (i < 0) {
                delete tmp2;
                tmp2 = nullptr;
                break;
              }
              tmp_pos2 = tmp2->son[i];
              delete tmp2;
              read(tmp2->son[i], tmp2);
              depth++;
            }
            if (tmp2 != nullptr and tmp2 != tmp_node) {
              tmp2->next = tmp_node->next;
              write(tmp_pos2, tmp2);
            }
            delete tmp2;
          }
          delete tmp;
          delete stack_key[head - 1];
          stack_key[head - 1] = nullptr;
          delete replace_key[head - 1];
          replace_key[head - 1] = nullptr;
          continue;
        }
        if (tmp_pos > 0 and i == 0) {
          stack[tail] = line[tmp_pos - 1];
          stack_key[tail] = new T(*tmp_t);
          pos[tail] = tmp_pos - 1;
          replace_key[tail] = new T(tmp_node->key[0]);
          tail++;
        }
        if (tmp_node->size > degree / 2 or tmp_pos == 0 or
            _line[tmp_pos] ==
                line[tmp_pos - 1]->son[line[tmp_pos - 1]->size - 1]) {
          write(_line[tmp_pos], line[tmp_pos]);
          delete stack_key[head - 1];
          stack_key[head - 1] = nullptr;
          delete replace_key[head - 1];
          replace_key[head - 1] = nullptr;
          continue;
        }
        node *next = nullptr;
        read(line[tmp_pos]->next, next);
        if (next->size + line[tmp_pos]->size < degree) {
          for (int i = 0; i < next->size; i++) {
            line[tmp_pos]->key[line[tmp_pos]->size] = next->key[i];
            line[tmp_pos]->son[line[tmp_pos]->size] = next->son[i];
            line[tmp_pos]->size++;
          }
          line[tmp_pos]->next = next->next;
          write(_line[tmp_pos], line[tmp_pos]);
          stack[tail] = line[tmp_pos - 1];
          stack_key[tail] = new T(next->key[0]);
          pos[tail] = tmp_pos - 1;
          replace_key[tail] = nullptr;
          tail++;
          delete next;
          delete stack_key[head - 1];
          stack_key[head - 1] = nullptr;
          delete replace_key[head - 1];
          replace_key[head - 1] = nullptr;
          continue;
        }
        line[tmp_pos]->key[line[tmp_pos]->size] = next->key[0];
        line[tmp_pos]->son[line[tmp_pos]->size] = next->son[0];
        line[tmp_pos]->size++;
        stack_key[tail] = new T(next->key[0]);
        for (int i = 0; i < next->size - 1; i++) {
          next->key[i] = next->key[i + 1];
          next->son[i] = next->son[i + 1];
        }
        next->size--;
        write(_line[tmp_pos], line[tmp_pos]);
        write(line[tmp_pos]->next, next);
        stack[tail] = line[tmp_pos - 1];
        pos[tail] = tmp_pos - 1;
        replace_key[tail] = new T(next->key[0]);
        delete next;
        tail++;
      } else {
        tmp_node->key[i] = *tmp_replace;
        if (i == 0 and tmp_pos > 0) {
          stack[tail] = line[tmp_pos - 1];
          stack_key[tail] = new T(*tmp_t);
          pos[tail] = tmp_pos - 1;
          replace_key[tail] = new T(*tmp_replace);
          tail++;
        }
        write(_line[tmp_pos], tmp_node);
      }
      delete stack_key[head - 1];
      stack_key[head - 1] = nullptr;
      delete replace_key[head - 1];
      replace_key[head - 1] = nullptr;
    }
    delete line[top];
    while (top--) {
      delete line[top];
    }
    for (int i = 0; i < tail; i++) {
      delete stack_key[i];
      delete replace_key[i];
    }
  }
  vector<T> find(T &vmin, T &vmax) {
    vector<T> ans;
    if (root == nullptr) {
      return ans;
    }
    node *tmp = nullptr;
    read(0, tmp);
    while (!tmp->is_leaf) {
      int i = 0, r = tmp->size, mid;
      while (i < r) {
        mid = (i + r) / 2;
        if (tmp->key[mid] >= vmin)
          r = mid;
        else
          i = mid + 1;
      }
      if (i > 0)
        i--;
      int t = tmp->son[i];
      delete tmp;
      read(t, tmp);
    }
    while (tmp->key[tmp->size - 1] < vmin) {
      if (tmp->next == -1) {
        delete tmp;
        return ans;
      }
      int t = tmp->next;
      delete tmp;
      read(t, tmp);
    }
    int i = 0, r = tmp->size, mid;
    while (i < r) {
      mid = (i + r) / 2;
      if (tmp->key[mid] >= vmin)
        r = mid;
      else
        i = mid + 1;
    }
    if (i == tmp->size or tmp->key[i] > vmax or tmp->key[i] < vmin) {
      delete tmp;
      return ans;
    }
    while (tmp->key[i] <= vmax) {
      ans.push_back(tmp->key[i]);
      i++;
      if (i == tmp->size) {
        if (tmp->next == -1)
          break;
        int t = tmp->next;
        delete tmp;
        read(t, tmp);
        i = 0;
      }
    }
    delete tmp;
    return ans;
  }
  bool find_first(T &vmin, T &vmax, T &ans) {
    if (root == nullptr) {
      return false;
    }
    node *tmp = nullptr;
    read(0, tmp);
    while (!tmp->is_leaf) {
      int i = 0, r = tmp->size, mid;
      while (i < r) {
        mid = (i + r) / 2;
        if (tmp->key[mid] >= vmin)
          r = mid;
        else
          i = mid + 1;
      }
      if (i > 0)
        i--;
      int t = tmp->son[i];
      delete tmp;
      read(t, tmp);
    }
    while (tmp->key[tmp->size - 1] < vmin) {
      if (tmp->next == -1) {
        delete tmp;
        return false;
      }
      int t = tmp->next;
      delete tmp;
      read(t, tmp);
    }
    int i = 0, r = tmp->size, mid;
    while (i < r) {
      mid = (i + r) / 2;
      if (tmp->key[mid] >= vmin)
        r = mid;
      else
        i = mid + 1;
    }
    if (i == tmp->size or tmp->key[i] > vmax or tmp->key[i] < vmin) {
      delete tmp;
      return false;
    }
    ans = tmp->key[i];
    delete tmp;
    return true;
  }
  void print(int pos) {
    node *t = nullptr;
    read(pos, t);
    for (int i = 0; i < t->size; i++) {
      if (!t->is_leaf)
        print(t->son[i]);
      else
        cout << t->key[i] << " ";
    }
  }
};
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::string;

template <class T, int info_len = 1> class MemoryRiver {
private:
  int add_len = 0;
  fstream file;
  string file_name;
  int sizeofT = sizeof(T);
  struct buffer {
    int pos = 0;
    T x = {};
    buffer *pre = nullptr, *next = nullptr;
  } *head = nullptr, *tail = nullptr;
  int length = 0;
  const int Maxlength = 600;
  map<int, buffer *> buffer_map;

public:
  MemoryRiver() = default;

  MemoryRiver(const string &file_name) : file_name(file_name + "river") {
    initialise();
  }
  ~MemoryRiver() {
    while (length)
      pop();
    file.close();
  }
  void pop() {
    if (length == 0)
      return;
    length--;
    buffer *tmp = tail;
    buffer_map.erase(buffer_map.find(tail->pos));
    file.seekp(tail->pos * sizeofT + info_len * sizeof(int));
    file.write(reinterpret_cast<char *>(&tail->x), sizeof(T));
    if (head == tail) {
      delete tail;
      head = tail = nullptr;
      return;
    }
    tail = tail->pre;
    if (tail != nullptr)
      tail->next = nullptr;
    delete tmp;
  }
  void clear() {
    while (length)
      pop();
    file.close();
    file.open(file_name, std::ios::out);
    int tmp = 0;
    for (int i = 0; i < info_len; ++i)
      file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
    file.close();
    file.open(file_name, std::ios::in | std::ios::out);
  }
  void initialise(string FN = "") {
    if (FN != "")
      file_name = FN;
    file.open(file_name, std::ios::in);
    if (!file.is_open()) {
      file.open(file_name, std::ios::out);
      int tmp = 0;
      for (int i = 0; i < info_len; ++i)
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
      file.close();
    } else {
      file.close();
    }
    file.open(file_name, std::ios::in | std::ios::out);
  }

  //读出第n个int的值赋给tmp，1_base
  void get_info(int &tmp, int n) {
    if (n > info_len + add_len)
      return;
    file.seekg((n - 1) * sizeof(int));
    file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
  }

  //将tmp写入第n个int的位置，1_base
  void write_info(int tmp, int n) {
    if (n > info_len + add_len)
      return;
    file.seekg((n - 1) * sizeof(int));
    file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
  }

  //在文件合适位置写入类对象t，并返回写入的位置索引index
  //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
  //位置索引index可以取为对象写入的起始位置
  int write(T &t) {
    file.seekp(0, std::ios::end);
    int index = ((int)file.tellp() - info_len * sizeof(int)) / sizeofT;
    file.write(reinterpret_cast<char *>(&t), sizeof(T));
    add_len += sizeofT / sizeof(int);
    return index;
  }

  //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
  void update(T &t, const int index) {
    if (buffer_map.find(index) != buffer_map.end()) {
      buffer *tmp = buffer_map[index];
      tmp->x = t;
      if (tmp == head)
        return;
      tmp->pre->next = tmp->next;
      if (tmp != tail)
        tmp->next->pre = tmp->pre;
      else
        tail = tmp->pre;
      tmp->next = head;
      head->pre = tmp;
      head = tmp;
      return;
    }
    file.seekp(index * sizeofT + info_len * sizeof(int));
    file.write(reinterpret_cast<char *>(&t), sizeof(T));
  }

  //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
  void read(T &t, const int index) {
    if (buffer_map.find(index) != buffer_map.end()) {
      buffer *tmp = buffer_map[index];
      t = tmp->x;
      if (tmp == head)
        return;
      tmp->pre->next = tmp->next;
      if (tmp != tail)
        tmp->next->pre = tmp->pre;
      else
        tail = tmp->pre;
      tmp->next = head;
      head->pre = tmp;
      head = tmp;
      return;
    }
    file.seekg(index * sizeofT + info_len * sizeof(int));
    file.read(reinterpret_cast<char *>(&t), sizeof(T));
    buffer *tmp = new buffer;
    tmp->pos = index;
    tmp->x = t;
    tmp->next = head;
    if (head != nullptr) {
      head->pre = tmp;
    }
    head = tmp;
    if (tail == nullptr) {
      tail = tmp;
    }
    length++;
    buffer_map.insert(pair<int, buffer *>(index, tmp));
    if (length == Maxlength) {
      pop();
    }
  }

  //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
  void Delete(int index) {}
};

#endif