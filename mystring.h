//
// Created by Oliwier Komorowski on 31/03/2024.
//

#ifndef ONP1_0_MYSTRING_H
#define ONP1_0_MYSTRING_H
#include <iostream>
#include <cstring>

class MyString {
private:
    char* data;
    size_t length;

    static bool isSpace(char c) {
        return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
    }

    static bool isDigit(char c) {
        return (c >= '0' && c <= '9');
    }

public:

    MyString() : data(nullptr), length(0) {}
    MyString(int number);
    MyString(const char* str) {
        length = std::strlen(str);
        data = new char[length + 1];
        std::strncpy(data, str, length);
        data[length] = '\0';
    }

    friend std::istream& getline(std::istream& is, MyString& str);


    void clear() {
        delete[] data;
        data = nullptr;
        length = 0;
    }

    MyString(const char* str, size_t len) {
        length = len;
        data = new char[length + 1];
        std::strncpy(data, str, length);
        data[length] = '\0';
    }

    static int toInt(const MyString& str) {
        int result = 0;
        int sign = 1;
        size_t i = 0;

        // Pomijamy białe znaki na początku ciągu
        while (i < str.size() && isSpace(str[i])) {
            ++i;
        }

        // Sprawdzamy znak + lub -
        if (i < str.size() && (str[i] == '+' || str[i] == '-')) {
            sign = (str[i++] == '-') ? -1 : 1;
        }

        // Konwertujemy cyfry na liczbę całkowitą
        while (i < str.size() && isDigit(str[i])) {
            result = result * 10 + (str[i++] - '0');
        }

        return sign * result;
    }



    MyString(const MyString& other) {
        length = other.length;
        data = new char[length + 1];
        std::strncpy(data, other.data, length);
        data[length] = '\0';
    }

    static MyString fromInt(int value) {
        // Obliczamy długość ciągu znaków potrzebnego do przechowania liczby całkowitej
        int len = snprintf(nullptr, 0, "%d", value);
        if (len <= 0) {
            return MyString(); // Zwracamy pusty obiekt w przypadku błędu
        }

        // Tworzymy bufor dla ciągu znaków
        char* buffer = new char[len + 1];
        // Konwertujemy liczbę całkowitą na ciąg znaków
        snprintf(buffer, len + 1, "%d", value);

        // Tworzymy obiekt MyString z bufora
        MyString result(buffer);
        delete[] buffer; // Zwolnij bufor

        return result;
    }

    ~MyString() {
     //   delete[] data;
    }

    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data;
            length = other.length;
            data = new char[length + 1];
            std::strncpy(data, other.data, length);
            data[length] = '\0';
        }
        return *this;
    }

    char& operator[](size_t index) {
        return data[index];
    }

    const char& operator[](size_t index) const {
        return data[index];
    }

    MyString substr(size_t pos, size_t len) const {
        len = (pos + len > length) ? (length - pos) : len;
        return MyString(data + pos, len);
    }

    size_t size() const {
        return length;
    }

    const char* c_str() const {
        return data;
    }

    bool checkIfNumber() const {
        for (size_t i = 0; i < length; ++i) {
            if (!isDigit(data[i])) {
                return false;
            }
        }
        return true;
    }

    MyString& operator+=(const MyString& other) {
        size_t newLength = length + other.length;
        char* newData = new char[newLength + 1];

        // Kopiujemy zawartość istniejących danych
        std::memcpy(newData, data, length);
        // Kopiujemy zawartość dodawanej wartości
        std::memcpy(newData + length, other.data, other.length);
        newData[newLength] = '\0';

        // Usuwamy stare dane i przypisujemy nowe
        delete[] data;
        data = newData;
        length = newLength;

        return *this;
    }
    friend bool operator!=(const MyString& lhs, const MyString& rhs) {
        return std::strcmp(lhs.data, rhs.data) != 0;
    }
    friend bool operator==(const MyString& lhs, const MyString& rhs) {
        return std::strcmp(lhs.data, rhs.data) == 0;
    }

    friend MyString operator+(const MyString& lhs, const MyString& rhs) {
        MyString result;
        result.length = lhs.length + rhs.length;
        result.data = new char[result.length + 1];

        std::strcpy(result.data, lhs.data);
        std::strcat(result.data, rhs.data);

        return result;
    }


};
/*
bool operator==(const MyString& lhs, const char* rhs) {
    return std::strcmp(lhs.c_str(), rhs) == 0;
}

bool operator==(const char* lhs, const MyString& rhs) {
    return std::strcmp(lhs, rhs.c_str()) == 0;
}
*/


MyString::MyString(int number) {
    length = snprintf(nullptr, 0, "%d", number);
    data = new char[length + 1];
    snprintf(data, length + 1, "%d", number);
}






#endif //ONP1_0_MYSTRING_H
