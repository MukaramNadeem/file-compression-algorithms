#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

struct Node{
    char ch;
    int freq;
    Node* left = nullptr;
    Node* right = nullptr;
};



// helper function to buid  MIN heap.
void heapify_min_heap(Node* array[], int size, int root) {

  // to calculate the left and right we have.
  int l = 2 * root + 1;
  int r = 2 * root + 2;

  // let the smallest as root at the start.
  int smallest = root;

  // update the smallest accordingly.
  // if left < smallest then smallest = left.
  if (l < size && array[l]->freq < array[smallest]->freq) {
    smallest = l;
  }
  // if right < smallest then smallest = right.
  if (r < size && array[r]->freq < array[smallest]->freq) {
    smallest = r;
  }
  // if smallest is not the root then heapify it.
  if (smallest != root) {
    // array[root] , array[smallest] = array[smallest] , array[root];
    Node* temp = array[root];
    array[root] = array[smallest];
    array[smallest] = temp;
    // Shift up function that heapify the other elements of heap-tress present above.
    heapify_min_heap(array, size, smallest); // recursion!!!
  }
}

// MIN Heap build fuction : It will use the `Shift DOWN` approach as we are using the reversed loop.
void build_min_heap(Node* array[],int size){
  //reversed loop for shift down.
  for (int i = size - 1; i >= 0; i--) {
    heapify_min_heap(array, size, i);//index is being passed : `i`
  }
}

//deletion function for min heap : It will use the `Shift Down` approach, starting from index 0.
Node* delete_min(Node * heap[] , int &size){
  if (size<=0) return nullptr;
  //Store the 0th element to return.
  Node* min_freq_node = heap[0];
  //swap last element with root
  heap[0] = heap[size-1];
  //remove last element
  size--;
  //passing 0 as root.
  heapify_min_heap(heap,size,0);
  
  return min_freq_node;
}



//function to make frequency table of characters in the text.
unordered_map<char, int> frequencyTable(string text) {
    unordered_map<char,int> freqMap;
    for(char c : text){
        freqMap[c]++;
    }
    return freqMap;
}


//function to make huffman tree.
Node* huffmanTree(Node* heap[],int &size) {
    while(size>1){
        Node* left = delete_min(heap,size);
        Node* right = delete_min(heap,size);
    
        Node* internal_node = new Node;
        internal_node->freq = left->freq+right->freq;
        internal_node->ch = '#';

        internal_node->left = left;
        internal_node->right = right;

        heap[size] = internal_node;
        size++;
        build_min_heap(heap,size);
    }
    //return head of huffman tree.
    return heap[0];
}

//assign binary to the each node in the huffman tree.
void generateCodes(Node* root, string currentCode, unordered_map<char, string>& codes) {
    //base case.
    if(root->left==nullptr && root->right == nullptr){
        codes[root->ch] = currentCode;
        return;
    }
    //recursive case.
    if(root->right != nullptr){ //right exists
        generateCodes(root->right,currentCode+"1",codes);
    }
    if(root->left != nullptr){ //left exists
        generateCodes(root->left,currentCode+"0",codes);
    }
}
//combine all the binaries of all the chars (against their specific binaries stored in map) in one string to replace the orignal text.
string encode(const string& text, unordered_map<char, string>& codes) {
    string result;
    for(char c : text){
        result += codes[c];
    }
    return result;
}

//decode the binary to text.
string decode(const string& encoded, Node* root) {
    string result;
    Node* current = root;
    for(char bit : encoded) {
        if(bit == '0'){
            current = current->left;
        }
        if(bit=='1'){
            current = current->right;
        } 
        if(current->left==nullptr && current->right == nullptr){
            result +=current->ch;
            current = root;
        }
    }
    return result;
}


int main(){
    string text = "hello worLd";
    unordered_map<char,int> result = frequencyTable(text);
    
    //print frequency table
    for(const auto&pair : result){
        cout<<pair.first << ":" << pair.second;
        cout<<endl; 
    }
    
    //build min_heap
    Node* heap[256];
    int heapSize = 0;

    for (const auto& pair : result) {
        Node* node = new Node();
        node->ch = pair.first;
        node->freq = pair.second;
        heap[heapSize] = node;
        heapSize++;
    }
    build_min_heap(heap,heapSize);

    //uncomment at your own risk!!!
    // //pop all and print : ascending frequency
    // while (!heap.empty()) {
    // cout << heap[0]->ch << " : " << heap[0]->freq << endl;
    // delete_min(heap);
    // }
    

    //Generate the Huffman Tree
    Node* root = huffmanTree(heap,heapSize);
    //print the huffman tree root->freq to confirm.
    cout << "Root frequency: " << root->freq << endl;

    //Generate codes and print
    unordered_map<char, string> codes;
    generateCodes(root, "", codes);

    for(const auto& pair : codes){
        cout << pair.first << " : " << pair.second << endl;
    }

    //encode and print
    string encoded = encode(text,codes);
    cout << encoded << endl;
    

    //decode and print
    string decoded = decode(encoded, root);
    cout << decoded << endl;
    return 0;
}
