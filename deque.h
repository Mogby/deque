//https://github.com/Mogby/deque
#pragma once

#ifndef DEQUE_H
#define DEQUE_H

#include <iterator>

template <typename data_type>
class deque {

    template<typename value_type, typename container_type, int move_direction>
    class deque_iterator : public std::iterator<std::random_access_iterator_tag, value_type> {

    typedef deque_iterator<value_type, container_type, move_direction> iterator;
    typedef typename std::iterator_traits<iterator>::pointer pointer;
    
    private:

        container_type *container;
        size_t offset;

        void move(int move_offset) {
            offset += move_offset * move_direction;
        }

    public:

        iterator& operator++() {
            move(1);

            return *this;
        }

        iterator operator++(int) {
            iterator res = *this;

            move(1);

            return res;
        }

        iterator& operator--() {
            move(-1);

            return *this;
        }

        iterator operator--(int) {
            iterator res = *this;

            move(-1);

            return res;
        }

        iterator& operator+=(int delta_offset) {
            move(delta_offset);

            return *this;
        }

        iterator operator+(int delta_offset) const {
            iterator res = *this;
            res += delta_offset;

            return res;
        }

        iterator& operator-=(int delta_offset) {
            move(-delta_offset);

            return *this;
        }

        iterator operator-(int delta_offset) const {
            iterator res = *this;
            res -= delta_offset;

            return res;
        }

        int operator-(const iterator &other) const {
            return static_cast<int>(offset) - static_cast<int>(other.offset);
        }

        value_type& operator*() const {
            return (*container)[offset];
        }

        value_type& operator[](const int delta_offset) const {
            return *(*this + delta_offset);
        }

        value_type* operator->() const {
            return &((*container)[offset]);
        }

        iterator& operator=(const iterator &other) {
            container = other.container;
            offset = other.offset;

            return *this;
        }

        bool operator==(const iterator &other) const {
            return container == other.container &&
                offset == other.offset;
        }

        bool operator!=(const iterator &other) const {
            return !(*this == other);
        }

        bool operator<(const iterator &other) const {
            return offset < other.offset;
        }

        bool operator<=(const iterator &other) const {
            return offset <= other.offset;
        }

        bool operator>(const iterator &other) const {
            return offset > other.offset;
        }

        bool operator>=(const iterator &other) const {
            return offset >= other.offset;
        }



        deque_iterator(container_type *container, size_t offset) : 
            container(container), offset(offset) {}

        deque_iterator(const iterator &other) {
            *this = other;
        }

        deque_iterator() {}



        virtual ~deque_iterator() {}

    };

private:

    data_type* vector;
    
    size_t vector_size;

    size_t vector_capacity;

    size_t begin_offset;

    size_t end_offset;



    void resize_vector(size_t new_capacity);

    inline void adapt_vector(size_t new_size);



    inline size_t move_index(size_t index, int offset) const;

    inline size_t next_index(size_t index) const;

    inline size_t previous_index(size_t index) const;

public:

    typedef deque_iterator<data_type, deque<data_type>, 1> iterator;

    typedef deque_iterator<const data_type, const deque<data_type>, 1> const_iterator;

    typedef deque_iterator<data_type, deque<data_type>, -1> reverse_iterator;
    
    typedef deque_iterator<const data_type, const deque<data_type>, -1> const_reverse_iterator;



    void push_back(const data_type& new_element);

    void pop_back();

    void push_front(const data_type& new_element);

    void pop_front();



    data_type& back();

    const data_type& back() const;

    data_type& front();

    const data_type& front() const;



    bool empty() const;

    size_t size() const;



    iterator begin();

    const_iterator begin() const;

    const_iterator cbegin() const;

    iterator end();
    
    const_iterator end() const;

    const_iterator cend() const;

    reverse_iterator rbegin();

    const_reverse_iterator rbegin() const;

    const_reverse_iterator crbegin() const;

    reverse_iterator rend();

    const_reverse_iterator rend() const;

    const_reverse_iterator crend() const;



    data_type& operator[](const size_t index);

    const data_type& operator[](const size_t index) const;

    bool operator==(const deque<data_type> &other) const;



    deque();

    deque(const deque& other);

    virtual ~deque();

};

template <typename data_type>
void deque<data_type>::resize_vector(size_t new_capacity) {
    data_type* new_vector = new data_type[new_capacity];

    for (size_t offset = 0; offset < vector_size; ++offset) {
        new_vector[offset] = vector[(begin_offset + offset) % vector_capacity];
    }

    if (vector) {
        delete[] vector;
    }

    vector = new_vector;
    vector_capacity = new_capacity;

    begin_offset = 0;
    end_offset = vector_size;

    return;
}

template <typename data_type>
inline void deque<data_type>::adapt_vector(size_t new_size) {
    vector_size= new_size;

    if (vector_size* 4 < vector_capacity && vector_capacity / 4 >= 4) {
        resize_vector(vector_capacity / 4);
    } else if (vector_size* 2 > vector_capacity) {
        resize_vector(vector_capacity * 2);
    }
}

