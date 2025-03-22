#include <iostream>
#include <iterator>
#include <vector>
#include <cassert>
#include "powersort.cpp"
#include "powersortmod.cpp"

using namespace std; 

typedef int* Iterator;


//dar print no vetor
template<typename T>
void print_vector(const std::vector<T>& vec) {
    for (const auto& val : vec) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}
//checar se está sorted
template<typename T>
bool is_sorted(const std::vector<T>& vec) {
    for (size_t i = 1; i < vec.size(); ++i) {
        if (vec[i - 1] > vec[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    
    std::vector<int> vec1 = {10, 20, 30, 40, 15, 25, 35, 5, 3, 1, 50, 60, 70, 55, 45, 80, 90, 100, 95, 85};
    std::vector<int> vec2 = {10, 20, 30, 40, 15, 25, 35, 5, 3, 1, 50, 60, 70, 55, 45, 80, 90, 100, 95, 85};

    std::cout << "Vetor: " << std::endl;
    print_vector(vec1);

    power_sort_paper(vec1.data(), vec1.data() + vec1.size());
    power_sort_paper2(vec2.data(), vec2.data() + vec2.size());

    std::cout << "Vetor ordenado: " << std::endl;
    print_vector(vec1);
    print_vector(vec2);
    

    //verificação
    assert(is_sorted(vec1));
    assert(is_sorted(vec2));
    std::cout << "Ordenou!" << std::endl;

    return 0;
}