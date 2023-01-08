#include <iostream> 
#include <iomanip> 
#include <ctime> 
#include <cstdlib> 
#include <fstream> 
#include <unordered_map> 
#include <vector> 
#include <cmath> 
#include <cstring> 
#include <algorithm>
using namespace std;

const int N = 4000;

struct record {
    char fiov[30];
    unsigned short int summ;
    char date[10];
    char fioa[22];
	// char author[12];
	// char title[32];
	// char publisher[16];
	// short int year;
	// short int num_of_page;
};

struct list {
	list *next; 
	record *data;
	list *prior;
};

struct queue {
	list *head;
	list *tail;
};

struct Vertex {
    record *data;
    Vertex *left;
    Vertex *right;
};

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
    char ch_arr[sizeof(record) * n];
    file.read((char *) ch_arr, sizeof(record) * n);
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

// string prompt(const string &str) {
//     cout << str;
//     cout << "\n> ";
//     string ans;
//     cin >> ans;
//     return ans;
// }

// void SDPREC(record *D, Vertex *&p) {
//     if (!p) {
//         p = new Vertex;
//         p->data = D;
//         p->left = nullptr;
//         p->right = nullptr;
//     } else if (D->year < p->data->year) {
//         SDPREC(D, p->left);
//     } else if (D->year >= p->data->year) {
//         SDPREC(D, p->right);
//     }
// }

// void A2(int L, int R, int w[], record *V[], Vertex *&root) {
//     int wes = 0, sum = 0;
//     int i;
//     if (L <= R) {
//         for (i = L; i <= R; i++)
//             wes += w[i];
//         for (i = L; i <= R - 1; i++) {
//             if ((sum < (wes / 2)) && ((sum + w[i]) > (wes / 2))) break;
//             sum += w[i];
//         }
//         SDPREC(V[i], root);
//         A2(L, i - 1, w, V, root);
//         A2(i + 1, R, w, V, root);
//     }
// }

// void Print_tree(Vertex *p) {
//     static int i = 1;
//     if (p) {
//         Print_tree(p->left);
//         cout << i++ << ")\t"
//             << p->data->author << "  |  "
//             << p->data->title << "  |  "
//             << p->data->publisher << "  |  "
//             << p->data->year << "  |  "
//             << p->data->num_of_page << endl;
//         Print_tree(p->right);
//     }
// }

// void search_in_tree(Vertex *root, int key) {
//      int i = 1;
//      while (root) {
//          if (key < root->data->year) {
//              root = root->left;
//          } else if (key > root->data->year) {
//              root = root->right;
//          } else if (key == root->data->year) {
//              cout << i++ << ")\t"
//              << root->data->author << "  |  "
//              << root->data->title << "  |  "
//              << root->data->publisher << "  |  "
//              << root->data->year << "  |  "
//              << root->data->num_of_page << endl;
//              root = root->right;
//          }
//      }
// }

// void rmtree(Vertex *root) {
//     if (root) {
//         rmtree(root->right);
//         rmtree(root->left);
//         delete root;
//     }
// }

// void tree(record *arr[], int n) {
//     Vertex *root = nullptr;
//     int w[n + 1];
//     int key;
//     for (int i = 0; i <= n; ++i) {
//         w[i] = rand() % 100;
//     }
//     A2(1, n, w, arr, root);
//     cout << "pos \t" 
// 		<< "Author       |  " 
// 		<< "Title                           |  " 
// 		<< "Publisher        |  " << "Year  |  " 
// 		<< "Pages" << endl;
    
//     Print_tree(root);
//     std::cout << "Enter search year: ";
//     std::cin >> key;
//     search_in_tree(root, key);
//     rmtree(root);
// }

// int strcomp(const string &str1, const string &str2, int len = -1) {
//     if (len == -1) {
//         len = (int) str1.length();
//     }
//     for (int i = 0; i < len; ++i) {
//         if (str1[i] == '\0' && str2[i] == '\0') {
//             return 0;
//         } else if (str1[i] == ' ' && str2[i] != ' ') {
//             return -1;
//         } else if (str1[i] != ' ' && str2[i] == ' ') {
//             return 1;
//         } else if (str1[i] < str2[i]) {
//             return -1;
//         } else if (str1[i] > str2[i]) {
//             return 1;
//         }
//     }
//     return 0;
// }

