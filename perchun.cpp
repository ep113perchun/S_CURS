#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

const int N = 4000;


struct Record {
    char fio[30];
    union {
        unsigned short int sum;
        unsigned char char_arr[2];
    };
    char date[10];
    char fio_adv[22];
};

struct Node {
    Record record;
    Node *next;
};

string prompt(const string &str) {
    cout << str;
    cout << "\n> ";
    string ans;
    cin >> ans;
    return ans;
}

int strcomp(const string &str1, const string &str2, int len = -1) {
    if (len == -1) {
        len = (int) str1.length();
    }
    for (int i = 0; i < len; ++i) {
        if (str1[i] == '\0' and str2[i] == '\0') {
            return 0;
        } else if (str1[i] == ' ' and str2[i] != ' ') {
            return -1;
        } else if (str1[i] != ' ' and str2[i] == ' ') {
            return 1;
        } else if (str1[i] < str2[i]) {
            return -1;
        } else if (str1[i] > str2[i]) {
            return 1;
        }
    }
    return 0;
}

void digitalSort(Node *&head) {
    int first_f = 2;
    int sec_f = 6;
    struct Queue {
        Node *tail;
        Node *head;
    } q[256];
    int L = first_f + sec_f;
    Node *p;
    int date_comparate_arr[] = {6, 7, 3, 4, 0, 1};
    for (int j = 0; j < L - 1; j++) {
        for (auto &i : q) {
            i.tail = i.head = nullptr;
        }
        while (head) {
            int d;
            if (j < sec_f) {
                d = abs(head->record.date[date_comparate_arr[L - j - 1 - first_f]]);
            } else if (j < sec_f + first_f) {
                d = (unsigned short) head->record.char_arr[L - j - 1];
            }
            p = q[d].tail;
            if (q[d].head == nullptr)
                q[d].head = head;
            else
                p->next = head;

            p = q[d].tail = head;
            head = head->next;
            p->next = nullptr;
        }
        int i;
        for (i = 0; i < 256; i++) {
            if (q[i].head != nullptr)
                break;
        }
        head = q[i].head;
        p = q[i].tail;
        for (int k = i + 1; k < 256; k++) {
            if (q[k].head != nullptr) {
                p->next = q[k].head;
                p = q[k].tail;
            }
        }
    }
}

Node *load_to_memory() {
    Node *root = nullptr;
    ifstream file("testBase3.dat", ios::binary);
    if (not file.is_open()) {
        return nullptr;
    }

    for (int i = 0; i < N; ++i) {
        Record record;
        file.read((char *) &record, sizeof(Record));
        root = new Node{record, root};
    }
    file.close();
    return root;
}

void print_head() {
    std::cout << "Record  Fio                            Sum    Date       Fio adv\n";
}

void print_record(Record *record) {
    std::cout << "  " << record->fio
              << "  " << setw(5) << record->sum
              << "  " << record->date
              << "  " << record->fio_adv << "\n";
}

void show_list(Record *records[], int n = N) {
    int ind = 0;
    while (true) {
        print_head();
        for (int i = ind; i < ind + 20 && i < n; i++) {
            Record *record = records[i];
            cout << "[" << setw(4) << i + 1 << "]";
            print_record(record);

        }
        string chose = prompt("w: Next page\t"
                              "q: Last page\t"
                              "e: Skip 10 next pages\n"
                              "s: Prev page\t"
                              "a: First page\t"
                              "d: Skip 10 prev pages\n"
                              "Any key: Exit");
        switch (chose[0]) {
            case 'w':
                ind += 20;
                break;
            case 's':
                ind -= 20;
                break;
            case 'a':
                ind = 0;
                break;
            case 'q':
                ind = n / 20 * 20;
                break;
            case 'd':
                ind -= 200;
                break;
            case 'e':
                ind += 200;
                break;
            default:
                return;
        }
        if (ind < 0) {
            ind = 0;
        }
        while (ind >= n) {
            ind -= 20;
        }
    }
}

