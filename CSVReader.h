#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include "BPlusTree.h"

struct Row {
    int rowId;
    std::vector<std::string> columns;
};

class CSVReader {
private:
    std::vector<std::string> headers;
    std::vector<Row> rows;
    std::unordered_map<std::string, BPlusTree<int, int>> indexes;

public:
    bool loadCSV(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Could not open file: " << filename << std::endl;
            return false;
        }
        std::string line;
        int id = 0;

        if (!std::getline(file, line)) return false;

        std::stringstream ss(line);
        std::string header;
        while (std::getline(ss, header, ','))
            headers.push_back(header);

        while (std::getline(file, line)) {
            std::stringstream rowStream(line);
            std::string cell;
            Row row{.rowId = id++};

            while (std::getline(rowStream, cell, ','))
                row.columns.push_back(cell);

            rows.push_back(row);
        }
        return true;
    }

    bool buildIndex(const std::string& column) {
        auto it = std::find(headers.begin(), headers.end(), column);
        if (it == headers.end()) return false;

        int colIdx = std::distance(headers.begin(), it);
        BPlusTree<int, int> tree;

        for (const auto& row : rows) {
            try {
                int key = std::stoi(row.columns[colIdx]);
                tree.insert(key, row.rowId);
            } catch (...) {}
        }

        indexes[column] = std::move(tree);
        return true;
    }

    std::vector<Row> find(const std::string& column, int value) {
        std::vector<Row> result;
        if (indexes.find(column) != indexes.end()) {
            try {
                int id = indexes[column].search(value);
                result.push_back(rows[id]);
            } catch (...) {}
        }
        return result;
    }

    std::vector<Row> range(const std::string& column, int start, int end) {
        std::vector<Row> result;
        if (indexes.find(column) != indexes.end()) {
            auto ids = indexes[column].rangeSearch(start, end);
            for (int id : ids)
                result.push_back(rows[id]);
        }
        return result;
    }

    void printRow(const Row& row) {
        for (const auto& col : row.columns)
            std::cout << col << " ";
        std::cout << "\n";
    }
};
