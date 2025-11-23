#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>
#include <numeric>
#include <chrono>

// Helper function to measure execution time
template<typename Func>
void measureTime(const std::string& label, Func func) {   
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << label << ": " << duration.count() << " ms" << std::endl;
}

int main() {
    // Create a large vector
    const size_t size = 10'000'000;
    std::vector<int> data(size);
    
    // Initialize with values
    std::iota(data.begin(), data.end(), 0);
    
    std::cout << "=== STL Parallel Execution Examples ===" << std::endl;
    std::cout << "Vector size: " << size << std::endl << std::endl;
    
    // ----- Example 1: std::for_each -----
    std::cout << "1. for_each (squaring values):" << std::endl;
    
    auto vec1 = data;
    measureTime("  Sequential", [&]() {
        std::for_each(std::execution::seq, vec1.begin(), vec1.end(),
            [](int& n) { n = n * n; });
    });
    
    auto vec2 = data;
    measureTime("  Parallel", [&]() {
        std::for_each(std::execution::par, vec2.begin(), vec2.end(),
            [](int& n) { n = n * n; });
    });
    
    auto vec3 = data;
    measureTime("  Parallel+Vectorized", [&]() {
        std::for_each(std::execution::par_unseq, vec3.begin(), vec3.end(),
            [](int& n) { n = n * n; });
    });
    
    // ----- Example 2: std::transform -----
    std::cout << "\n2. transform (multiply by 2):" << std::endl;
    
    std::vector<int> result1(size);
    measureTime("  Sequential", [&]() {
        std::transform(std::execution::seq, data.begin(), data.end(),
            result1.begin(), [](int n) { return n * 2; });
    });
    
    std::vector<int> result2(size);
    measureTime("  Parallel", [&]() {
        std::transform(std::execution::par, data.begin(), data.end(),
            result2.begin(), [](int n) { return n * 2; });
    });
    
    std::vector<int> result3(size);
    measureTime("  Parallel+Vectorized", [&]() {
        std::transform(std::execution::par_unseq, data.begin(), data.end(),
            result3.begin(), [](int n) { return n * 2; });
    });
    
    // ----- Example 3: std::sort -----
    std::cout << "\n3. sort:" << std::endl;
    
    auto vec3 = data;
    std::reverse(vec3.begin(), vec3.end()); // Make it unsorted
    measureTime("  Sequential", [&]() {
        std::sort(std::execution::seq, vec3.begin(), vec3.end());
    });
    
    auto vec4 = data;
    std::reverse(vec4.begin(), vec4.end());
    measureTime("  Parallel", [&]() {
        std::sort(std::execution::par, vec4.begin(), vec4.end());
    });
    
    auto vec5 = data;
    std::reverse(vec5.begin(), vec5.end());
    measureTime("  Parallel+Vectorized", [&]() {
        std::sort(std::execution::par_unseq, vec5.begin(), vec5.end());
    });
    
    // ----- Example 4: std::reduce (sum) -----
    std::cout << "\n4. reduce (sum of elements):" << std::endl;
    
    long long sum1 = 0;
    measureTime("  Sequential", [&]() {
        sum1 = std::reduce(std::execution::seq, data.begin(), data.end(), 0LL);
    });
    std::cout << "    Result: " << sum1 << std::endl;
    
    long long sum2 = 0;
    measureTime("  Parallel", [&]() {
        sum2 = std::reduce(std::execution::par, data.begin(), data.end(), 0LL);
    });
    std::cout << "    Result: " << sum2 << std::endl;
    
    long long sum3 = 0;
    measureTime("  Parallel+Vectorized", [&]() {
        sum3 = std::reduce(std::execution::par_unseq, data.begin(), data.end(), 0LL);
    });
    std::cout << "    Result: " << sum3 << std::endl;
    
    // ----- Example 5: std::count_if -----
    std::cout << "\n5. count_if (even numbers):" << std::endl;
    
    size_t count1 = 0;
    measureTime("  Sequential", [&]() {
        count1 = std::count_if(std::execution::seq, data.begin(), data.end(),
            [](int n) { return n % 2 == 0; });
    });
    std::cout << "    Count: " << count1 << std::endl;
    
    size_t count2 = 0;
    measureTime("  Parallel", [&]() {
        count2 = std::count_if(std::execution::par, data.begin(), data.end(),
            [](int n) { return n % 2 == 0; });
    });
    std::cout << "    Count: " << count2 << std::endl;
    
    size_t count3 = 0;
    measureTime("  Parallel+Vectorized", [&]() {
        count3 = std::count_if(std::execution::par_unseq, data.begin(), data.end(),
            [](int n) { return n % 2 == 0; });
    });
    std::cout << "    Count: " << count3 << std::endl;
    
    return 0;
}


