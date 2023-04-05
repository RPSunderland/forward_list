
#ifndef FORWARD_LIST_HPP
#define FORWARD_LIST_HPP

#include <memory>
#include <functional>
#include <limits>

namespace rafal {
    template<class T, class Allocator = std::allocator<T>>
    class forward_list {
    private:

        /**
         * help structures
         */
        using SplitPredicate = std::function<bool(const T &value)>;

        struct Node {
            Node() : data(T{}), next(nullptr) {}

            Node(T data, Node* next) : data(data), next(next) {}

            T data;
            Node* next = nullptr;
        };

        struct Iterator {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;

            explicit Iterator(Node* node_ptr) : node_ptr(node_ptr) {}

            reference operator*() const { return node_ptr->data; }

            pointer operator->() const { return &(node_ptr->data); }

            bool operator==(Iterator &other) const {
                return node_ptr == other.node_ptr;
            }

            bool operator!=(Iterator &other) const {
                return node_ptr != other.node_ptr;
            }

            Iterator& operator++() {
                if (node_ptr) {
                    node_ptr = node_ptr->next;
                }
                return *this;
            }

            Iterator& operator++(int) {
                Iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            Node* node_ptr;
        };

        /**
         * member types
         */
    public:
        using value_type = T;
        using allocator_type = Allocator;
        using allocator_type_internal = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = T&;
        using const_reference = const T&;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointner = typename std::allocator_traits<Allocator>::const_pointer;
        using iterator = Iterator;
        using const_iterator = const Iterator;

    public:

        /**
         * constructors
         */
        forward_list() : alloc() {
            head = alloc.allocate(1);
            new(head) Node();
        }

        forward_list(size_type count, const T& value) : forward_list() {
            Node* last_node = head;
            for (size_type i = 0; i < count; i++) {
                last_node -> next = alloc.allocate(1);
                new(last_node -> next) Node(value, nullptr);
                last_node = last_node -> next;
            }
        }

        explicit forward_list(size_type count) : forward_list(count, T{}) {}

        forward_list(Iterator first, Iterator last) : forward_list() {
            Node* last_node = head;
            for (auto it = first; it != last; ++it) {
                last_node -> next = alloc.allocate(1);
                new(last_node -> next) Node(*it, nullptr);
                last_node = last_node -> next;
            }
        }

        forward_list(const forward_list& other) : forward_list(other.cbegin(), other.cend()) {}

        forward_list(forward_list&& other) noexcept: alloc() {
            head = alloc.allocate(1);
            new(head) Node();
            Node *last_node = head;
            auto first = other.begin();
            auto last = other.end();
            for (auto it = first; it != last; ++it) {
                last_node -> next = alloc.allocate(1);
                new(last_node -> next) Node(std::move(*it), nullptr);
                last_node = last_node -> next;
            }
            other.clear();
        }

        forward_list(std::initializer_list<T> init) : forward_list() {
            Node *last_node = head;
            auto first = init.begin();
            auto last = init.end();
            for (auto it = first; it != last; ++it) {
                last_node -> next = alloc.allocate(1);
                new(last_node -> next) Node(*it, nullptr);
                last_node = last_node -> next;
            }
        }

        /**
         * destructor
         */
        ~forward_list() {
            clear();
            ((Node*) head) -> ~Node();
            alloc.deallocate(head, 1);
        }

        /**
         * assignment operators
         */
        forward_list& operator=(const forward_list& other) {
            if (this != &other) {
                forward_list tmp(other);
                (*this).swap(tmp);
                return *this;
            }
        }

        forward_list &operator=(forward_list&& other) noexcept {
            forward_list tmp(std::move(other));
            (*this).swap(tmp);
            return *this;
        }

        /**
         * other
         */
        void assign(size_type count, const T &value) {
            clear();
            *this = forward_list(count, value);
        }

        void assign(Iterator first, Iterator last) {
            clear();
            *this = forward_list(first, last);
        }

        void assign(std::initializer_list<T> init) {
            clear();
            *this = forward_list(init);
        }

        allocator_type get_allocator() const noexcept {
            return alloc;
        }

        /**
         * element access
         */
        reference front() {
            return *begin();
        }

        const_reference front() const {
            return *cbegin();
        }

        /**
         * iterators
         */
        iterator before_begin() noexcept {
            return iterator(head);
        }

        const_iterator before_begin() const noexcept {
            return cbefore_begin();
        }

        const_iterator cbefore_begin() const noexcept {
            return const_iterator(head);
        }

        iterator begin() noexcept {
            return iterator(head->next);
        }

        const_iterator begin() const noexcept {
            return cbegin();
        }

        const_iterator cbegin() const noexcept {
            return const_iterator(head->next);
        }

        iterator end() noexcept {
            return iterator(nullptr);
        }

        const_iterator end() const noexcept {
            return cend();
        }

        const_iterator cend() const noexcept {
            return const_iterator(nullptr);
        }

        /**
         * capacity
         */
        bool empty() const noexcept {
            return !(head -> next);
        }

        size_type max_size() const noexcept {
            return std::numeric_limits<difference_type>::max();
        }

        /**
         * modifiers
         */
        void clear() noexcept {
            erase_after(cbefore_begin(), cend());
            head->next = nullptr;
        }

