#include<bits/stdc++.h>

using namespace std;

#define ll long long

mt19937_64 rnd(chrono::steady_clock::now().time_since_epoch().count());

template <class Data>
struct Node{
    Data data;
    Node* next;
    Node* prev;
    Node(Data _data) : data(_data), next(nullptr), prev(nullptr){}
};

struct element{
    string townName;
    string regionName;
    int townPopulation;
};

template <class Data>
struct List{
    Node<element>* first;
    Node<element>* last;
    int numberOfElements;
    pair<Node<Data>*, int> lastPoint;

    List() : numberOfElements(0), lastPoint(make_pair(nullptr, 0)), first(nullptr), last(nullptr){}

    bool is_empty(){
        return first == nullptr;
    }

    int count(){
        return numberOfElements;
    }

    void add(Data* value){
        numberOfElements++;
        Node<Data>* currentElement = new Node<Data>(*value);
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

    void insert(int position, Data* value){
        numberOfElements++;
        Node<Data>* insertElement = new Node<Data>(*value);
        Node<Data>* currentElement = first;
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
        if(lastPoint.first != nullptr && lastPoint.second <= position){
            currentElement = lastPoint.first;
            for (int i = lastPoint.second; i < position; ++i) {
                currentElement = currentElement->next;
            }
        }
        else{
            for (int i = 0; i < position; ++i) {
                currentElement = currentElement->next;
            }
        }
        insertElement->next = currentElement;
        insertElement->prev = currentElement->prev;
        currentElement->prev->next = insertElement;
        currentElement->prev = insertElement;
        lastPoint = make_pair(insertElement, position);
    }

    void removeAt(int position){
        if(is_empty()){ return; }
        numberOfElements--;
        Node<Data>* currentElement = first;
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
        if(lastPoint.first != nullptr && lastPoint.second <= position){
            currentElement = lastPoint.first;
            for (int i = lastPoint.second; i < position; ++i) {
                currentElement = currentElement->next;
            }
        }
        else{
            for (int i = 0; i < position; ++i) {
                currentElement = currentElement->next;
            }
        }
        currentElement->prev->next = currentElement->next;
        currentElement->next->prev = currentElement->prev;
        delete currentElement;
        lastPoint = make_pair(nullptr, 0);
    }

    Data elementAt(int position){
        Node<Data>* currentElement = first;
        if(lastPoint.first != nullptr && lastPoint.second <= position){
            currentElement = lastPoint.first;
            for (int i = lastPoint.second; i < position; ++i) {
                currentElement = currentElement->next;
            }
        }
        else{
            for (int i = 0; i < position; ++i) {
                currentElement = currentElement->next;
            }
        }
        lastPoint = make_pair(currentElement, position);
        return currentElement->data;
    }

    void clear(){
        if(is_empty()){ return; }
        Node<Data>* currentElement = first;
        while(currentElement->next != nullptr){
            Node<Data>* forDel = currentElement;
            delete forDel;
            currentElement = currentElement->next;
        }
        delete currentElement;
        first = nullptr;
        last = nullptr;
    }
};

vector<pair<int, string>> regPop;

void merge(int left, int mid, int right){
    int it1 = 0, it2 = 0;
    vector<pair<int, string>> res(right - left);
    while(left + it1 < mid && mid + it2 < right){
        if(regPop[left + it1].first > regPop[mid + it2].first){
            res[it1 + it2] = regPop[it1 + left];
            it1++;
        }
        else{
            res[it1 + it2] = regPop[mid + right];
            it2++;
        }
    }
    while(left + it1 < mid){
        res[it1 + it2] = regPop[it1 + left];
        it1++;
    }
    while(mid + it2 < right){
        res[it1 + it2] = regPop[mid + it2];
        it2++;
    }
    for (int i = 0; i < it1 + it2; ++i) {
        regPop[left + i] = res[i];
    }
}

void mergeSort(int right, int left){
    if(left + 1 >= right){ return; }
    int mid = (right + left) / 2;
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
            element *value;
            cin >> population >> townName >> regionName;
            value->regionName = regionName;
            value->townName = townName;
            value->townPopulation = population;
            list.add(value);
        }
        else if(s == "insert"){
            int population, position;
            string townName, regionName;
            element* value;
            cin >> position >> population >> townName >> regionName;
            value->regionName = regionName;
            value->townName = townName;
            value->townPopulation = population;
            list.insert(position, value);
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
            string region;
            cin >> region;
            vector<int> forDel;
            for (int i = 0; i < list.count(); ++i) {
                element value = list.elementAt(i);
                if(value.regionName == region){
                    forDel.push_back(i);
                }
            }
            for(int i = 0; i < forDel.size(); i++){
                list.removeAt(forDel[i] - i);
            }
        }
        else if(s == "sort"){
            for (int i = 0; i < list.count(); ++i) {
                element value = list.elementAt(i);
                bool ch = 0;
                for (int j = 0; j < regPop.size(); ++j) {
                    if(regPop[i].second == value.regionName){
                        regPop[i].first += value.townPopulation;
                        ch = 1;
                        break;
                    }
                }
                if(!ch){
                    regPop.emplace_back(value.townPopulation, value.regionName);
                }
            }
            mergeSort(0, regPop.size());
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