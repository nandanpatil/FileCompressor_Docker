#include "decompress.h"
#include "compress.h"
using namespace std;

class codeDriver{
    clock_t startTime,endTime;
    string filename;
    string operation;
    public:
    codeDriver(vector<string>param){
        filename = param.back();
        operation = param[0];
        startTime=clock();
        operations();
    }

    void operations(){
        if(operation=="-c"){
            Compressor compressor(filename);
            compressor.countFrequency();
            compressor.createHuffmanTree();
            compressor.compress();
            endTime=clock();
            double time_taken = double(endTime - startTime) / double(CLOCKS_PER_SEC);
            cout<<"............. File compressed ..............\n";
            cout<<"filename : compressed.cmp\n";
            cout<<"Time taken :"<<time_taken << setprecision(5)<<"\n";  
        }
        else if(operation=="-dc"){
            Decompressor decompressor(filename);
            decompressor.deserializeTree();
            decompressor.decompress();
            endTime=clock();
            double time_taken = double(endTime - startTime) / double(CLOCKS_PER_SEC);
            cout<<"............. File Decompressed ..............\n";
            cout<<"filename : decompressed.txt\n";
            cout<<"Time taken :"<<time_taken << setprecision(5)<<"\n";  
        }
        else cout<<"Command does not exist\n";
    }  
};


int main(int argc,char *argv[])
{
    vector<string>params;

    if(argc<3){
        cout<<"Command does not exist.\n";
        cout<<"Help -> ./bytebinder -c|-dc filename|filename.cmp\n";
    }
    else{
        for(int i=1;i<=argc;i++){
            if(argv[i])
                params.push_back(argv[i]);
        }
        codeDriver cd(params);
    }

}

