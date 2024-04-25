#include<bits/stdc++.h>

using namespace std;

mt19937_64 rnd(chrono::steady_clock::now().time_since_epoch().count());

template <typename T>
struct Node{
    T data;
    Node* next;
    Node* prev;
    Node(T _data) : data(_data), next(nullptr), prev(nullptr){}
};

struct element{
    string townName;
    string regionName;
    int townPopulation;
};

template <typename T>
struct List{
    Node<T>* first;
    Node<T>* last;
    Node<T>* nowElement;
    int numberOfElements;
    pair<Node<T>*, int> lastPoint;

    List() : numberOfElements(0), lastPoint(make_pair(nullptr, 0)), first(nullptr), last(nullptr), nowElement(nullptr){}

    bool is_empty(){
        return first == nullptr;
    }

    int count(){
        return numberOfElements;
    }

    void getElement(int position){
        nowElement = first;
        if(lastPoint.first != nullptr && lastPoint.second <= position){
            nowElement = lastPoint.first;
            position -= lastPoint.second;
        }
        for (int i = 0; i < position; ++i) {
            nowElement = nowElement->next;
        }
    }

    void add(T value){
        numberOfElements++;
        Node<T>* currentElement = new Node<T>(value);
        if(is_empty()){
            first = currentElement;
            last = currentElement;
        }
        else{
            last->next = currentElement;
            currentElement->prev = last;
            last = currentElement;
        }
    }

    void insert(int position, T value){
        numberOfElements++;
        Node<T>* insertElement = new Node<T>(value);
        if(is_empty()){
            first = insertElement;
            last = insertElement;
            return;
        }
        else if(position == numberOfElements - 1) {
            last->next = insertElement;
            insertElement->prev = last;
            last = insertElement;
            return;
        }
        else if(position == 0){
            insertElement->next = first;
            first->prev = insertElement;
            first = insertElement;
            return;
        }
        else{
            getElement(position);
            insertElement->next = nowElement;
            insertElement->prev = nowElement->prev;
            nowElement->prev->next = insertElement;
            nowElement->prev = insertElement;
            lastPoint = make_pair(insertElement, position);
        }
    }

    void removeAt(int position){
        if(is_empty()){ return; }
        numberOfElements--;
        Node<T>* currentElement = first;
        if(position == 0){
            if(first == last){
                clear();
                return;
            }
            first = first->next;
            first->prev = nullptr;
            delete currentElement;
        }
        else if(position == numberOfElements){
            currentElement = last;
            last = last->prev;
            last->next = nullptr;
            delete currentElement;
        }
        else{
            getElement(position);
            nowElement->prev->next = nowElement->next;
            nowElement->next->prev = nowElement->prev;
            delete currentElement;
            lastPoint = make_pair(nullptr, 0);
        }
    }

    T elementAt(int position){
        getElement(position);
        lastPoint = make_pair(nowElement, position);
        return nowElement->data;
    }

    void clear(){
        if(is_empty()){ return; }
        Node<T>* currentElement = first;
        while(currentElement->next != nullptr){
            Node<T>* forDel = currentElement;
            delete forDel;
            currentElement = currentElement->next;
        }
        delete currentElement;
        first = nullptr;
        last = nullptr;
    }
};

element pack(int population, string townName, string regionName){
    element info;
    info.townPopulation = population;
    info.regionName = regionName;
    info.townName = townName;
    return info;
}

vector<pair<int, string>>regPop;

void merge(int left, int mid, int right){
    int it1 = 0, it2 = 0;
    vector<pair<int, string>> result(right - left);
    while(left + it1 < mid && mid + it2 < right){
        if(regPop[left + it1].first > regPop[mid + it2].first){
            result[it1 + it2] = regPop[left + it1];
            it1++;
        }
        else{
            result[it1 + it2] = regPop[mid + it2];
            it2++;
        }
    }
    while(left + it1 < mid){
        result[it1 + it2] = regPop[left + it1];
        it1++;
    }
    while(mid + it2 < right){
        result[it1 + it2] = regPop[mid + it2];
        it2++;
    }
    for (int i = 0; i < it1 + it2; ++i) {
        regPop[left + i] = result[i];
    }
}

void mergeSort(int left, int right){
    if(left + 1 >= right){
        return;
    }
    int mid = (left + right) / 2;
    mergeSort(left, mid);
    mergeSort(mid, right);
    merge(left, mid, right);
}

int main(int argc, char *argv[]){
    List<element> list;
    while(1){
        string s;
        cin >> s;
        if(s == "add"){
            int population;
            string townName, regionName;
            cin >> population >> townName >> regionName;
            list.add(pack(population, townName, regionName));
        }
        else if(s == "insert"){
            int population, position;
            string townName, regionName;
            cin >> position >> population >> townName >> regionName;
            list.insert(position, pack(population, townName, regionName));
        }
        else if(s == "removeAt"){
            int position;
            cin >> position;
            list.removeAt(position);
        }
        else if(s == "elementAt"){
            element value;
            int position;
            cin >> position;
            value = list.elementAt(position);
            cout << value.townName << ' ' << value.regionName << ' ' << value.townPopulation << '\n';
        }
        else if(s == "removeRegion"){
            string regionName;
            cin >> regionName;
            int it = 0, n = list.count();
            while(it < n){
                element value = list.elementAt(it);
                if(value.regionName == regionName){
                    list.removeAt(it);
                    n--;
                }
                else{
                    it++;
                }
            }
        }
        else if(s == "sort"){
            for (int i = 0; i < list.count(); ++i) {
                element value = list.elementAt(i);
                bool ch = 0;
                for(auto it : regPop){
                    if(it.second == value.regionName){
                        it.first += value.townPopulation;
                        ch = 1;
                        break;
                    }
                }
                if(!ch){
                    regPop.emplace_back(value.townPopulation, value.regionName);
                }
            }
            mergeSort(0, regPop.size());
            for (auto it : regPop) {
                cout << it.second << ' ' << it.first << '\n';
            }
        }
        else if(s == "count_elements"){
            cout << list.count() << '\n';
        }
        else if(s == "clear"){
            list.clear();
        }
        else if(s == "stop"){
            break;
        }
        else{
            cout << "unknown command\n";
        }
    }
    return 0;
}
