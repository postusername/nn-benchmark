#include <iostream>

// Объявления тестовых функций
extern bool test_euclidean_distance();
extern bool test_brute_force();

int main() {
    int passed = 0;
    int total = 0;
    
    // Тестирование евклидовой метрики
    ++total;
    std::cout << "Testing Euclidean distance..." << std::endl;
    if (test_euclidean_distance()) {
        ++passed;
        std::cout << "Euclidean distance test PASSED" << std::endl;
    } else {
        std::cout << "Euclidean distance test FAILED" << std::endl;
    }
    
    // Тестирование наивного алгоритма
    ++total;
    std::cout << "Testing Brute Force algorithm..." << std::endl;
    if (test_brute_force()) {
        ++passed;
        std::cout << "Brute Force algorithm test PASSED" << std::endl;
    } else {
        std::cout << "Brute Force algorithm test FAILED" << std::endl;
    }
    
    // Итоговый результат
    std::cout << "\n=========================" << std::endl;
    std::cout << "Tests passed: " << passed << "/" << total << std::endl;
    std::cout << "=========================" << std::endl;
    
    return (passed == total) ? 0 : 1;
} 