void make_index_array(Record *arr[], Node *root, int n = N) {
    Node *p = root;
    for (int i = 0; i < n; i++) {
        arr[i] = &(p->record);
        p = p->next;
    }
}

int quick_search(Record *arr[], int key) {
    int l = 0;
    int r = N - 1;
    while (l < r) {
        int m = (l + r) / 2;
        if (arr[m]->sum < key) {
            l = m + 1;
        } else {
            r = m;
        }
    }
    if (arr[r]->sum == key) {
        return r;
    }
    return -1;
}

void search(Record *arr[], int &ind, int &n) {
    Node *head = nullptr, *tail = nullptr;
    int key;
    key = stoi(prompt("Input search key (sum)"));
    ind = quick_search(arr, key);
    if (ind == -1) {
        cout << "Not found\n";
    } else {
        head = new Node{*arr[ind], nullptr};
        tail = head;
        int i;
        for (i = ind + 1; i < 4000 && arr[i]->sum == key; ++i) {
            tail->next = new Node{*arr[i], nullptr};
            tail = tail->next;
        }
        n = i - ind;
        Record *find_arr[n];
        make_index_array(find_arr, head, n);
        show_list(find_arr, n);
    }
}

struct Vertex {
    Record *data;
    Vertex *left;
    Vertex *right;
};

void SDPREC(Record *D, Vertex *&p) {
    if (!p) {
        p = new Vertex;
        p->data = D;
        p->left = nullptr;
        p->right = nullptr;
    } else if (strcomp(D->fio, p->data->fio) < 0) {
        SDPREC(D, p->left);
    } else {
        SDPREC(D, p->right);
    }
}

void A2(int L, int R, int w[], Record *V[], Vertex *&root) {
    int wes = 0, sum = 0;
    int i;
    if (L <= R) {
        for (i = L; i <= R; i++)
            wes += w[i];
        for (i = L; i <= R - 1; i++) {
            if ((sum < (wes / 2)) && ((sum + w[i]) > (wes / 2))) break;
            sum += w[i];
        }
        SDPREC(V[i - 1], root);
        A2(L, i - 1, w, V, root);
        A2(i + 1, R, w, V, root);
    }
}

void Print_tree(Vertex *p) {
    static int i = 1;
    if (p) {
        Print_tree(p->left);
        cout << "[" << setw(4) << i++ << "]";
        print_record(p->data);
        Print_tree(p->right);
    }
}

void search_in_tree(Vertex *root, const string &key) {
	int i = 1;
    if (root) {
        if (strcomp(key, root->data->fio) < 0) {
            search_in_tree(root->left, key);
        } else if (strcomp(key, root->data->fio) > 0) {
            search_in_tree(root->right, key);
        } else if (strcomp(key, root->data->fio) == 0) {
            search_in_tree(root->left, key);
            print_record(root->data);
            search_in_tree(root->right, key);
        }
    }
}

void rmtree(Vertex *root) {
    if (root) {
        rmtree(root->right);
        rmtree(root->left);
        delete root;
    }
}

void tree(Record *arr[], int n) {
    Vertex *root = nullptr;
    int w[n + 1];
    for (int i = 0; i <= n; ++i) {
        w[i] = rand() % 100;
    }
    A2(1, n, w, arr, root);
    print_head();
    int i = 1;
    Print_tree(root);

    string key;
    getline(cin, key);
    i = 1;
    do {
        cout << "Input search key1 (fio), q - exit\n";
        getline(cin, key);
        if (!key.empty()) {
            print_head();
            search_in_tree(root, key);
        }
    } while (key != "q");
    rmtree(root);
}

