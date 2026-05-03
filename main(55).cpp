#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>

using namespace std;

struct Employee {
    string fullName;
    string position;
    int yearJoined;
    double salary;
};

struct TrieNode {
    map<char, TrieNode*> children;
    Employee* data;
    bool isEndOfWord;

    TrieNode() : data(nullptr), isEndOfWord(false) {}
};

TrieNode* createNode() {
    return new TrieNode();
}

void insert(TrieNode* root, const string& key, const Employee& emp) {
    TrieNode* curr = root;
    for (char ch : key) {
        if (curr->children.find(ch) == curr->children.end()) {
            curr->children[ch] = createNode();
        }
        curr = curr->children[ch];
    }
    curr->isEndOfWord = true;
    if (curr->data) delete curr->data;
    curr->data = new Employee(emp);
}

Employee* search(TrieNode* root, const string& key) {
    TrieNode* curr = root;
    for (char ch : key) {
        if (curr->children.find(ch) == curr->children.end()) return nullptr;
        curr = curr->children[ch];
    }
    return (curr != nullptr && curr->isEndOfWord) ? curr->data : nullptr;
}

bool hasChildren(TrieNode* node) {
    return !node->children.empty();
}

bool remove(TrieNode*& curr, string key, int depth) {
    if (!curr) return false;

    if (depth == key.size()) {
        if (curr->isEndOfWord) {
            curr->isEndOfWord = false;
            if (curr->data) {
                delete curr->data;
                curr->data = nullptr;
            }
            return !hasChildren(curr);
        }
    } else {
        char ch = key[depth];
        if (curr->children.count(ch) && remove(curr->children[ch], key, depth + 1)) {
            delete curr->children[ch];
            curr->children.erase(ch);
            return !curr->isEndOfWord && !hasChildren(curr);
        }
    }
    return false;
}

void serialize(TrieNode* root, string prefix, ofstream& out) {
    if (root->isEndOfWord && root->data) {
        out << prefix << "\n" 
            << root->data->position << "\n" 
            << root->data->yearJoined << "\n" 
            << root->data->salary << "\n";
    }
    for (auto const& pair : root->children) {
        serialize(pair.second, prefix + pair.first, out);
    }
}

void saveToFile(TrieNode* root, const string& filename) {
    ofstream out(filename);
    if (out.is_open()) {
        serialize(root, "", out);
        out.close();
    }
}

void loadFromFile(TrieNode* root, const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) return;

    string name, pos;
    int year;
    double sal;
    while (getline(in, name)) {
        getline(in, pos);
        in >> year >> sal;
        in.ignore();
        Employee emp = {name, pos, year, sal};
        insert(root, name, emp);
    }
    in.close();
}

void printAll(TrieNode* root) {
    if (root == nullptr) return;
    if (root->isEndOfWord && root->data) {
        cout << "--------------------------------" << endl;
        cout << "Prizvyshche: " << root->data->fullName << endl;
        cout << "Posada:      " << root->data->position << endl;
        cout << "Rik:         " << root->data->yearJoined << endl;
        cout << "Oklad:       " << root->data->salary << endl;
    }
    for (auto const& pair : root->children) {
        printAll(pair.second);
    }
}

int getValidatedInt(string prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) return value;
        cout << "Pomylka! Vvedit tsile chyslo.\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

double getValidatedDouble(string prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) return value;
        cout << "Pomylka! Vvedit chyslo.\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

void displayEmployee(const Employee* emp) {
    if (emp) {
        cout << "\n--- Informatsiya pro sluzhbovtsya ---" << endl;
        cout << "Prizvyshche: " << emp->fullName << endl;
        cout << "Posada: " << emp->position << endl;
        cout << "Rik pryiomu: " << emp->yearJoined << endl;
        cout << "Oklad: " << emp->salary << endl;
    } else {
        cout << "Sluzhbovtsya ne znaydeno." << endl;
    }
}

void menu() {
    TrieNode* root = createNode();
    int choice;
    string filename, name;

    do {
        cout << "\n1. Dodaty/Redahuvaty\n2. Vyluchyty\n3. Poshuk\n4. Spysok usikh\n"
             << "5. Zberehty u fayl\n6. Zavantazhyty z faylu\n0. Vykhid\nVash vybir: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                Employee e;
                cout << "Vvedit prizvyshche: "; getline(cin, e.fullName);
                cout << "Vvedit posadu: "; getline(cin, e.position);
                e.yearJoined = getValidatedInt("Vvedit rik pryiomu: ");
                e.salary = getValidatedDouble("Vvedit oklad: ");
                insert(root, e.fullName, e);
                break;
            }
            case 2:
                cout << "Vvedit prizvyshche dlya vydalennya: "; getline(cin, name);
                remove(root, name, 0);
                break;
            case 3:
                cout << "Vvedit prizvyshche dlya poshuku: "; getline(cin, name);
                displayEmployee(search(root, name));
                break;
            case 4:
                printAll(root);
                break;
            case 5:
                cout << "Vvedit imya faylu: "; getline(cin, filename);
                saveToFile(root, filename);
                break;
            case 6:
                cout << "Vvedit imya faylu: "; getline(cin, filename);
                loadFromFile(root, filename);
                break;
        }
    } while (choice != 0);
}

int main() {
    menu();
    return 0;
}
