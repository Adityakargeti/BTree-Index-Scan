#include <iostream>
#include "CSVReader.h"
using namespace std;



int main() {
    CSVReader reader;
    if (!reader.loadCSV("employees.csv")) {
        std::cerr << "CSV load failed.\n";
        return 1;
    }

    // Build index on "id"
    reader.buildIndex("id");

    std::cout << "\nSearch id = 1001:\n";
    auto found = reader.find("id", 1001);
    for (const auto& row : found)
        reader.printRow(row);

    // Build index on "salary"
    reader.buildIndex("salary");

    std::cout << "\nRange salary [50000–80000]:\n";
    auto range = reader.range("salary", 50000, 80000);
    for (const auto& row : range)
        reader.printRow(row);

    return 0;
}
