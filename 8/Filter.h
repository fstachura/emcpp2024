#ifndef LAB6_FILTER_H
#define LAB6_FILTER_H

#include <cstddef>
#include <iterator>
#include <utility>

template<typename Container, typename Predicate>
class Filter {
  Container container;
  Predicate predicate;

public:
  Filter(Container container, Predicate predicate): container(container), predicate(predicate) {
  }

  class iterator {
    Filter<Container, Predicate>& f;
    decltype(f.container.begin()) it;

  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = typename decltype(f.container.begin())::difference_type;
    using value_type = typename decltype(f.container.begin())::value_type;
    using pointer = typename decltype(f.container.begin())::pointer;
    using reference = typename decltype(f.container.begin())::reference;

    iterator(Filter<Container, Predicate>& f): f(f), it(f.container.begin()) {
      update();
    }

    iterator(Filter<Container, Predicate>& f, decltype(f.container.begin()) it): f(f), it(it) {
      update();
    }

    void update() {
      while(it != f.container.end()) {
        if(it != f.container.end() && f.predicate(*it)) {
          break;
        }
        it++;
      }
    }

    reference operator*() {
      return *it;
    }

    pointer operator->() {
      return it.operator->();
    }

    iterator& operator++() {
      if(it != f.container.end()) {
        it++;
        update();
      }
      return *this;
    }

    iterator operator++(int) {
      auto tmp = *this;
      operator++();
      return tmp;
    }

    friend bool operator==(const iterator& a, const iterator& b) {
      return a.it == b.it;
    }

    friend bool operator!=(const iterator& a, const iterator& b) {
      return a.it != b.it;
    }
  };

  iterator begin() {
    return iterator(*this);
  }

  iterator end() {
    return iterator(*this, container.end());
  }
};

template<typename Container, typename Predicate>
auto make_filter(Container&& c, Predicate p) {
  return Filter<Container, Predicate>(std::forward<Container>(c), p);
}

#endif //LAB6_FILTER_H
