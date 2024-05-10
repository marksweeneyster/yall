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
    /* in particular, if "T" is a reference type. get the base type */
    using DecayT = typename std::decay<T>::type;

    struct Node {
      explicit Node(const T& data_) : data(data_) {}
      //~Node() { std::cout << "dtor: " << data << std::endl; }

      // "Each node should contain a reference to application data"
      const T data;
      std::shared_ptr<Node> prev;
      std::shared_ptr<Node> next;
    };
    using NodePtr = std::shared_ptr<Node>;

  public:
    Yall() = default;

    ~Yall() {
      /* Since "prev" and "next" are shared_ptrs I need to explicitly free up
       * any allocated nodes due to circular references.
       * I tested with weak_ptrs but had the opposite problem, ref counts
       * going to zero too soon.
       */
      reset();
    }
    /* since I have a custom destructor follow the rule-of-five */
    Yall(const Yall&)            = delete;
    Yall(Yall&&)                 = delete;
    Yall& operator=(const Yall&) = delete;
    Yall& operator=(Yall&&)      = delete;

    //! Insert a new node at the front of the list.
    //! \param data node value
    void push_front(const T& data) {
      auto node_ptr = std::make_shared<Node>(data);
      if (head) {
        if (head == tail) {
          tail->prev     = node_ptr;
          node_ptr->next = tail;
        } else {
          node_ptr->next = head;
          head->prev     = node_ptr;
        }
        head = node_ptr;
      } else {
        head = node_ptr;
        tail = node_ptr;
      }
    }

    //! Insert a new node at the back of the list.
    //! \param data node value
    void push_back(const T& data) {
      auto node_ptr = std::make_shared<Node>(data);
      if (tail) {
        if (head == tail) {
          head->next     = node_ptr;
          node_ptr->prev = head;
        } else {
          node_ptr->prev = tail;
          tail->next     = node_ptr;
        }
        tail = node_ptr;
      } else {
        head = node_ptr;
        tail = node_ptr;
      }
    }

    //! Removes the first element in the linked list
    void pop_front() {
      if (head) {
        auto new_head = head->next;
        if (new_head && new_head->prev) {
          new_head->prev.reset();
        }
        head = new_head;
      }
    }

    //! Removes the last element in the linked list.
    void pop_back() {
      if (tail) {
        auto new_tail = tail->prev;
        if (new_tail && new_tail->next) {
          new_tail->next.reset();
        }
        tail = new_tail;
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
      if (tail) {
        DecayT val = tail->data;
        return val;
      }
      return {};
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
            if (ptr == tail) {
              pop_back();
              return true;
            }
            //
            auto prev_node = ptr->prev;
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
      if (tail) {
        if (tail->data == match_val) {
          pop_back();
          return true;
        }
        auto ptr = tail->prev;
        while (ptr) {
          if (ptr->data == match_val) {
            if (ptr == head) {
              pop_front();
              return true;
            }
            //
            auto prev_node = ptr->prev;
            auto next_node = ptr->next;
            ptr.reset();
            prev_node->next = next_node;
            next_node->prev = prev_node;
            return true;
          }
          ptr = ptr->prev;
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
      std::cout << "|-\n";// list "null-terminator"
    }

    //! Free all nodes (create an empty list).
    void reset() noexcept {
      auto ptr = head;
      while (ptr) {
        if (ptr->prev) {
          ptr->prev.reset();
        }
        ptr = ptr->next;
      }
      head.reset();
      tail.reset();
    }

    //! \return whether the linked list is empty
    bool empty() const { return !(head && tail); }

  private:
    NodePtr head;
    NodePtr tail;
  };
}// namespace yall


#endif//YALL_INCLUDE_YALL_HPP
