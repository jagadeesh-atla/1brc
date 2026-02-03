#include <algorithm>
#include <cstdio>
#include <format>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct Mesaurment {
    string city;
    int temperature;

    Mesaurment(const string& line) {
        auto it = line.find(';');
        city = line.substr(0, it);
        temperature = parse_int(line, it);
    }

    int parse_int(const string& line, size_t from) {
        int res = 0;
        int sign = line[from] == '-' ? -1 : 1;
        for (size_t i = from + 1; i < line.size(); i++) {
            if (line[i] != '.') {
                res = res * 10 + (line[i] - '0');
            }
        }
        return sign * res;
    }

    string to_string() const {
        ostringstream oss;
        oss << "City: " << city << ", Temperature: " << fixed << setprecision(1)
            << temperature / 10.0;
        return oss.str();
    }
};

struct Aggregate {
    int max;
    int min;
    int count;
    long sum;

    string to_string() const {
        double mini = min / 10.0;
        double mean = (double)sum / (10.0 * count);
        double maxi = max / 10.0;

        return format("{:7.1f} ; {:7.1f} ; {:7.1f}", mini, mean, maxi);
    }
};

struct Map {
    unordered_map<string, Aggregate> mp;

    void add(const string& line) {
        auto [city, temperature] = Mesaurment(line);
        if (mp.count(city)) {
            mp[city].max = max(mp[city].max, temperature);
            mp[city].min = min(mp[city].min, temperature);
            mp[city].sum += temperature;
            mp[city].count += 1;

        } else {
            mp[city].max = temperature;
            mp[city].min = temperature;
            mp[city].sum = temperature;
            mp[city].count = 1;
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

    char* filepath = argv[1];
    ifstream input(filepath);
    if (not input.is_open()) {
        cerr << "[ERR] Could not open file " << filepath << ".\n";
        return 1;
    }

    Map map;
    string line;
    line.reserve(128);
    while (getline(input, line)) {
        map.add(line);
    }

    map.print();
    input.close();
}
