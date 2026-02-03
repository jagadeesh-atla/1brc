#include <algorithm>
#include <format>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using namespace std;

struct Aggregate {
    int max;
    int min;
    int count;
    long long sum;

    string to_string() const {
        double mini = min / 10.0;
        double mean = (double)sum / (10.0 * count);
        double maxi = max / 10.0;

        return format("{:7.1f} ; {:7.1f} ; {:7.1f}", mini, mean, maxi);
    }
};

struct CustomHash {
    using is_transparent = void;

    size_t operator()(std::string_view sv) const noexcept {
        return std::hash<std::string_view>{}(sv);
    }

    size_t operator()(const std::string& s) const noexcept {
        return std::hash<std::string_view>{}(s);
    }
};

struct Map {
    unordered_map<string, Aggregate, CustomHash, equal_to<>> mp;

    void add(char* line) {
        char* semicolon = line;
        while (*semicolon != ';') {
            semicolon++;
        }

        string_view city(line, semicolon - line);

        int temperature = 0;
        int sign = 1;
        char* p = semicolon + 1;

        if (*p == '-') {
            sign = -1;
            p++;
        }

        // Assumes format X.X or XX.X
        if (p[1] == '.') {
            temperature = (p[0] - '0') * 10 + (p[2] - '0');
        } else {
            temperature = (p[0] - '0') * 100 + (p[1] - '0') * 10 + (p[3] - '0');
        }
        temperature *= sign;

        auto it = mp.find(city);
        if (it != mp.end()) {
            it->second.max = max(it->second.max, temperature);
            it->second.min = min(it->second.min, temperature);
            it->second.sum += temperature;
            it->second.count += 1;
        } else {
            mp.emplace(string(city), Aggregate{temperature, temperature, 1, (long)temperature});
        }
    }

    void print() const {
        vector<pair<string, Aggregate>> data;
        data.reserve(mp.size());

        for (const auto& it : mp) data.push_back(it);

        sort(data.begin(), data.end(),
             [](const auto& a, const auto& b) { return a.first < b.first; });

        size_t maxWidth = 0;
        for (const auto& [city, _] : data) maxWidth = max(maxWidth, city.size());

        for (const auto& [city, agg] : data) {
            cout << setw((int)maxWidth) << left << city << ": " << agg.to_string() << '\n';
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <measurements-path>\n";
        return 1;
    }

    ifstream input(argv[1]);
    if (!input.is_open()) {
        cerr << "[ERR] Could not open file " << argv[1] << ".\n";
        return 1;
    }

    Map map;
    char buffer[1024];
    while (input.getline(buffer, sizeof(buffer))) {
        map.add(buffer);
    }

    if (argc > 3) map.print();
    input.close();
}
