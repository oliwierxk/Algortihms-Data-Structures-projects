#include <iostream>
#include "myString.h"
#include "MyVector.h"

using Sign = MyString;
using Characters = MyVector<MyString>;

bool startsWithMinOrMax(const MyString& sign) {
    return (sign.substr(0, 3) == "MIN" || sign.substr(0, 3) == "MAX");
}
//function checking if given sign is an operator
bool checkIfOperator(const MyString& sign)
{
    return (sign == "+" || sign == "-" || sign == "*" || sign == "/" || sign == "IF" || sign == "N");
}

//fucntion checking priority of a given opeator
int checkPriority(const MyString& sign)
{
    if (sign == "+" || sign == "-")
    {
        return 1;
    }
    else if (sign == "*" || sign == "/")
    {
        return 2;
    }
    else if (sign == "N")
    {
        return 3;
    }
    else if (sign == "MIN" || sign == "MAX")
    {
        return 0;
    }
    else
    {
        return 4;
    }
}

//function checking priority of an operator
bool PriorityQueisHigher(const MyString& sign1, const MyString& sign2)
{
    return checkPriority(sign1) >= checkPriority(sign2);
}


bool PriorityQueisEqual(const MyString& sign1, const MyString& sign2)
{
    return checkPriority(sign1) == checkPriority(sign2);
}

// function to check if given sign is a number
bool checkIfNumber(const MyString& number) {
    return number.checkIfNumber();
}

Characters separateCharacters(const char* infixNotation)
{
    Characters infix;
    do {
        const char* FirstPos = infixNotation;
        while (*infixNotation && *infixNotation != ' ') {
            ++infixNotation;
        }
        size_t size = (infixNotation - FirstPos);
        if (size) {
            MyString sign(FirstPos, size);
            infix.push_back(sign);
        }

    } while (*infixNotation++ != '\0');
    return infix;
}

MyString ConvertSignToString(const Characters & characters)
{
    MyString ConvertedCharacters;

    for (size_t i = 0; i < characters.size(); ++i)
    {
        ConvertedCharacters += characters[i];
        if (i < characters.size() - 1)
        {
            ConvertedCharacters += " ";
        }
    }
    return ConvertedCharacters;
}


void printSeparatedCharacters(const Characters& characters)
{
    // Convert characters to a string
    MyString separateChara = ConvertSignToString(characters);

    // Print each character
    for (size_t i = 0; i < separateChara.size(); ++i)
    {
        std::cout << separateChara[i];
    }
}

int makeOperation(const MyString& operation, int number1, int number2)
{
    if (operation == "+")
    {
        return number1 + number2;
    }
    else if (operation == "-")
    {
        return number1 - number2;
    }
    else if (operation == "*")
    {
        return number1 * number2;
    }
    else
    {
        return number1 / number2;
    }
}

int makeOperationN (int number)
{
    return (0 - number);
}

int makeOperationIF(int number1, int number2, int number3)
{
    int n1 = number1;
    int n2 = number2;
    int n3 = number3;
    if (n1 > 0)
    {
        return n2;
    }
    else {
        return n3;
    }
}

