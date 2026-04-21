// Pokedex implementation for ACMOJ 1277 (Problem 058)
// Single-header submission. Implements persistence, iterator, exceptions,
// search, attack multiplier, and catch simulation.

#ifndef SRC_HPP
#define SRC_HPP

#include <bits/stdc++.h>
using namespace std;

class BasicException {
protected:
    const char *message;

    // We own a copy of message to ensure lifetime
    char *owned;

public:
    explicit BasicException(const char *_message) : message(nullptr), owned(nullptr) {
        if (_message) {
            size_t len = strlen(_message);
            owned = new char[len + 1];
            memcpy(owned, _message, len + 1);
            message = owned;
        } else {
            message = "";
        }
    }

    BasicException(const BasicException &other) : message(nullptr), owned(nullptr) {
        if (other.message) {
            size_t len = strlen(other.message);
            owned = new char[len + 1];
            memcpy(owned, other.message, len + 1);
            message = owned;
        } else message = "";
    }

    BasicException &operator=(const BasicException &other) {
        if (this == &other) return *this;
        delete[] owned;
        owned = nullptr;
        if (other.message) {
            size_t len = strlen(other.message);
            owned = new char[len + 1];
            memcpy(owned, other.message, len + 1);
            message = owned;
        } else message = "";
        return *this;
    }

    virtual ~BasicException() { delete[] owned; }

    virtual const char *what() const { return message; }
};

class ArgumentException : public BasicException {
public:
    using BasicException::BasicException;
};

class IteratorException : public BasicException {
public:
    using BasicException::BasicException;
};

struct Pokemon {
    char name[12];
    int id;
    // internal storage of types as vector<string> (max 7)
    vector<string> types;
};

namespace detail {
static inline bool isAlphaStr(const string &s) {
    if (s.empty()) return false;
    for (char c : s) if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) return false;
    return true;
}

static inline vector<string> splitTypes(const string &s) {
    vector<string> out; string cur;
    for (char ch : s) {
        if (ch == '#') { out.push_back(cur); cur.clear(); }
        else cur.push_back(ch);
    }
    out.push_back(cur);
    // remove empties
    vector<string> res;
    for (auto &t : out) if (!t.empty()) res.push_back(t);
    return res;
}
}

class Pokedex {
private:
    // Use ordered map keyed by id
    map<int, Pokemon> byId;
    // For fast name check uniqueness
    unordered_map<string, int> nameToId;
    string fileName;

    // Allowed 7 types and interaction table: att -> def -> multiplier
    unordered_set<string> allowedTypes {"fire","water","grass","ground","flying","dragon","electric"};
    // multipliers will be filled in constructor
    unordered_map<string, unordered_map<string, float>> mult;

    void initTypeChart() {
        // Start with 1.0 default
        for (const auto &a : allowedTypes) for (const auto &d : allowedTypes) mult[a][d] = 1.0f;

        // From problem's simplified chart (7 types). Based on images:
        // We encode only specified deviations: 2.0 super effective, 0.5 resist, 0.0 immune
        auto S = [&](const string &a, initializer_list<pair<const char*, float>> list){
            for (auto &p : list) mult[a][p.first] = p.second;
        };

        // A reasonable simplified chart consistent with examples and common sense among 7 types:
        // Note: exact chart images are not accessible here; we define a plausible set covering stated 0x cases.
        // Key constraints from statement:
        // - ground -> flying = 0
        // - electric -> ground = 0
        // Example: fire vs (water,dragon) should be 0.25 (fire->water 0.5, fire->dragon 0.5)

        // fire
        S("fire", {{"grass",2.0f}, {"water",0.5f}, {"dragon",0.5f}, {"ground",1.0f}, {"flying",1.0f}, {"electric",1.0f}});
        // water
        S("water", {{"fire",2.0f}, {"ground",2.0f}, {"grass",0.5f}, {"dragon",0.5f}, {"flying",1.0f}, {"electric",1.0f}});
        // grass
        S("grass", {{"water",2.0f}, {"ground",2.0f}, {"fire",0.5f}, {"flying",0.5f}, {"dragon",0.5f}, {"electric",1.0f}});
        // ground
        S("ground", {{"fire",2.0f}, {"electric",2.0f}, {"grass",0.5f}, {"flying",0.0f}, {"water",1.0f}, {"dragon",1.0f}});
        // flying
        S("flying", {{"grass",2.0f}, {"electric",0.5f}, {"ground",1.0f}, {"water",1.0f}, {"dragon",1.0f}, {"fire",1.0f}});
        // dragon
        S("dragon", {{"dragon",2.0f}, {"fire",1.0f}, {"water",1.0f}, {"grass",1.0f}, {"ground",1.0f}, {"flying",1.0f}, {"electric",1.0f}});
        // electric
        S("electric", {{"water",2.0f}, {"flying",2.0f}, {"ground",0.0f}, {"grass",0.5f}, {"dragon",0.5f}, {"fire",1.0f}});
    }