        iterator insert_after(const_iterator pos, const T& value) {
            if (pos.node_ptr) {
                Node* prev_node = pos.node_ptr;
                Node* ins_node;
                ins_node = alloc.allocate(1);
                new(ins_node) Node(value, prev_node->next);
                prev_node -> next = ins_node;
                return iterator(ins_node);
            }
        }

        iterator insert_after(const_iterator pos, T&& value) {
            if (pos.node_ptr) {
                Node* prev_node = pos.node_ptr;
                Node* ins_node;
                ins_node = alloc.allocate(1);
                new(ins_node) Node(std::move(value), prev_node -> next);
                prev_node -> next = ins_node;
                return iterator(ins_node);
            }
        }

        iterator insert_after(const_iterator pos, size_type count, const T& value) {
            if (pos.node_ptr) {
                iterator insert_pos(pos.node_ptr);
                for (size_type i = 0; i < count; ++i) {
                    insert_pos = insert_after(insert_pos, value);
                }
                return insert_pos;
            }
        }

        template<class InputIt>
        iterator insert_after(const_iterator pos, InputIt first, InputIt last) {
            if (pos.node_ptr) {
                iterator insert_pos(pos.node_ptr);
                for (auto it = first; it != last; ++it) {
                    insert_pos = insert_after(insert_pos, *it);
                }
                return insert_pos;
            }
        }

        iterator insert_after(const_iterator pos, std::initializer_list<T> init) {
            return insert_after(pos, init.begin(), init.end());
        }

        iterator erase_after(const_iterator pos) {
            if (pos.node_ptr) {
                Node* del_node = pos.node_ptr -> next;
                if (del_node) {
                    pos.node_ptr -> next = del_node -> next;
                }
                else {
                    pos.node_ptr -> next = nullptr;
                }
                ((Node*) del_node) -> ~Node();
                alloc.deallocate(del_node, 1);
                return iterator(pos.node_ptr -> next);
            }
        }

        iterator erase_after(const_iterator first, const_iterator last) {
            if (first.node_ptr == last.node_ptr) {
                return iterator(last.node_ptr);
            }
            iterator it(first.node_ptr);
            do {
                it = erase_after(first);
            } while (it != const_cast<iterator&>(last));
            return it;
        }

        void push_front(const T& value) {
            insert_after(cbefore_begin(), value);
        }

        void push_front(T&& value) {
            insert_after(cbefore_begin(), std::move(value));
        }

        void pop_front() {
            erase_after(cbefore_begin());
        }

        void resize(size_type count) {
            size_type list_size = size();
            if (list_size > count) {
                iterator it = before_begin();
                for (size_type i = 0; i < count; ++i) {
                    ++it;
                }
                erase_after(it, cend());
                return;
            }
            if (list_size < count) {
                iterator it = before_begin();
                while (it.node_ptr -> next != cend().node_ptr) {
                    ++it;
                }
                for (size_type i = 0; i < count - list_size; ++i) {
                    it = insert_after(it, T{});
                }
                return;
            }
        }

        void resize(size_type count, const value_type &value) {
            size_type list_size = size();
            if (list_size > count) {
                iterator it = before_begin();
                for (size_type i = 0; i < count; ++i) {
                    ++it;
                }
                erase_after(it, cend());
                return;
            }
            if (list_size < count) {
                iterator it = before_begin();
                while (it.node_ptr -> next != cend().node_ptr) {
                    ++it;
                }
                for (size_type i = 0; i < count - list_size; ++i) {
                    it = insert_after(it, value);
                }
                return;
            }
        }

        void swap(forward_list &other) {
            std::swap(head, other.head);
        }

        /**
         * operations
         */
        void splice_after(const_iterator pos, forward_list &other) {
            if (pos.node_ptr) {
                insert_after(pos, other.cbegin(), other.cend());
                other.clear();
            }
        }

        void remove(const T &value) {
            Iterator it = before_begin();
            while (it.node_ptr -> next != nullptr) {
                if (it.node_ptr -> next -> data == value) {
                    erase_after(it);
                    continue;
                }
                ++it;
            }
        }

        void reverse() noexcept {
            Node* current_node = head->next;
            Node* prev_node = nullptr;
            Node* next_node = nullptr;
            while (current_node) {
                next_node = current_node -> next;
                current_node -> next = prev_node;
                prev_node = current_node;
                current_node = next_node;
            }
            head -> next = prev_node;
        }

        forward_list split_when(SplitPredicate condition) {
            forward_list other;
            iterator my_it = before_begin();
            iterator other_it = other.before_begin();
            while (my_it.node_ptr -> next != nullptr) {
                if (condition( my_it.node_ptr -> next -> data)) {
                    iterator tmp_it(my_it.node_ptr);
                    other.insert_after(other_it, ++tmp_it, cend());
                    erase_after(my_it, cend());
                    break;
                }
                ++my_it;
            }
            return other;
        }

        size_type size() {
            size_type size = 0;
            auto first = begin();
            auto last = end();
            for (auto it = first; it != last; ++it) {
                ++size;
            }
            return size;
        }

    private:
        allocator_type_internal alloc;
        Node* head;
    };
}


#endif //FORWARD_LIST_HPP