// void show_list(record *ind_arr[], int n = N) {
//     int ind = 0;
//     cout << "pos \t" 
// 		<< "Author       |  " 
// 		<< "Title                           |  " 
// 		<< "Publisher        |  " << "Year  |  " 
// 		<< "Pages" << endl;
//     for (int i = 0; i < n; i++) {
//         record *Record = ind_arr[ind + i];
//         cout << ind + i + 1 << ")\t"
//             << Record->author << "  |  "
//             << Record->title << "  |  "
//             << Record->publisher << "  |  "
//             << Record->year << "  |  "
//             << Record->num_of_page << endl;
//     }
// }

// int bin_search(record *arr[], const string &key) {
//     int l = 0;
//     int r = N - 1;
//     while (l < r) {
//         int m = (l + r) / 2;
//         if (strcomp(arr[m]->publisher, key, 3) < 0) {
//             l = m + 1;
//         } else {
//             r = m;
//         }
//     }
//     if (strcomp(arr[r]->publisher, key, 3) == 0) {
//         return r;
//     }
//     return -1;
// }

// void search(record *arr[], int &ind, int &n) {
//     string key;
//     do {
//         key = prompt("Input search key (3 characters)");
//     } while (key.length() != 3);
//     ind = bin_search(arr, key);
//     if (ind == -1) {
//         cout << "Not found\n";
//     } else {
//         int i;
//         for (i = ind + 1; strcomp(arr[i]->publisher, key, 3) == 0; ++i) {}
//         n = i - ind;
//         show_list(&arr[ind], n);
//     }
// }

// void delList(list *&p){
//   while(p){
//     list * temp = p->next;
//     delete p;
//     p = temp;
//   }
// }

// void delArr(record** &arr, unsigned int size){
//   for (unsigned int i = 0; i < size; i++)
//     delete arr[i];
//   arr = NULL;
// }

// void createIndexArr(record** &arr, list* p ,unsigned int size){
//   if (arr != NULL)
//     delArr(arr, size);
//   arr = new record *[size] { NULL };
//   for (int i = 0; p != NULL; p = p->next, i++)
//     arr[i] = *&(p->data);
// }

void printTableTitle() {
	cout << "pos \t" 
		<< "fiov       |  " 
		<< "sum                           |  " 
		<< "date        |  " 
        << "fioa  |  " << endl;
}

void printSpisEl(struct list *spisEl, int pos) {
	cout << pos << ")\t"
		<< spisEl->data->fiov << "  |  "
		<< spisEl->data->summ << "  |  "
		<< spisEl->data->date << "  |  "
		<< spisEl->data->fioa << "  |  " << endl;
}

void printSpis(struct list *head) {
	struct list *p;
	int i = 0;
	
	printTableTitle();
	
	for(p = head; p; p = p->next) {
		printSpisEl(p, i + 1);
		i++;
	}
    system("pause");
}

void printSpisN(struct list *head, int N) {
	struct list *p;
	p = head;
	for (int j = 0; j < N - 1; j++) {
		p = p->next;
	}
	
	printTableTitle();
	
	printSpisEl(p, N);
    system("pause");
}

void printSpisCount(struct list *head, int ch) {
	struct list *p;
	int size;
	
	cout << "Enter page size >>  ";
	cin >> size;
	
	p = head;
	
	printTableTitle();
	
	for (int j = 0; j < 4000 && ch != 'n' || ch == 1; j += size) {
		for (int i = j; i < j + size; i++) {
			printSpisEl(p, i + 1);
			p = p->next;
		}
		printf("if you want to continue then press 'y' or 'n' if not:   ");
		scanf(" %c", &ch);
	}
    system("pause");
}

void copy_base(list *a, list *b) {
	int N = 4000;
	b->prior = NULL;
	b->data = a->data;
	for (int i = 1; i < N; i++) {
		a = a->next;
		b->next = new list;
		b->next->prior = b;
		b = b->next;
		b->data = a->data;
	}
	b->next = NULL;
}

void prior(list *p) {
	int N = 4000;
	p->prior = NULL;
	for (int i = 1; i < N; i++) {
		p->next->prior = p;
		p = p->next;
	}
}
void CheckSpace (char *str, int N) {
	for (int i = 0; i<N; i++) {
		if(str[i]==' ')
			str[i]='_';
	}
}

