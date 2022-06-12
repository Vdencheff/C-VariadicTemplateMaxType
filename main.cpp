#include<iostream>
using namespace std;
// conditiona must be called always with all template parameters supplied:
// conditiona<false, int, double>
// the best match of all overloaded versions will be called.
// we have defined version with no specified arguments and with one supplied argument:
// conditiona and conditiona<false, T, F>
// so the best mach is the second version.

// define conditional
template<bool B, class T, class F>
struct conditiona { typedef T type; };

template<class T, class F>
struct conditiona<false, T, F> { typedef F type; };

template<typename T, typename U>
class Component1
{
public:
    Component1()
    {
        cout << "Component1 constructor!" << endl;
        Type1 = 2;
        Type2 = 2;
        Type3 = 2;
        m_int = 6;
        cout << "Type1= " << Type1 << endl;
        cout << "m_int= " << m_int << endl;
    }

private:
    typename conditiona<true, T, U>::type  Type1; // calls definition with 3 types, Type1 is a variable of type the conditional returns
    typename conditiona<false, T, U>::type  Type2; // calls overloaded definition with first type false
    typename conditiona<sizeof(T) >= sizeof(U), T, U>::type Type3;
    // create a structure of types
    struct largest_type
    {
        using myint = int;
        typedef int a;
        a b = 4;
    };
    typename largest_type::myint m_int; // create a variable
public:
    largest_type lt;
};

// variadic template class
template<typename... DerivedTypes>
class Component2
{
public:
    template<typename... T>
    struct largestType;


    // we are dealing with types not with objects, so the template params must be gived with <> in the definition
    // structure taking one template parameter
    template<typename T>
    struct largestType<T>
    {
        using typeSize = T;
    };
    // explicit specialization requires template<>. This specialization is not needed, it is just for checking syntaxis.
    template<> struct largestType<int>
    {
        using typeSize = int;
    };
    // short version of finding max type

    template <typename T, typename U, typename... Us>
    using typeSize = typename largestType<typename conditiona<sizeof(T) >= sizeof(U), T, U>::type, Us...>::typeSize;
    void printMaxVarShort()
    {
        cout << "myMaxVarShort = " << typeid(myMaxVar).name() << endl;
        valOfmaxSize = sizeof(typeSize<DerivedTypes...>);
        cout << "valOfmaxSizeShort = " << valOfmaxSize << endl;
    }
    // struct taking 2 or more template arguments
    template <typename T, typename U, typename... Us>
    struct largestType<T, U, Us...>
    {
        using typeSize = typename largestType<typename conditiona<sizeof(T) >= sizeof(U), T, U>::type, Us...>::typeSize;
    };
    // how the above works:
    // lets call largestType<T, U, Us...> with 3 param: largestType<int, char, byte>, the first two parameters are combined into one, due to conditional<> expression
    // typeSize = largestType<int, byte>, and this will call largestType<T, U, Us...> with largestType<int, byte>, so Us... is empty, so not supplied
    // typeSize = largestType<int>, and this will call the overloaded struct that takes single template parameter largestType<T> and returs T.
    void printMaxVar()
    {
        cout << "myMaxVar = " << typeid(myMaxVar).name() << endl;
        valOfmaxSize = sizeof(largestType<DerivedTypes...>::typeSize);
        cout << "valOfmaxSize = " << valOfmaxSize << endl;
    }
private:
    typename largestType<DerivedTypes...>::typeSize  myMaxVar; // create an object of the type with the maximum size from the supplied types
    int valOfmaxSize;
};

int main()
{
    Component1<int, double> c1;
    c1.lt.b = 2;
    typedef std::conditional<true, int, double>::type Type1;
    typedef std::conditional<false, int, double>::type Type2;
    typedef std::conditional<sizeof(int) >= sizeof(double), int, double>::type Type3;

    std::cout << typeid(Type1).name() << '\n';
    std::cout << typeid(Type2).name() << '\n';
    std::cout << typeid(Type3).name() << '\n';

    Component2<int, char, double> c2;
    c2.printMaxVar();
    c2.printMaxVarShort();
}