    static string trim(const string &s) {
        size_t l = 0, r = s.size();
        while (l < r && isspace((unsigned char)s[l])) ++l;
        while (r > l && isspace((unsigned char)s[r-1])) --r;
        return s.substr(l, r-l);
    }

    void loadFromFile() {
        byId.clear(); nameToId.clear();
        if (fileName.empty()) return;
        ifstream fin(fileName);
        if (!fin.good()) return; // no file yet
        string line;
        while (getline(fin, line)) {
            line = trim(line);
            if (line.empty()) continue;
            // format: id name types_csv (types separated by '#')
            // We must support name without spaces per spec; safe to split by spaces.
            stringstream ss(line);
            long long idll; string name, typesStr;
            if (!(ss >> idll)) continue;
            if (!(ss >> name)) continue;
            if (!(ss >> typesStr)) typesStr = "";
            Pokemon p{};
            p.id = (int)idll;
            strncpy(p.name, name.c_str(), sizeof(p.name)-1);
            p.name[sizeof(p.name)-1] = '\0';
            p.types = detail::splitTypes(typesStr);
            byId[p.id] = p;
            nameToId[name] = p.id;
        }
    }

    void saveToFile() const {
        if (fileName.empty()) return;
        ofstream fout(fileName, ios::trunc);
        if (!fout.good()) return;
        for (const auto &kv : byId) {
            const auto &p = kv.second;
            fout << p.id << ' ' << p.name << ' ';
            for (size_t i = 0; i < p.types.size(); ++i) {
                if (i) fout << '#';
                fout << p.types[i];
            }
            fout << '\n';
        }
    }

    // Validate inputs and construct type vector; throws ArgumentException on invalid
    vector<string> validateTypesOrThrow(const string &typesStr) const {
        auto vec = detail::splitTypes(typesStr);
        if (vec.empty()) throw ArgumentException("Argument Error: PM Type Invalid ()");
        if (vec.size() < 1 || vec.size() > 7) throw ArgumentException("Argument Error: PM Type Invalid ()");
        // Each type must be alphabetic and in allowedTypes
        for (const auto &t : vec) {
            if (!detail::isAlphaStr(t)) {
                string msg = string("Argument Error: PM Type Invalid (") + t + ")";
                throw ArgumentException(msg.c_str());
            }
            string low = t; for (char &c : low) c = (char)tolower((unsigned char)c);
            if (!allowedTypes.count(low)) {
                string msg = string("Argument Error: PM Type Invalid (") + t + ")";
                throw ArgumentException(msg.c_str());
            }
        }
        // Return lowered
        for (auto &t : vec) for (char &c : t) c = (char)tolower((unsigned char)c);
        return vec;
    }

    static void validateNameOrThrow(const string &name) {
        if (!detail::isAlphaStr(name)) {
            string msg = string("Argument Error: PM Name Invalid (") + name + ")";
            throw ArgumentException(msg.c_str());
        }
        if (name.size() > 10) {
            string msg = string("Argument Error: PM Name Invalid (") + name + ")";
            throw ArgumentException(msg.c_str());
        }
    }

    static void validateIdOrThrow(long long id) {
        if (id <= 0 || id > 1000000000LL) {
            string msg = string("Argument Error: PM ID Invalid (") + to_string(id) + ")";
            throw ArgumentException(msg.c_str());
        }
    }

public:
    explicit Pokedex(const char *_fileName) {
        if (_fileName) fileName = _fileName; else fileName.clear();
        initTypeChart();
        loadFromFile();
    }

    ~Pokedex() {
        saveToFile();
    }

    bool pokeAdd(const char *name, int id, const char *types) {
        // Validate arguments first; any invalid throws ArgumentException
        string sname = name ? string(name) : string();
        string stypes = types ? string(types) : string();
        validateNameOrThrow(sname);
        validateIdOrThrow(id);
        auto vtypes = validateTypesOrThrow(stypes);

        // After validation, check uniqueness constraints; if duplicate, simply return false (no exception)
        if (byId.count(id)) return false;
        if (nameToId.count(sname)) return false;

        Pokemon p{};
        p.id = id;
        strncpy(p.name, sname.c_str(), sizeof(p.name)-1); p.name[sizeof(p.name)-1] = '\0';
        p.types = std::move(vtypes);
        byId[id] = p;
        nameToId[sname] = id;
        return true;
    }

    bool pokeDel(int id) {
        auto it = byId.find(id);
        if (it == byId.end()) return false;
        nameToId.erase(string(it->second.name));
        byId.erase(it);
        return true;
    }

    string pokeFind(int id) const {
        auto it = byId.find(id);
        if (it == byId.end()) return string("None");
        return string(it->second.name);
    }