// void digitalSort(list *&head) {
//     int first_f = 12;
//     int sec_f = 16;
//     struct Queue {
//         list *tail;
//         list *head;
//     } q[256];
//     int L = first_f + sec_f;
//     list *p;

//     for (int j = 0; j < L; j++) {
//         for (auto &i : q) {
//             i.tail = i.head = nullptr;
//         }
//         while (head) {
//             int d;
//             if (j < sec_f) {
//                 d = 255 - abs(head->data->date[L - j - 1 - first_f]);
//             } else if (j < sec_f + first_f) {
//                 d = 255 - abs(head->data->summ[L - j - 1]);
//             } else {
//                 throw out_of_range("d out of range");
//             }
//             if (d > 255 or d < 0) {
//                 throw out_of_range("Out of queue range");
//             }
//             p = q[d].tail;
//             if (q[d].head == nullptr)
//                 q[d].head = head;
//             else
//                 p->next = head;

//             p = q[d].tail = head;
//             head = head->next;
//             p->next = nullptr;
//         }
//         int i;
//         for (i = 0; i < 256; i++) {
//             if (q[i].head != nullptr)
//                 break;
//         }
//         head = q[i].head;
//         p = q[i].tail;
//         for (int k = i + 1; k < 256; k++) {
//             if (q[k].head != nullptr) {
//                 p->next = q[k].head;
//                 p = q[k].tail;
//             }
//         }
//     }
// }


list* read(FILE *file){
	struct list *head, *p;
	record* tmp;
	head=new list;
	head=NULL;
	unsigned char b[2];
	
	for(int i = 0; i < 4000; i++){
		p = new list;
		tmp = new record();

		fread(tmp->fiov, 30, 1, file);
		CheckSpace(tmp->fiov, 30);
		// std::cout << tmp->fiov << " ";

        fread(b, 2, 1, file);
		tmp->summ = (b[1]<<8)|b[0];
		// std::cout << tmp->summ << " ";

		fread(tmp->date, 10, 1, file);
		CheckSpace(tmp->date, 10);
		// std::cout << tmp->date << " ";
		
		fread(tmp->fioa, 22, 1, file);
		CheckSpace(tmp->fioa, 22);
		// std::cout << tmp->fioa << "\n\n" << std::endl;
		
		p->data = tmp;
		p->next=head;
		head=p;
	}
	return p = head;
}

void writeMenu() {
	system("cls");
	cout << "1. Print all base" << endl;
	cout << "2. Print N element" << endl;
	cout << "3. Print N elemnts" << endl;
	cout << "4. Switch on sort spis *" << endl;
	cout << "5. Switch on default spis" << endl;
	cout << "6. Binary search *" << endl;
	cout << "7. Tree *" << endl;
    cout << "8. Coding" << endl;
    cout << "9. Exit" << endl;
	cout << "Enter >> ";
}

int main() {
	list *base, *sbase, *spis;
	
	record **arr = NULL;
	
    int search_ind, search_n = -1;

	sbase = new list;
	
	FILE *file;
	file=fopen("testBase3.dat","rb");
	if(file==NULL){
		printf("Can't open file");
		exit(6);
	}
	
	base = read(file);
	
	copy_base(base, sbase);
	
	// digitalSort(sbase);
	
	// createIndexArr(arr, sbase, N);
	
	spis = base;
	
	while(1){
		int ch;
		
		writeMenu();
		
		scanf(" %d",&ch);
		
		switch(ch) {
			case 1:
				printSpis(spis);			
				break;
			case 2:
				int position;
				printf("Choose a position:  ");
				cin >> position;
				printSpisN(spis, position);
				break;
			case 3:
				printSpisCount(spis, ch);
				break;
			case 4:
				spis = sbase;
				break;
			case 5:
				spis = base;
				break;
			case 6:
		        // search(arr, search_ind, search_n);
                system("pause");
		        break;
			case 7:
		        if (search_n == -1) {
                    std::cout << "Please search first\n";
                } else {
                    // tree(&arr[search_ind], search_n);
                }
                system("pause");
		        break;
            case 8:
                coding();
                system("pause");
                break;
            case 9:
                system("cls");
                return 1;
            default:
				break;
		}
	}
	return 0;
}