template <typename data_type, typename value_type, typename container_type, int move_direction>
typename deque<data_type>::template deque_iterator<value_type, container_type, 
    move_direction> operator+(int offset, const typename deque<data_type>::template deque_iterator<
    value_type, container_type, move_direction> &iterator) {
    return iterator + offset;
}

template <typename data_type>
inline size_t deque<data_type>::move_index(size_t index, int offset) const {
    offset %= vector_capacity;
    if (offset < 0) {
        offset += vector_capacity;
    }

    return (index + offset) % vector_capacity;
}

template <typename data_type>
inline size_t deque<data_type>::next_index(size_t index) const {
    return move_index(index, 1);
}

template <typename data_type>
inline size_t deque<data_type>::previous_index(size_t index) const {
    return move_index(index, -1);
}



template <typename data_type>
void deque<data_type>::push_back(const data_type& new_element) {
    vector[end_offset] = new_element;
    end_offset = next_index(end_offset);

    adapt_vector(vector_size+ 1);
}

template <typename data_type>
void deque<data_type>::pop_back() {
    end_offset = previous_index(end_offset);

    adapt_vector(vector_size- 1);
}

template <typename data_type>
void deque<data_type>::push_front(const data_type& new_element) {
    begin_offset = previous_index(begin_offset);
    vector[begin_offset] = new_element;

    adapt_vector(vector_size+ 1);
}

template <typename data_type>
void deque<data_type>::pop_front() {
    begin_offset = next_index(begin_offset);

    adapt_vector(vector_size- 1);
}



template <typename data_type>
data_type& deque<data_type>::back() {
    return vector[previous_index(end_offset)];
}

template <typename data_type>
const data_type& deque<data_type>::back() const {
    return vector[previous_index(end_offset)];
}

template <typename data_type>
data_type& deque<data_type>::front() {
    return vector[begin_offset];
}

template <typename data_type>
const data_type& deque<data_type>::front() const {
    return vector[begin_offset];
}



template <typename data_type>
bool deque<data_type>::empty() const {
    return !vector_size;
}

template <typename data_type>
size_t deque<data_type>::size() const {
    return vector_size;
}



template <typename data_type>
typename deque<data_type>::iterator deque<data_type>::begin() {
    return typename deque<data_type>::iterator(this, 0);
}

template <typename data_type>
typename deque<data_type>::const_iterator deque<data_type>::begin() const {
    return typename deque<data_type>::const_iterator(this, 0);
}

template <typename data_type>
typename deque<data_type>::const_iterator deque<data_type>::cbegin() const {
    return typename deque<data_type>::const_iterator(this, 0);
}

template <typename data_type>
typename deque<data_type>::iterator deque<data_type>::end() {
    return typename deque<data_type>::iterator(this, vector_size);
}

template <typename data_type>
typename deque<data_type>::const_iterator deque<data_type>::end() const {
    return typename deque<data_type>::const_iterator(this, vector_size);
}

template <typename data_type>
typename deque<data_type>::const_iterator deque<data_type>::cend() const {
    return typename deque<data_type>::const_iterator(this, vector_size);
}

template <typename data_type>
typename deque<data_type>::reverse_iterator deque<data_type>::rbegin() {
    return typename deque<data_type>::reverse_iterator(this, vector_size - 1);
}

template <typename data_type>
typename deque<data_type>::const_reverse_iterator deque<data_type>::rbegin() const {
    return typename deque<data_type>::const_reverse_iterator(this, vector_size - 1);
}

template <typename data_type>
typename deque<data_type>::const_reverse_iterator deque<data_type>::crbegin() const {
    return typename deque<data_type>::const_reverse_iterator(this, vector_size - 1);
}

template <typename data_type>
typename deque<data_type>::reverse_iterator deque<data_type>::rend() {
    return typename deque<data_type>::reverse_iterator(this, -1);
}

template <typename data_type>
typename deque<data_type>::const_reverse_iterator deque<data_type>::rend() const {
    return typename deque<data_type>::const_reverse_iterator(this, -1);
}

template <typename data_type>
typename deque<data_type>::const_reverse_iterator deque<data_type>::crend() const {
    return typename deque<data_type>::const_reverse_iterator(this, -1);
}


template <typename data_type>
data_type& deque<data_type>::operator[](const size_t index) {
    return vector[move_index(begin_offset, index)];
}

template <typename data_type>
const data_type& deque<data_type>::operator[](const size_t index) const {
    return vector[move_index(begin_offset, index)];
}

template <typename data_type>
bool deque<data_type>::operator==(const deque<data_type> &other) const {
    return vector == other.vector;
}



template <typename data_type>
deque<data_type>::deque() {
    vector = nullptr;
    vector_size= 0;
    begin_offset = end_offset = 0;

    resize_vector(4);
}

template <typename data_type>
deque<data_type>::deque(const deque& other) {
    vector = nullptr;

    if (vector_capacity != other.vector_capacity) {
        resize_vector(other.vector_capacity);
    }

    for (size_t offset = 0; offset < other.vector_size; ++offset) {
        vector[offset] = other.vector[(other.begin_offset + offset) % other.vector_capacity];
    }
    vector_size= other.vector_size;

    begin_offset = 0;
    end_offset = vector_size;
}

template <typename data_type>
deque<data_type>::~deque() {
    delete[] vector;
}

#endif