void Mur(const int n, double p[], int Length[], char c[][20]) {
    double *q = new double[n];
    Length[0] = -floor(log2(p[0])) + 1;
    q[0] = p[0] / 2;
    for (int i = 1; i < n; ++i) {
        double tmp = 0;
        for (int k = 0; k < i; k++)
            tmp += p[k];

        q[i] = tmp + p[i] / 2;
        Length[i] = -floor(log2(p[i])) + 1;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < Length[i]; ++j) {
            q[i] *= 2;
            c[i][j] = floor(q[i]) + '0';
            if (q[i] >= 1) {
                q[i] -= 1;
            }
        }

    }
}

unordered_map<char, int> get_char_counts_from_file(const string &file_name, int &file_size, int n = N) {
    ifstream file(file_name, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Could not open file");
    }
    char ch_arr[sizeof(Record) * n];
    file.read((char *) ch_arr, sizeof(Record) * n);
    file.close();

    unordered_map<char, int> counter_map;
    file_size = 0;
    for (auto ch : ch_arr) {
        counter_map[ch]++;
        file_size++;
    }
    return counter_map;
}

vector<pair<double, char>> calc_probabilities(const unordered_map<char, int> &counter_map, int count) {
    vector<pair<double, char>> probabilities;
    probabilities.reserve(counter_map.size());
    for (auto i : counter_map) {
        probabilities.emplace_back(make_pair((double) i.second / count, i.first));
    }
    return probabilities;
}

void coding() {
    int file_size;
    unordered_map<char, int> counter_map;
    counter_map = get_char_counts_from_file("testBase3.dat", file_size);

    auto probabilities = calc_probabilities(counter_map, file_size);
    counter_map.clear();

    sort(probabilities.begin(), probabilities.end(),greater<pair<double, char>>());
    cout << "Probabil.  char\n";
    for (auto i : probabilities) {
        cout << fixed << i.first << " | " << i.second << '\n';
    }

    const int n = (int) probabilities.size();

    auto c = new char[n][20];
    auto Length = new int[n];
    auto p = new double[n];
    for (int i = 0; i < n; ++i) {
        p[i] = probabilities[i].first;
    }

    double shen = 0;
    Mur(n, p, Length, c);
    cout << "\nMur Code:\n";
    cout << "\nCh  Prob      Code\n";
    double avg_len = 0;
    double entropy = 0;
    for (int i = 0; i < n; i++) {
        avg_len += Length[i] * p[i];
        entropy -= p[i] * log2(p[i]);
        printf("%c | %.5lf | ", probabilities[i].second, p[i]);
        for (int j = 0; j < Length[i]; ++j) {
            printf("%c", c[i][j]);
        }
        cout << '\n';
        shen += p[i];
    }
    cout << "Average length = " << avg_len << '\n'
         << "Entropy = " << entropy << '\n'
         << "Average length < entropy + 2\n"
         << "N = " << n << endl<<endl;
}


void mainloop(Record *unsorted_ind_array[], Record *sorted_ind_array[]) {
    int search_ind, search_n = -1;
    while (true) {
        std::string chose = prompt("1: Show unsorted list\n"
                                   "2: Show sorted list\n"
                                   "3: Search\n"
                                   "4: Tree\n"
                                   "5: Murr\n"
                                   "Any key: Exit");
        switch (chose[0]) {
            case '1':
                show_list(unsorted_ind_array);
                break;
            case '2':
                show_list(sorted_ind_array);
                break;
            case '3':
                search(sorted_ind_array, search_ind, search_n);
                break;
            case '4':
                if (search_n == -1) {
                    std::cout << "Please search first\n";
                } else {
                    tree(&sorted_ind_array[search_ind], search_n);
                }
                break;
            case '5':
                coding();
                break;
            default:
                return;
        }
    }
}

int main() {
    Node *root = load_to_memory();
    if (!root) {
        cout << "File not found" << endl;
        return 1;
    }
    Record *unsorted_ind_arr[N];
    Record *sorted_ind_arr[N];
    make_index_array(unsorted_ind_arr, root);
    digitalSort(root);
    make_index_array(sorted_ind_arr, root);
    mainloop(unsorted_ind_arr, sorted_ind_arr);
}
