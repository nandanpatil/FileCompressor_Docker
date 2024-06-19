#include <bits/stdc++.h>
using namespace std;


class Node{
    char val;
    unsigned long long int freq;
    Node *left;
    Node *right;
    
    Node(){
        val='~';
        freq=-1;
        left=NULL;
        right=NULL;
    }

    Node(char c,unsigned long long int f){
        val=c;
        freq=f;
        left=NULL;
        right=NULL;
    }

    Node(unsigned long long int f){
        val='~';
        freq=f;
        left=NULL;
        right=NULL;
    }

    friend class Compressor;
    friend class pqComp;
};

class pqComp{
    public:
    bool operator()(Node const *a,Node const *b){
        return a->freq > b->freq;
    }
};


class Compressor{
    string fileName;
    unordered_map<char,unsigned long long int>frequency;
    unordered_map<char,string>huffcodes;
    Node *tree;
    string headerText;
    public:
    Compressor(string name){
        fileName=name;
        tree=NULL;
        headerText="";
    }
    void countFrequency(){
        fstream file;
        file.open(fileName,ios::in);
        if(!file.is_open()){
            cerr<<"Error opening file \n";
            return;
        }
        string line;
        while(getline(file,line)){
            for(unsigned long long int i=0;i<line.length();i++){
                frequency[line[i]]++;
            }
            frequency['\n']++;
        }}

    void createHuffmanTree(){
        priority_queue<Node*,vector<Node*>,pqComp>pq;
        for(auto it:frequency){
            Node *node = new Node(it.first,it.second);
            pq.push(node);
        }

        while(pq.size()>=2){
            Node *node1 = pq.top();
            pq.pop();
            Node *node2 =pq.top();
            pq.pop();

            unsigned long long int newTotalFreq = node1->freq+node2->freq;
            Node *parentNode = new Node(newTotalFreq);
            parentNode->left = node1;
            parentNode->right = node2;
            pq.push(parentNode);
        }

        tree  = pq.top();
        string temp="";
        assignCodes(tree,temp); 
        // printHuffTable();
        serializeTree(tree);       
    }

    void serializeTree(Node *root){
        queue<Node*>q;
        q.push(root);
        while(!q.empty()){
            Node* x = q.front();
            q.pop();
            if(x==NULL)headerText+="~~,";
            else{
                string txt="";
                if(x->val=='\n'){
                    txt+="nn";
                }
                else{
                txt+=x->val;
                }
                txt+=',';
                headerText+=txt;
            }
            if(x){
                q.push(x->left);
                q.push(x->right);
            }
        }
        headerText+="~~~,";
    }

    void assignCodes(Node *root,string &temp){
        if(!root)return;
        temp+="0";
        assignCodes(root->left,temp);
        temp.pop_back();
        if(root->val!='~'){
            huffcodes[root->val]=temp;
        }
        temp+="1";
        assignCodes(root->right,temp);
        temp.pop_back();
    }

    void printHuffTable(){
        for(auto it:huffcodes){
            cout<<it.first<<" "<<it.second<<" "<<endl;
        }
    }

    void compress(){
        fstream file,output;
        file.open(fileName,ios::in);

        string line;
        string binaryString="";
        while(getline(file,line)){
            for(int i=0;i<line.length();i++){
                char c = line[i];
                binaryString+=huffcodes[c];
            }
            binaryString+=huffcodes['\n'];
        }
        string outputfilename = "";
        for(int i=fileName.size()-5;i>=0;i--){
            if(fileName[i]=='/')break;
            else outputfilename=fileName[i]+outputfilename;
        }
        outputfilename = "./downloads/"+outputfilename+".cmp";
        cout<<outputfilename<<endl;
        writeHeader(outputfilename,headerText);
        writeToFile(binaryString,outputfilename);
    }

void writeToFile(const string& binaryString, const string& filename){
      ofstream outFile(filename, ios::binary|ios::app);

    if (!outFile) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    for (size_t i = 0; i < binaryString.length(); i += 8) {
        std::string byteString = binaryString.substr(i, 8);
        char byte = static_cast<char>(std::stoi(byteString, nullptr, 2));
        outFile.write(&byte, 1);
    }
    outFile.close();
    }

    void writeHeader(const string& filename, const string& text) {
   ofstream binaryFile(filename, ios::out | ios::binary);

    if (!binaryFile.is_open()) {
        cerr << "Error: Unable to open file for writing." << endl;
        return;
    }

    binaryFile.write(text.data(), text.size());
    binaryFile.close();

}

void levelOrder(){
    queue<Node*>q;
    q.push(tree);
    while(!q.empty()){
        int size = q.size();
        for(int i=0;i<size;i++){
            Node *x =q.front();
            q.pop();
            cout<<x->val<<" ";
            if(x->left)q.push(x->left);
            if(x->right)q.push(x->right);
        }
        cout<<endl;
    }
}

};
