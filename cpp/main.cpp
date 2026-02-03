#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <format>
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

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        cerr << "[ERR] Could not open file " << argv[1] << ".\n";
        return 1;
    }

    struct stat st;
    fstat(fd, &st);

    size_t sz = st.st_size;
    char* data = (char*)mmap(nullptr, sz, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        cerr << "[ERR] mmap failed: " << strerror(errno) << ".\n";
        close(fd);
        return 1;
    }
    close(fd);
    madvise(data, sz, MADV_SEQUENTIAL);

    char* cur = data;
    char* end = data + sz;

    Map map;
    while (cur < end) {
        char* nl = (char*)memchr(cur, '\n', end - cur);
        if (!nl) nl = end;
        map.add(cur);
        cur = nl + 1;
    }

    if (argc > 3) map.print();

    munmap(data, sz);
    return 0;
}