Characters ConverterPostFix(const Characters& characters)
{
    Characters Stack;
    Characters Postfix;
    Characters hStack;
    Characters zbd;

    for (size_t i = 0; i < (characters.size())-1; ++i)
    {
        const MyString& sign = characters[i];
        if (checkIfNumber(sign))
        {
            Postfix.push_back(sign);
            while (!Stack.empty() && Stack.back() == "N")
            {
                Postfix.push_back(Stack.back());
                Stack.pop_back();
            }

        }
        else if (checkIfOperator(sign))
        {   // Sprawdzamy, czy stos nie jest pusty i czy postfix nie jest pusty
            if (!Stack.empty() && sign == "N" && Stack.back() == "N")
            {
                Stack.push_back(sign);
            }
            else {
                if (!Stack.empty() && checkIfOperator(Stack.back()) &&
                    PriorityQueisHigher(Stack.back(), sign) && !Postfix.empty()) {
                    Postfix.push_back(Stack.back());
                    Stack.pop_back();
                    if (!Stack.empty() && checkIfOperator(Stack.back()) &&
                        PriorityQueisEqual(Stack.back(), sign) && !Postfix.empty()) {
                        Postfix.push_back(Stack.back());
                        Stack.pop_back();
                    }
                }

                Stack.push_back(sign);
            }
        }

        else if (sign == "(")
        {
                Stack.push_back(sign);
        }
        else if (sign == ")")
        {
            while (!Stack.empty() && Stack.back() != MyString("(")) {
                Postfix.push_back(Stack.back());
                Stack.pop_back();
            }
            if (!Stack.empty()) {
                Stack.pop_back();
            }
            if (!Stack.empty() && startsWithMinOrMax(Stack.back()))
            {
                Postfix.push_back(Stack.back() + hStack.back());

                Stack.pop_back();
                hStack.pop_back();
            }
            if (!Stack.empty() && Stack.back() == "IF")
            {
                Postfix.push_back(Stack.back());
                Stack.pop_back();
            }

            while (!Stack.empty() && Stack.back() == "N")
            {
                Postfix.push_back(Stack.back());
                Stack.pop_back();
            }
        }
        else if (sign == "MAX" || sign == "MIN")
        {
            Stack.push_back(sign);
            // Konwersja liczby całkowitej na MyString
            int l = 1;
            hStack.push_back(MyString::fromInt(l));
        }
        else if (sign == ",")
        {
            if (!Stack.empty())
            {
                while (!Stack.empty() && checkIfOperator(Stack.back())) {
                    Postfix.push_back(Stack.back());
                    Stack.pop_back();
                }
                if (!Stack.empty())
                {
                    zbd.push_back(Stack.back());
                    Stack.pop_back();
                    if (!Stack.empty() && startsWithMinOrMax(Stack.back()))
                    {
                        // Pobieramy liczbę argumentów ze stosu pomocniczego
                        int liczbaArg = MyString::toInt(hStack.back().c_str());
                        hStack.pop_back();
                        liczbaArg++;
                        hStack.push_back(MyString::fromInt(liczbaArg));
                    }
                    // Przenosimy argument na stos Postfix
                    Stack.push_back(zbd.back());
                    zbd.pop_back();
                }
            }
        }


    }

    while (!Stack.empty())
    {
        Postfix.push_back(Stack.back());
        Stack.pop_back();
    }
    return Postfix;
}


bool startsWithMax(const MyString& sign) {
    // Sprawdzamy, czy długość obiektu MyString jest co najmniej 3
    if (sign.size() >= 3) {
        // Tworzymy nowy obiekt MyString, który zawiera pierwsze 3 znaki
        MyString firstThree = sign.substr(0, 3);
        // Porównujemy pierwsze 3 znaki z "MAX"
        return (firstThree == "MAX");
    }
    // Jeśli obiekt MyString jest za krótki, nie zaczyna się od "MAX"
    return false;
}