//Use functor
/*
#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>
#include <numeric>
#include <chrono>

// Functor to measure execution time
class MeasureTime {
private:
    std::string label;
    
public:
    MeasureTime(const std::string& lbl) : label(lbl) {}
    
    template<typename Func>
    void operator()(Func func) const {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << label << ": " << duration.count() << " ms" << std::endl;
    }
};

int main() {
    // Create a large vector
    const size_t size = 10'000'000;
    std::vector<int> data(size);
    
    // Initialize with values
    std::iota(data.begin(), data.end(), 0);
    
    std::cout << "=== STL Parallel Execution Examples ===" << std::endl;
    std::cout << "Vector size: " << size << std::endl << std::endl;
    
    // ----- Example 1: std::for_each -----
    std::cout << "1. for_each (squaring values):" << std::endl;
    
    auto vec1 = data;
    MeasureTime("  Sequential")([&]() {
        std::for_each(std::execution::seq, vec1.begin(), vec1.end(),
            [](int& n) { n = n * n; });
    });
    
    auto vec2 = data;
    MeasureTime("  Parallel")([&]() {
        std::for_each(std::execution::par, vec2.begin(), vec2.end(),
            [](int& n) { n = n * n; });
    });
    
    auto vec3 = data;
    MeasureTime("  Parallel+Vectorized")([&]() {
        std::for_each(std::execution::par_unseq, vec3.begin(), vec3.end(),
            [](int& n) { n = n * n; });
    });
    
    // ----- Example 2: std::transform -----
    std::cout << "\n2. transform (multiply by 2):" << std::endl;
    
    std::vector<int> result1(size);
    MeasureTime("  Sequential")([&]() {
        std::transform(std::execution::seq, data.begin(), data.end(),
            result1.begin(), [](int n) { return n * 2; });
    });
    
    std::vector<int> result2(size);
    MeasureTime("  Parallel")([&]() {
        std::transform(std::execution::par, data.begin(), data.end(),
            result2.begin(), [](int n) { return n * 2; });
    });
    
    std::vector<int> result3(size);
    MeasureTime("  Parallel+Vectorized")([&]() {
        std::transform(std::execution::par_unseq, data.begin(), data.end(),
            result3.begin(), [](int n) { return n * 2; });
    });
    
    // ----- Example 3: std::sort -----
    std::cout << "\n3. sort:" << std::endl;
    
    auto vec3 = data;
    std::reverse(vec3.begin(), vec3.end()); // Make it unsorted
    MeasureTime("  Sequential")([&]() {
        std::sort(std::execution::seq, vec3.begin(), vec3.end());
    });
    
    auto vec4 = data;
    std::reverse(vec4.begin(), vec4.end());
    MeasureTime("  Parallel")([&]() {
        std::sort(std::execution::par, vec4.begin(), vec4.end());
    });
    
    auto vec5 = data;
    std::reverse(vec5.begin(), vec5.end());
    MeasureTime("  Parallel+Vectorized")([&]() {
        std::sort(std::execution::par_unseq, vec5.begin(), vec5.end());
    });
    
    // ----- Example 4: std::reduce (sum) -----
    std::cout << "\n4. reduce (sum of elements):" << std::endl;
    
    long long sum1 = 0;
    MeasureTime("  Sequential")([&]() {
        sum1 = std::reduce(std::execution::seq, data.begin(), data.end(), 0LL);
    });
    std::cout << "    Result: " << sum1 << std::endl;
    
    long long sum2 = 0;
    MeasureTime("  Parallel")([&]() {
        sum2 = std::reduce(std::execution::par, data.begin(), data.end(), 0LL);
    });
    std::cout << "    Result: " << sum2 << std::endl;
    
    long long sum3 = 0;
    MeasureTime("  Parallel+Vectorized")([&]() {
        sum3 = std::reduce(std::execution::par_unseq, data.begin(), data.end(), 0LL);
    });
    std::cout << "    Result: " << sum3 << std::endl;
    
    // ----- Example 5: std::count_if -----
    std::cout << "\n5. count_if (even numbers):" << std::endl;
    
    size_t count1 = 0;
    measureTime("  Sequential", [&]() {
        count1 = std::count_if(std::execution::seq, data.begin(), data.end(),
            [](int n) { return n % 2 == 0; });
    });
    std::cout << "    Count: " << count1 << std::endl;
    
    size_t count2 = 0;
    measureTime("  Parallel", [&]() {
        count2 = std::count_if(std::execution::par, data.begin(), data.end(),
            [](int n) { return n % 2 == 0; });
    });
    std::cout << "    Count: " << count2 << std::endl;
    
    size_t count3 = 0;
    measureTime("  Parallel+Vectorized", [&]() {
        count3 = std::count_if(std::execution::par_unseq, data.begin(), data.end(),
            [](int n) { return n % 2 == 0; });
    });
    std::cout << "    Count: " << count3 << std::endl;
    
    return 0;
}

*/

