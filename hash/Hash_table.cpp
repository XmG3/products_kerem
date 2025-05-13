
#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <vector>
using namespace std;

class Hash_table {
    private:
    int *arr;
    int size;

    public:
    Hash_table(int size) : size(size) {
        arr = new int[size];
        for(int i = 0; i < size; i++) {
            arr[i] = -2;
        }
    }
    ~Hash_table() { delete[] arr;}

    bool search(int key){
        int j = key % size;
        for (int i = 0; i < size; i++){
            if(arr[j] == key){
                return true;
            }
            if(arr[j] == -2){
            return false;
        }
        j += (i + 1) % size;
        }
        return false;
    }
    
    void insert(int key) {
        int j = key % size;
        for (int i = 0; i < size; i++){
            if(arr[j] == -2 || arr[j] == -1){
                arr[j] = key;
                return;
            }
            j += (i + 1);
            j = j % size;
        }
    }

    bool remove(int key){
        int j = key % size;
        for(int i = 0; i < size; i++){
            int inx = (j + i) % size;
            if(arr[inx] == key){
                arr[inx] = -1;
                return true;
            }
        if (arr[i] == -2){
            return false;
        }
        j += (i + 1);
        j = j % size;
        }
        return false;
    }

    int get_size() const {
        return size;
    }

    void print() {
        for(int i = 0; i < size; i++) {
            cout << "[" << i << "]: ";
            if(arr[i] == -2) cout << "EMPTY";
            else if(arr[i] == -1) cout << "DELETED";
            else cout << arr[i];
            cout << endl;
        }
    }
};

int main() {
    Hash_table ht(10);
    ht.insert(15);
    ht.insert(5);
    ht.remove(15);
    ht.print();

}