Characters EvaluatingPostfix (Characters& characters)
{
    Characters Calculation;
    Characters Stack;


    for (size_t i = 0; i < characters.size(); ++i) {
        const MyString& sign = characters[i];
        // Sprawdzamy, czy dany znak jest liczbą
        if (sign.checkIfNumber()) {
            Stack.push_back(sign);
        }
        else if (checkIfOperator(sign)) {
            if (sign == "N") {
                // Dodajemy znak "N" do wynikowego ciągu obliczeń
                Calculation.push_back(sign);
                // Dodajemy ostatni element ze stosu do wynikowego ciągu obliczeń
                Calculation.push_back(Stack.back());
                // Konwertujemy ostatni element ze stosu na liczbę całkowitą
                int number = MyString::toInt(Stack.back().c_str());
                int resultN = makeOperationN(number);
                // Usuwamy ostatni element ze stosu
                Stack.pop_back();
                // Przenosimy pozostałe elementy ze stosu do wynikowego ciągu obliczeń
                int licznik = 0;
                while (!Stack.empty()) {
                    Calculation.push_back(Stack.back());
                    Stack.pop_back();
                    licznik++;
                }
                // Wyświetlamy wynik
                printSeparatedCharacters(Calculation);
                std::cout << std::endl;
                // Przenosimy elementy z wynikowego ciągu obliczeń z powrotem na stos
                for (int i = 0; i < licznik; ++i) {
                    Stack.push_back(Calculation.back());
                    Calculation.pop_back();
                }
                // Dodajemy wynik operacji "N" na stos
                Stack.push_back(MyString(resultN));
                // Usuwamy ostatni dodany znak "N" i wynik operacji z wynikowego ciągu obliczeń
                Calculation.pop_back();
                Calculation.pop_back();
            }
            else if (sign == "/") {
                // Dodajemy znak "/" do wynikowego ciągu obliczeń
                Calculation.push_back(sign);

                // Pobieramy drugą liczbę ze stosu i dodajemy ją do wynikowego ciągu obliczeń
                int number2 = MyString::toInt(Stack.back().c_str());
                Calculation.push_back(Stack.back());
                Stack.pop_back();

                // Pobieramy pierwszą liczbę ze stosu i dodajemy ją do wynikowego ciągu obliczeń
                int number1 = MyString::toInt(Stack.back().c_str());
                Calculation.push_back(Stack.back());
                Stack.pop_back();

                // Sprawdzamy, czy druga liczba nie jest zerem
                if (number2 == 0) {
                    // Jeśli druga liczba jest zerem, obsługujemy błąd
                    while (!Stack.empty()) {
                        Calculation.push_back(Stack.back());
                        Stack.pop_back();
                    }
                    // Wyświetlamy wynik
                    printSeparatedCharacters(Calculation);
                    std::cout << std::endl;
                    std::cout << "ERROR" << std::endl;
                    // Czyścimy wynikowe obliczenia
                    while (!Calculation.empty()) {
                        Calculation.pop_back();
                    }
                    break;
                }
                else {
                    // Wykonujemy operację z użyciem znaku
                    int result = makeOperation(sign, number1, number2);

                    // Przenosimy pozostałe elementy ze stosu do wynikowego ciągu obliczeń
                    int licznik = 0;
                    while (!Stack.empty()) {
                        Calculation.push_back(Stack.back());
                        Stack.pop_back();
                        licznik++;
                    }

                    // Wyświetlamy wynik
                    printSeparatedCharacters(Calculation);
                    std::cout << std::endl;

                    // Przenosimy elementy z wynikowego ciągu obliczeń z powrotem na stos
                    for (int i = 0; i < licznik; ++i) {
                        Stack.push_back(Calculation.back());
                        Calculation.pop_back();
                    }

                    // Dodajemy wynik operacji na stos
                    Stack.push_back(MyString(result));

                    // Czyścimy wynikowe obliczenia
                    while (!Calculation.empty()) {
                        Calculation.pop_back();
                    }
                }

            }
            else if (sign == "IF") {
                // Dodajemy znak "IF" do wynikowego ciągu obliczeń
                Calculation.push_back(sign);

                // Pobieramy trzecią liczbę ze stosu i dodajemy ją do wynikowego ciągu obliczeń
                int number3 = MyString::toInt(Stack.back().c_str());
                Calculation.push_back(Stack.back());
                Stack.pop_back();

                // Pobieramy drugą liczbę ze stosu i dodajemy ją do wynikowego ciągu obliczeń
                int number2 = MyString::toInt(Stack.back().c_str());
                Calculation.push_back(Stack.back());
                Stack.pop_back();

                // Pobieramy pierwszą liczbę ze stosu i dodajemy ją do wynikowego ciągu obliczeń
                int number1 = MyString::toInt(Stack.back().c_str());
                Calculation.push_back(Stack.back());
                Stack.pop_back();

                // Wykonujemy operację IF
                int resultIF = makeOperationIF(number1, number2, number3);

                // Przenosimy pozostałe elementy ze stosu do wynikowego ciągu obliczeń
                int licznik = 0;
                while (!Stack.empty()) {
                    Calculation.push_back(Stack.back());
                    Stack.pop_back();
                    licznik++;
                }

                // Wyświetlamy wynik
                printSeparatedCharacters(Calculation);
                std::cout << std::endl;

                // Przenosimy elementy z wynikowego ciągu obliczeń z powrotem na stos
                for (int i = 0; i < licznik; ++i) {
                    Stack.push_back(Calculation.back());
                    Calculation.pop_back();
                }

                // Dodajemy wynik operacji IF na stos
                Stack.push_back(MyString(resultIF));

                // Czyścimy wynikowe obliczenia
                while (!Calculation.empty()) {
                    Calculation.pop_back();
                }
            }
            else {
                // Dodajemy znak operatora do wynikowego ciągu obliczeń
                Calculation.push_back(sign);

                // Pobieramy drugą liczbę ze stosu i dodajemy ją do wynikowego ciągu obliczeń
                int number2 = MyString::toInt(Stack.back().c_str());
                Calculation.push_back(Stack.back());
                Stack.pop_back();

                // Pobieramy pierwszą liczbę ze stosu i dodajemy ją do wynikowego ciągu obliczeń
                int number1 = MyString::toInt(Stack.back().c_str());
                Calculation.push_back(Stack.back());
                Stack.pop_back();

                // Wykonujemy operację z użyciem znaku
                int result = makeOperation(sign, number1, number2);

                // Przenosimy pozostałe elementy ze stosu do wynikowego ciągu obliczeń
                int licznik = 0;
                while (!Stack.empty()) {
                    Calculation.push_back(Stack.back());
                    Stack.pop_back();
                    licznik++;
                }

                // Wyświetlamy wynik
                printSeparatedCharacters(Calculation);
                std::cout << std::endl;

                // Przenosimy elementy z wynikowego ciągu obliczeń z powrotem na stos
                for (int i = 0; i < licznik; ++i) {
                    Stack.push_back(Calculation.back());
                    Calculation.pop_back();
                }

                // Dodajemy wynik operacji na stos
                Stack.push_back(MyString(result));

                // Usuwamy ostatni dodany znak operatora oraz operandy z wynikowego ciągu obliczeń
                Calculation.pop_back();
                Calculation.pop_back();
                Calculation.pop_back();
            }

        }
        else if (startsWithMinOrMax(sign)) {
            // Dodajemy znak "MIN" lub "MAX" do wynikowego ciągu obliczeń
            Calculation.push_back(sign);

            // Pobieramy liczbę po "MIN" lub "MAX" i konwertujemy ją na int
            MyString afterMinOrMax = sign.substr(3, sign.size() - 3);
            int ile = MyString::toInt(afterMinOrMax.c_str());

            // Pobieramy pierwszy element ze stosu i konwertujemy go na int, który będzie początkowym wynikiem
            int result = MyString::toInt(Stack.back().c_str());
            Calculation.push_back(Stack.back());
            Stack.pop_back();

            // Jeśli stos nie jest pusty, kontynuujemy działanie
            if (!Stack.empty()) {
                for (int i = 1; i < ile; i++) {
                    // Dodajemy kolejny element ze stosu do wynikowego ciągu obliczeń
                    Calculation.push_back(Stack.back());

                    // Sprawdzamy czy znak jest "MAX" lub "MIN" i aktualizujemy wynik
                    if (startsWithMax(sign)) {
                        if (MyString::toInt(Stack.back().c_str()) > result) {
                            result = MyString::toInt(Stack.back().c_str());
                        }
                        Stack.pop_back();
                    } else {
                        if (MyString::toInt(Stack.back().c_str()) < result) {
                            result = MyString::toInt(Stack.back().c_str());
                        }
                        Stack.pop_back();
                    }
                }
            }

            // Przenosimy pozostałe elementy ze stosu do wynikowego ciągu obliczeń
            int licznik = 0;
            while (!Stack.empty()) {
                Calculation.push_back(Stack.back());
                Stack.pop_back();
                licznik++;
            }

            // Wyświetlamy wynik
            printSeparatedCharacters(Calculation);
            std::cout << std::endl;

            // Przenosimy elementy z wynikowego ciągu obliczeń z powrotem na stos
            for (int i = 0; i < licznik; ++i) {
                Stack.push_back(Calculation.back());
                Calculation.pop_back();
            }

            // Dodajemy wynik operacji na stos
            Stack.push_back(MyString(result));

            // Czyścimy wynikowe obliczenia
            while (!Calculation.empty()) {
                Calculation.pop_back();
            }
        }

    }
    while (!Stack.empty())
    {
        Calculation.push_back(Stack.back());
        Stack.pop_back();
    }
    return Calculation;

}


