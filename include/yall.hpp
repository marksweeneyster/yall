#ifndef YALL_INCLUDE_YALL_HPP
#define YALL_INCLUDE_YALL_HPP

#include <functional>
#include <iostream>
#include <memory>
#include <optional>

namespace yall {

  //! Generic doubly linked-list.
  //!  The implementation allows for "T" to be a reference type but this can
  //!  to undefined behavior if the list's reference outlives the client's
  //!  value that the reference refers to.
  //!
  //!* \tparam T The type of the node data.
  template<typename T>
  class Yall final {
    /* getter functions further below require a non-reference type  */
    using DecayT = typename std::decay<T>::type;

    struct Node {
      explicit Node(const T& data_) : data(data_) {}

      const T data;
      std::weak_ptr<Node> prev;
      std::shared_ptr<Node> next;
    };
    using NodePtr = std::shared_ptr<Node>;

  public:
    Yall()  = default;
    ~Yall() = default;

    Yall(const Yall&)            = delete;
    Yall(Yall&&)                 = delete;
    Yall& operator=(const Yall&) = delete;
    Yall& operator=(Yall&&)      = delete;

    //! Insert a new node at the front of the list.
    //! \param data node value
    void push_front(const T& data) {
      auto node_ptr = std::make_shared<Node>(data);
      if (head) {
        node_ptr->next = head;
        head->prev     = node_ptr;
        head           = node_ptr;
      } else {
        head = node_ptr;
        tail = node_ptr;
      }
    }

    //! Insert a new node at the back of the list.
    //! \param data node value
    void push_back(const T& data) {
      auto node_ptr = std::make_shared<Node>(data);
      if (auto old_tail = tail.lock()) {
        node_ptr->prev = old_tail;
        old_tail->next = node_ptr;
        tail           = node_ptr;
      } else {
        head = node_ptr;
        tail = node_ptr;
      }
    }

    //! Removes the first element in the linked list
    void pop_front() {
      if (head) {
        auto new_head = head->next;
        if (new_head) {
          head.reset();
          head = new_head;
        } else {
          head.reset();
          tail.reset();
        }
      }
    }

    //! Removes the last element in the linked list.
    void pop_back() {
      if (auto old_tail = tail.lock()) {
        auto new_tail = old_tail->prev.lock();
        if (new_tail) {
          new_tail->next.reset();
          tail = new_tail;
        } else {
          head.reset();
          tail.reset();
        }
      }
    }

    //! This method will make a copy of the node data, which may be costly.
    //!
    //! \return a copy of the value at the front of the list, or none.
    std::optional<DecayT> front_val() const {
      if (head) {
        DecayT val = head->data;
        return val;
      }
      return {};
    }

    //! This method will make a copy of the node data, which may be costly.
    //!
    //! \return a copy of the value at the back of the list, or none.
    std::optional<DecayT> back_val() const {
      if (auto sp = tail.lock()) {
        DecayT val = sp->data;
        return val;
      }
      return {};
    }

    //! Get the value at the front of the list
    //! \param ref Output
    //! \return true if the list is not-empty and the reference has been assigned
    bool front(T& ref) const {
      if (head) {
        ref = head->data;
        return true;
      }
      return false;
    }

    //! Get the value at the back of the list
    //! \param ref Output
    //! \return true if the list is not-empty and the reference has been assigned
    bool back(T& ref) const {
      if (auto sp = tail.lock()) {
        ref = sp->data;
        return true;
      }
      return false;
    }

    //! Start from the front of the list, find the first match, and remove it.
    //! \param match_val
    //! \return true if the value was found and removed, otherwise false
    bool remove_first(const T& match_val) {
      if (head) {
        if (head->data == match_val) {
          pop_front();
          return true;
        }
        auto ptr = head->next;
        while (ptr) {
          if (ptr->data == match_val) {
            if (ptr == tail.lock()) {
              pop_back();
              return true;
            }
            //
            auto prev_node = ptr->prev.lock();
            auto next_node = ptr->next;
            ptr.reset();
            prev_node->next = next_node;
            next_node->prev = prev_node;
            return true;
          }
          ptr = ptr->next;
        }
      }
      return false;
    }