/*
//Without the use of lambda functions
#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>
#include <numeric>
#include <chrono>

// Functor to measure execution time
class MeasureTime {
private:
    std::string label;
    
public:
    MeasureTime(const std::string& lbl) : label(lbl) {}
    
    template<typename Func>
    void operator()(Func func) const {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << label << ": " << duration.count() << " ms" << std::endl;
    }
};

// Functor for squaring values
class SquareValue {
public:
    void operator()(int& n) const {
        n = n * n;
    }
};

// Functor for multiplying by 2
class MultiplyByTwo {
public:
    int operator()(int n) const {
        return n * 2;
    }
};

// Functor for checking if number is even
class IsEven {
public:
    bool operator()(int n) const {
        return n % 2 == 0;
    }
};

int main() {
    // Create a large vector
    const size_t size = 10'000'000;
    std::vector<int> data(size);
    
    // Initialize with values
    std::iota(data.begin(), data.end(), 0);
    
    std::cout << "=== STL Parallel Execution Examples ===" << std::endl;
    std::cout << "Vector size: " << size << std::endl << std::endl;
    
    // ----- Example 1: std::for_each -----
    std::cout << "1. for_each (squaring values):" << std::endl;
    
    SquareValue squareOp;
    
    auto vec1 = data;
    MeasureTime("  Sequential")([&]() {
        std::for_each(std::execution::seq, vec1.begin(), vec1.end(), squareOp);
    });
    
    auto vec2 = data;
    MeasureTime("  Parallel")([&]() {
        std::for_each(std::execution::par, vec2.begin(), vec2.end(), squareOp);
    });
    
    auto vec3 = data;
    MeasureTime("  Parallel+Vectorized")([&]() {
        std::for_each(std::execution::par_unseq, vec3.begin(), vec3.end(), squareOp);
    });
    
    // ----- Example 2: std::transform -----
    std::cout << "\n2. transform (multiply by 2):" << std::endl;
    
    MultiplyByTwo multiplyOp;
    
    std::vector<int> result1(size);
    MeasureTime("  Sequential")([&]() {
        std::transform(std::execution::seq, data.begin(), data.end(),
            result1.begin(), multiplyOp);
    });
    
    std::vector<int> result2(size);
    MeasureTime("  Parallel")([&]() {
        std::transform(std::execution::par, data.begin(), data.end(),
            result2.begin(), multiplyOp);
    });
    
    std::vector<int> result3(size);
    MeasureTime("  Parallel+Vectorized")([&]() {
        std::transform(std::execution::par_unseq, data.begin(), data.end(),
            result3.begin(), multiplyOp);
    });
    
    // ----- Example 3: std::sort -----
    std::cout << "\n3. sort:" << std::endl;
    
    auto vec3 = data;
    std::reverse(vec3.begin(), vec3.end()); // Make it unsorted
    MeasureTime("  Sequential")([&]() {
        std::sort(std::execution::seq, vec3.begin(), vec3.end());
    });
    
    auto vec4 = data;
    std::reverse(vec4.begin(), vec4.end());
    MeasureTime("  Parallel")([&]() {
        std::sort(std::execution::par, vec4.begin(), vec4.end());
    });
    
    auto vec5 = data;
    std::reverse(vec5.begin(), vec5.end());
    MeasureTime("  Parallel+Vectorized")([&]() {
        std::sort(std::execution::par_unseq, vec5.begin(), vec5.end());
    });
    
    // ----- Example 4: std::reduce (sum) -----
    std::cout << "\n4. reduce (sum of elements):" << std::endl;
    
    long long sum1 = 0;
    MeasureTime("  Sequential")([&]() {
        sum1 = std::reduce(std::execution::seq, data.begin(), data.end(), 0LL);
    });
    std::cout << "    Result: " << sum1 << std::endl;
    
    long long sum2 = 0;
    MeasureTime("  Parallel")([&]() {
        sum2 = std::reduce(std::execution::par, data.begin(), data.end(), 0LL);
    });
    std::cout << "    Result: " << sum2 << std::endl;
    
    long long sum3 = 0;
    MeasureTime("  Parallel+Vectorized")([&]() {
        sum3 = std::reduce(std::execution::par_unseq, data.begin(), data.end(), 0LL);
    });
    std::cout << "    Result: " << sum3 << std::endl;
    
    // ----- Example 5: std::count_if -----
    std::cout << "\n5. count_if (even numbers):" << std::endl;
    
    IsEven evenChecker;
    
    size_t count1 = 0;
    MeasureTime("  Sequential")([&]() {
        count1 = std::count_if(std::execution::seq, data.begin(), data.end(), evenChecker);
    });
    std::cout << "    Count: " << count1 << std::endl;
    
    size_t count2 = 0;
    MeasureTime("  Parallel")([&]() {
        count2 = std::count_if(std::execution::par, data.begin(), data.end(), evenChecker);
    });
    std::cout << "    Count: " << count2 << std::endl;
    
    size_t count3 = 0;
    MeasureTime("  Parallel+Vectorized")([&]() {
        count3 = std::count_if(std::execution::par_unseq, data.begin(), data.end(), evenChecker);
    });
    std::cout << "    Count: " << count3 << std::endl;
    
    return 0;
}
*/