std::istream& getline(std::istream& istri, MyString& str) {
    char ch;
    int striSize = 250;
    int size = 0;
    char* stri = new char[striSize];
    while (istri.get(ch) && ch != '\n') {
        if (size >= striSize - 1) {
         
               char* newBuffer = new char[2 * striSize]; 
            std::memcpy(newBuffer, stri, size);  
            delete[] stri;
            stri = newBuffer;
            striSize *= 2;
        }
        stri[size++] = ch;  
    }
    stri[size] = '\0'; 
    str.data = stri; 
    str.length = size;
    return istri;
}


int main()
{
    //const char* infix = "4 + 1 - 3 * 8 .";
    //const char* infix1 = "( ( 10 * 281 ) + 839 - 92 + 109 ) / N IF ( 191 , 493 , 852 ) + IF ( 25 , 244 , 601 ) * IF ( ( 678 + 328 ) / ( 544 / 908 ) )";

    int rows;
    std ::cin>>rows;
    std ::cin.ignore();
    std ::cout<<std ::endl;
    for (int i = 0; i < rows; i++) {
        MyString infix;
        getline(std::cin,infix);
        Characters separated = separateCharacters(infix.c_str());
        //printSeparatedCharacters(separated);
        Characters PostFix = ConverterPostFix(separated);
        printSeparatedCharacters(PostFix);
        std::cout << std::endl;
        Characters Calc = EvaluatingPostfix(PostFix);
        printSeparatedCharacters(Calc);
        std::cout << std::endl;
        //infix = MyString();
        //infix.clear();
    }


    return 0;
}
