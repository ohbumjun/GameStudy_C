#include <iostream>
#include <vector>
#include <thread>

thread_local std::vector<int> threadLocalData;

int main()
{
    const int numThreads = 5;
    std::vector<std::thread> threads;

    // Launch the threads
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&i]() {
            std::vector<int> data;

            // Simulate data loading
            // ...
            data.push_back(i);

            // Store the data in the thread-local variable
            threadLocalData = data;
        });
    }

    // Wait for all threads to complete
    for (std::thread& thread : threads) {
        thread.join();
    }

    // Merge the thread-local data into a single vector
    std::vector<int> mergedData;
    for (int i = 0; i < numThreads; ++i) {
        mergedData.insert(mergedData.end(), threadLocalData.begin(), threadLocalData.end());
    }

    // Process the merged data
    // ...

    // Print the merged data as an example
    for (const int& value : mergedData) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return 0;
}