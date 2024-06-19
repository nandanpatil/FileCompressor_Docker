#include<bits/stdc++.h>
using namespace std;
class NodeD{
    char val;
    NodeD *left;
    NodeD *right;
    
    NodeD(){
        val='~';
        left=NULL;
        right=NULL;
    }

    NodeD(char c){
        val=c;
        left=NULL;
        right=NULL;
    }

    friend class Decompressor;
};

class Decompressor{
    NodeD *TreeRoot;
    string fileText;
    string fileName;
    streampos dataStart;
    public:
    Decompressor(const string &filename){
        TreeRoot=NULL;
        fileText="";
        this->fileName=filename;
    }

    void deserializeTree(){
        string headerText = readHeader(fileName,dataStart);
        int cnt=0;
        stringstream s(headerText);
        string str; 
        getline(s, str, ',');
        NodeD *root = new NodeD(str[0]);
        queue<NodeD*> q; 
        q.push(root); 
        while(!q.empty()) {
            NodeD *Node = q.front(); 
            q.pop(); 
            getline(s, str, ',');
            if(str.length() && str == "~~") {
                Node->left = NULL; 
            }
            else {
                if(str.length()!=0){
                    if(str=="nn")str="\n";
                NodeD* leftNode = new NodeD(str[0]); 
                Node->left = leftNode; 
                q.push(leftNode); 
                }
                else if(cnt==0){
                NodeD* leftNode = new NodeD(','); 
                Node->left = leftNode; 
                q.push(leftNode);
                cnt++;
                getline(s, str, ',');
                }
                
            }
            
            getline(s, str, ',');
            if(str.length() && str == "~~") {
                Node->right = NULL; 
            }
            else {
                if(str.length()!=0){
                 if(str=="nn")str="\n";
                NodeD* rightNode = new NodeD(str[0]); 
                Node->right = rightNode;
                q.push(rightNode); 
                }
                else if(cnt==0){
                NodeD* rightNode = new NodeD(','); 
                Node->right = rightNode; 
                q.push(rightNode);
                cnt++; 
               getline(s, str, ',');
                }
            }
        }
       TreeRoot=root;
    }

string readHeader(const string& filename, streampos& stopPosition) {
    string headerText="";

    ifstream inputFile(filename); // Open the file for reading
    
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open file for reading." << endl;
        return ""; // Return empty vector if file cannot be opened
    }
    int cnt=0;
    string line;
    // Read the file line by line
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string word;
        // Read each word separated by comma
        while (getline(ss, word, ',')) {
            if (word == "~~~") {
                //cout<<inputFile.tellg();
                dataStart=headerText.length()+4;
                inputFile.close();// Close the file
                headerText.pop_back();
                return headerText; // Return the words read so far
            }
            headerText+=word;
            headerText+=',';
           
        }
    }
    
   // stopPosition = -1; // Set stopPosition to -1 if "~~~" was not found
    inputFile.close(); // Close the file when done
    return headerText;
}   

    void levelOrder(){
    queue<NodeD*>q;
    q.push(TreeRoot);
    while(!q.empty()){
        int size = q.size();
        for(int i=0;i<size;i++){
            NodeD *x =q.front();
            q.pop();
            cout<<x->val<<" ";
            if(x->left)q.push(x->left);
            if(x->right)q.push(x->right);
        }
        cout<<endl;
    }
}

    void decompress(){
        fstream outputfile;
        string outputfilename="";
        for(int i=fileName.size()-5;i>=0;i--){
            if(fileName[i]=='/')break;
            else outputfilename = fileName[i]+outputfilename;
        }
          outputfilename = "./downloads/"+outputfilename+".txt";
        outputfile.open(outputfilename,ios::out|ios::app);

        ifstream inputfile;
        inputfile.open(fileName,ios::binary|ios::in);
        inputfile.seekg(dataStart,ios::beg);
        char c;
        string binaryData="";
        while(inputfile.get(c)){
            binaryData+=bitset<8>(c).to_string();
        }
        // cout<<binaryData<<endl;

        NodeD *currNode=TreeRoot;
        for(auto bit:binaryData){
            if(bit=='0'){
                currNode=currNode->left;
            }
            else{
                currNode=currNode->right;
            }
            if(!currNode->left && !currNode->right){
                outputfile<<currNode->val;
                currNode=TreeRoot;
            }
        }
        inputfile.close();
        outputfile.close();
    }

    


};