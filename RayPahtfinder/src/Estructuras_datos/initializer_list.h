#ifndef MY_INITIALIZER_LIST_H
#define MY_INITIALIZER_LIST_H

template <typename T>
class my_initializer_list {
private:
    const T* m_data;
    int m_size;

public:
    constexpr my_initializer_list(const T* data, int size)
        : m_data(data), m_size(size) {}

    constexpr const T* begin() const { return m_data; }
    constexpr const T* end() const { return m_data + m_size; }
    constexpr int size() const { return m_size; }
};

/*
 * Nueva macro segura: 
 * - Crea un array estático con duración extendida.
 * - Evita el error "taking address of temporary array".
 * 
 * Uso:
 *   MyVector<int> v = MY_INIT_LIST(miVector, int, 1, 2, 3, 4);
 */
#define MY_INIT_LIST(name, type, ...)                            \
    static const type name##_array[] = {__VA_ARGS__};            \
    my_initializer_list<type>(name##_array,                      \
                              sizeof(name##_array) / sizeof(type))

#endif // MY_INITIALIZER_LIST_H