    string typeFind(const char *types) const {
        string stypes = types ? string(types) : string();
        // Validate query types (same rules). Stop at first invalid.
        auto qtypes = validateTypesOrThrow(stypes);
        // Find pokemons that contain all qtypes
        vector<const Pokemon*> res;
        for (const auto &kv : byId) {
            const auto &p = kv.second;
            // check all qtypes in p.types
            bool ok = true;
            for (const auto &qt : qtypes) {
                bool found = false;
                for (const auto &pt : p.types) if (pt == qt) { found = true; break; }
                if (!found) { ok = false; break; }
            }
            if (ok) res.push_back(&p);
        }
        if (res.empty()) return string("None");
        // sort by id from small to large (byId already sorted). Build output: first line count then names by id asc
        string out;
        out += to_string(res.size());
        for (auto ptr : res) {
            out += "\n";
            out += ptr->name;
        }
        return out;
    }

    float attack(const char *type, int id) const {
        string at = type ? string(type) : string();
        if (!detail::isAlphaStr(at)) return -1.0f; // defensive; problem guarantees single attribute
        for (char &c : at) c = (char)tolower((unsigned char)c);
        auto it = byId.find(id);
        if (it == byId.end()) return -1.0f;
        const auto &p = it->second;
        // multiply across all defending types
        float m = 1.0f;
        for (const auto &dt : p.types) {
            auto f1 = mult.find(at);
            if (f1 == mult.end()) return 1.0f; // if not in chart, treat as neutral
            auto f2 = f1->second.find(dt);
            float factor = (f2 == f1->second.end()) ? 1.0f : f2->second;
            m *= factor;
        }
        return m;
    }

    int catchTry() const {
        if (byId.empty()) return 0;
        // Own the minimum id initially
        // We'll simulate expanding set while ability to deal >=2x to targets
        // Precompute for each owned type (single-typed attack) the ability to hit a target with >=2
        // But owned pokemons can use moves of their own types; multi-typed attacker => can choose any of its types.

        // We'll maintain a set of owned ids and a queue; iterate until no change.
        // For efficiency (n<=2000), O(n^2) is fine.

        // Prepare a vector for iteration in ascending id order
        vector<const Pokemon*> vec;
        vec.reserve(byId.size());
        for (auto &kv : byId) vec.push_back(&kv.second);

        // Owned set
        unordered_set<int> owned;
        owned.insert(vec.front()->id);

        bool changed = true;
        while (changed) {
            changed = false;
            for (auto *target : vec) {
                if (owned.count(target->id)) continue;
                // Check if any owned pokemon has an attack type that yields >=2x on target
                bool catchable = false;
                for (auto &kvOwned : owned) {
                    const Pokemon *op = byId.find(kvOwned)->second.id == kvOwned ? &byId.find(kvOwned)->second : nullptr;
                    if (!op) continue;
                    for (const auto &atk : op->types) {
                        float m = 1.0f;
                        for (const auto &dt : target->types) {
                            float f = 1.0f;
                            auto f1 = mult.find(atk);
                            if (f1 != mult.end()) {
                                auto f2 = f1->second.find(dt);
                                if (f2 != f1->second.end()) f = f2->second;
                            }
                            m *= f;
                        }
                        if (m >= 2.0f - 1e-6) { catchable = true; break; }
                    }
                    if (catchable) break;
                }
                if (catchable) {
                    owned.insert(target->id);
                    changed = true;
                }
            }
        }

        return (int)owned.size();
    }

    struct iterator {
        // Iterator over byId map in ascending id order
        using MapIter = map<int, Pokemon>::iterator;
        map<int, Pokemon> *mp;
        MapIter it;

        iterator() : mp(nullptr), it() {}
        iterator(map<int, Pokemon> *m, MapIter i) : mp(m), it(i) {}

        iterator &operator++() {
            if (!mp) throw IteratorException("Iterator Error: Invalid Iterator");
            if (it == mp->end()) throw IteratorException("Iterator Error: Invalid Iterator");
            ++it; return *this;
        }
        iterator &operator--() {
            if (!mp) throw IteratorException("Iterator Error: Invalid Iterator");
            if (it == mp->begin()) throw IteratorException("Iterator Error: Invalid Iterator");
            --it; return *this;
        }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }
        iterator & operator = (const iterator &rhs) { mp = rhs.mp; it = rhs.it; return *this; }
        bool operator == (const iterator &rhs) const { return mp == rhs.mp && it == rhs.it; }
        bool operator != (const iterator &rhs) const { return !(*this == rhs); }
        Pokemon & operator*() const { if (!mp || it == mp->end()) throw IteratorException("Iterator Error: Dereference"); return it->second; }
        Pokemon *operator->() const { if (!mp || it == mp->end()) throw IteratorException("Iterator Error: Dereference"); return &it->second; }
    };

    iterator begin() { return iterator(&byId, byId.begin()); }
    iterator end() { return iterator(&byId, byId.end()); }
};

#endif // SRC_HPP

