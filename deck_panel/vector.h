#ifndef VECTOR_H
#define VECTOR_H

#include <Arduino.h> //needed for Serial.println
#include <string.h> //needed for memcpy

template <typename T> class Vector {
  public:
    Vector(int initialCapacity = 10);

    ~Vector();

    int capacity() const { return capacity_; }

    T *operator[](int index);
    const T *operator[](int index) const;

    void push_back(const T &value);
    void pop_back();

    int length() const { return size; }

	void clear();

  private:
    T *data;
    int capacity_;
    int size;

    void resize(int newCapacity);
};

template <typename T> Vector<T>::Vector(int initialCapacity) {
    this->capacity_ = initialCapacity;
    this->size = 0;
    this->data = new T[this->capacity_];
}

template <typename T> Vector<T>::~Vector() { delete[] this->data; }

template <typename T> T *Vector<T>::operator[](int index) {
    if (index < 0 || index >= this->size) {
        return nullptr;
    }

    return &this->data[index];
}

template <typename T> const T *Vector<T>::operator[](int index) const {
    if (index < 0 || index >= this->size) {
        return nullptr;
    }

    return &this->data[index];
}

template <typename T> void Vector<T>::push_back(const T &value) {
    if (this->size == this->capacity_) {
        resize(this->capacity_ * 2);
    }

    this->data[this->size++] = value;
}

template <typename T> void Vector<T>::pop_back() {
    if (this->size > 0) {
        this->size--;
    }
}

template <typename T> void Vector<T>::resize(int newCapacity) {
    T* newData = new T[newCapacity];
    for (int i =0; i < this->size; ++i) {
        newData[i] = this->data[i];
    }

    delete[] this->data;
    this->data = newData;
    this->capacity_ = newCapacity;
}

template <typename T> void Vector<T>::clear() {
	T* newData = new T[0];
	delete[] this->data;

	this->size = 0;
	this->capacity_ = 0;
	this->data = newData;
}

#endif // VECTOR_H
