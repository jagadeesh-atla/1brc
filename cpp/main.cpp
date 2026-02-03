#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

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

    string to_string() {
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
    int64_t sum;

    string to_string() {
        ostringstream oss;
        double maxi = double(max) / 10.0;
        double mini = double(min) / 10.0;
        double mean = double(sum) / (10.0 * double(count));

        oss << "<MIN|MEAN|MAX>: " << setw(4) << fixed << setprecision(1) << mini << "| " << mean
            << "|" << maxi;
        return oss.str();
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

    void print() {
        for (auto [city, agg] : mp) {
            cout << "City: " << city << "; " << agg.to_string() << endl;
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