    //! Start from the back of the list, find the first match, and remove it.
    //! \param match_val
    //! \return true if the value was found and removed, otherwise false
    bool remove_last(const T& match_val) {
      if (auto sp = tail.lock()) {
        if (sp->data == match_val) {
          pop_back();
          return true;
        }
        auto ptr = sp->prev;
        while ((sp = ptr.lock())) {
          if (sp->data == match_val) {
            if (sp == head) {
              pop_front();
              return true;
            }
            auto prev_node = sp->prev.lock();
            auto next_node = sp->next;
            sp.reset();
            prev_node->next = next_node;
            next_node->prev = prev_node;
            return true;
          }
          ptr = sp->prev;
        }
      }
      return false;
    }

    using PrinterCB = std::function<void(const T&)>;

    //! Print the values in the list, front-to-back.
    //!
    //! \param printer_cb callback that will print node data to stdout
    void print(PrinterCB printer_cb) const {
      auto ptr = head;
      while (ptr) {
        printer_cb(ptr->data);
        ptr = ptr->next;
      }
      std::cout << "|-\n";// list display "null-terminator"
    }

    //! Free all nodes (create an empty list).
    void reset() noexcept {
      auto ptr = tail.lock();
      while (ptr) {
        if (ptr->next) {
          ptr->next.reset();
        }
        ptr = ptr->prev.lock();
      }
      head.reset();
      tail.reset();
    }

    //! \return whether the linked list is empty
    bool empty() const { return !(head || tail.lock()); }

    size_t size() const {
      size_t sz = 0;
      auto ptr = head;
      while (ptr) {
        ++sz;
        ptr = ptr->next;
      }
      return sz;
    }

  private:
    NodePtr head;
    std::weak_ptr<Node> tail;

  public:
    struct ConstIterator {
      // iterator traits
      using iterator_category = std::bidirectional_iterator_tag;
      using difference_type   = std::ptrdiff_t;// TODO is this correct?
      using value_type        = DecayT;
      using pointer           = NodePtr;
      using reference         = const DecayT&;

      explicit ConstIterator() : m_ptr(nullptr) {}
      explicit ConstIterator(pointer ptr) : m_ptr(ptr) {}
      explicit ConstIterator(std::weak_ptr<Node> ptr) : m_ptr(ptr.lock()) {}

      reference operator*() const { return m_ptr->data; }
      pointer operator->() { return m_ptr; }

      ConstIterator& operator++() {
        m_ptr = m_ptr->next;
        return *this;
      }

      ConstIterator operator++(int) {
        ConstIterator tmp = *this;
        ++(*this);
        return tmp;
      }

      ConstIterator& operator--() {
        m_ptr = m_ptr->prev.lock();
        return *this;
      }

      ConstIterator operator--(int) {
        ConstIterator tmp = *this;
        --(*this);
        return tmp;
      }

      friend bool operator==(const ConstIterator& a, const ConstIterator& b) {
        return a.m_ptr == b.m_ptr;
      };
      friend bool operator!=(const ConstIterator& a, const ConstIterator& b) {
        return a.m_ptr != b.m_ptr;
      };

    private:
      pointer m_ptr;
    };

    ConstIterator cbegin() { return ConstIterator(head); }
    ConstIterator cend() { return ConstIterator(); }
    // allows range-based for loops with Yall containers
    ConstIterator begin() { return cbegin(); }
    ConstIterator end() { return cend(); }

    ConstIterator crbegin() { return ConstIterator(tail); }
    ConstIterator crend() { return ConstIterator(); }
  };
}// namespace yall


#endif//YALL_INCLUDE_YALL_HPP
