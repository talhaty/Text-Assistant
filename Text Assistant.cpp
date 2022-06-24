#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <vector>
#include <queue>
#include <fstream>
#include <windows.h>
#include <string>
#include <ctime>
#define fileName "abc.txt"
#define SIZE 17012
using namespace std;

string to_Lower(string);
queue<string> sentenceToWords(string);

//convert a sentence into words and add those words into queue
queue<string> sentenceToWords(string str){
    string word = "";
    queue<string> q;
    int len =str.length();
    char x;
    for (int i=0; i<len; i++) {

        x = str[i];
        
        if (x == ' ' || x == ',' || x == '.' || x == '!' || x == '?'){
            word = to_Lower(word);
            
            if(word == "")
                continue;

            q.push(word);
            word = "";
        }
        else {
            word = word + x;
        }
    }
    if(word != "")
        q.push(word);
    return q;
}


//convert a string to lower case 
string to_Lower(string word){
    for(int i=0; i<word.length(); i++){
        word[i] = tolower(word[i]);
    }
    return word;
}
  
struct node{
    string word;
    int frequency;

    node(){
        frequency = 0;
    }
};

struct root{
    string word;
    vector<node*> list;

    root(){
        list.clear();
    }
};

class abc{
    vector<root*> Root;
    //Data structure used here is like a tree which has different roots and they can have many children
    //Every word that comes after a specific word is the child of that word 
    // help   ---->  (word = her)(frequecy = 2), (word = him)(frequency = 4), (word = me)(frequency = 7)
    // he     ---->  (word = is)(frequecy = 2), (word = will)(frequency = 4)
    // .....
    public:

    abc(){
        Root.clear();
    }

    void addQueue(queue<string> q){
        string word, word2;
        bool found, found2;
        word = q.front();
        word2 = q.front();

        q.pop();

        while(!q.empty()){
            word = word2;
            word2 = q.front();

            found = false;

            for(int i=0; i < Root.size(); i++){

                if(Root[i]->word == word){
                    
                    found2 = false;

                    for(int j=0; j<Root[i]->list.size(); j++){

                        if(Root[i]->list[j]->word == word2){
                            Root[i]->list[j]->frequency++;
                            found2 = true;

                            break;
                        }
                    }

                    if(found2 == false){
                        node* newNode;
                        newNode = new node;
                        newNode->word = word2;
                        newNode->frequency++;
                        Root[i]->list.push_back(newNode);
                    }

                    found = true;
                    break;
                }
            }

            if(found == false){
                root* newRoot;
                newRoot = new root;
                newRoot->word = word;
                Root.push_back(newRoot);

                node* newNode;
                newNode = new node;
                newNode->word = word2;
                newNode->frequency++;
                Root[Root.size()-1]->list.push_back(newNode);
            }

            q.pop();
        }
    }

    void readFile(){
        ifstream read;
        read.open(fileName);
        string line;
        while(getline(read, line)){
            queue<string> q;
            q = sentenceToWords(line);
            addQueue(q);
        }
        read.close();
    }

    void print(string word){
        for(int i=0; i<Root.size(); i++){
            if(Root[i]->word == word){
                cout<<Root[i]->word<<" -> ";
                for(int j=0; j<Root[i]->list.size(); j++){
                    cout<<Root[i]->list[j]->word<<":"<<Root[i]->list[j]->frequency<<"  ";
                }
            }
        }
    }

    bool equal(string a1, string a2){
        if(a1 == "")
            return true;
        for(int i=0; i<a1.size(); i++){
            if(a1[i] != a2[i]){
                return false;
            }
        }
        return true;
    }

    int findMin(int array[4]){
        int min=0;
        for(int i=0; i<4; i++){
            if(array[min] > array[i])
                min = i;
        }
        return min;
    }

    string* Search(string word, string current = ""){
        word = to_Lower(word);
        current = to_Lower(current);

        string *words = new string[4];
        words[0] = "";
        words[1] = "";
        words[2] = "";
        words[3] = "";

        int frequency[4], min;

        bool found, found2, isEmpty;
        
        found = false;
        for(int i=0; i<Root.size(); i++){
            
            if(Root[i]->word == word){
                
                for(int j=0; j<Root[i]->list.size(); j++){

                    found2 = equal(current, Root[i]->list[j]->word);

                    if(found2 == true){
                        isEmpty = false;

                        for(int k=0; k<4; k++){

                            if(words[k] == ""){
                                words[k] = Root[i]->list[j]->word;
                                frequency[k] = Root[i]->list[j]->frequency;
                                isEmpty = true;
                                break;
                            }
                        }

                        if(isEmpty == false){
                        
                            min = findMin(frequency);
                            if(frequency[min] < Root[i]->list[j]->frequency){
                                words[min] = Root[i]->list[j]->word;
                                frequency[min] = Root[i]->list[j]->frequency;
                            }
                        
                        }
                    }
                }
                found = true;
            }
        }
        return words;
    }

    void writeFile(queue<string> q){
        ofstream write;
        write.open(fileName, ios::app);
        write<<endl;
        while(!q.empty()){
            write<<q.front()<<' ';
            q.pop();
        }
        write.close();
    }

};




int is_Empty(string* words){
    int k=0;
    for(int i=0; i<4; i++){
        if(words[i] == "" || words[i] == " ")
            k++;
    }
    return k;
}


bool Equal(string a1, string a2){
        if(a1 == "")
            return true;
        for(int i=0; i<a1.size(); i++){
            if(a1[i] != a2[i]){
                return false;
            }
        }
        return true;
}


string* randomPrediction(ifstream& read, string* words, string current=""){
    if(current == ""){
        read.clear();
        read.seekg(0, ios::beg);
    }
    int total = is_Empty(words), x;
    string a;
    if(!is_Empty(words)){
        return words;
    }

    if(current == ""){
        for(int i=0; i<total; i++){
            x = rand()%SIZE;
            for(int j=0; j<x; j++){
                getline(read, a);
            }

            for(int j=0; j<4; j++){
                if(words[j] == ""){
                    words[j] = a;
                    break;
                }
            }
            if(!is_Empty(words)){
                return words;
            }
        }
        return words;
    }
    
    bool EQUAL;
    bool found;
    while(getline(read, a)){
        EQUAL = Equal(current, a);
        if(EQUAL == true){
            found = false;
            for(int j=0; j<4; j++){
                if(words[j] == a){
                    found = true;
                    break;
                }
                    
            }
            if(found == false){
                for(int j=0; j<4; j++){
                    if(words[j] == ""){
                        words[j] = a;
                        break;
                    }
                }
            }
        }

        if(!is_Empty(words)){
            return words;
        }
    }
    return words;
}

void Write_File(string word){
    if(word == "" || word == " ")
        return;
    ofstream write;
    write.open("words_alpha.txt", ios::app);
    word = to_Lower(word);
    write<<'\n'<<word;
}

void menu(){
    cout<<"\t\t\t\t\t\t* TEXT ASSISTANT *\n\n";
    cout<<"Text Assistant which will pridict 4 words accourding to the previous words enterered by the user\n";
    cout<<"It will also learn User's way of typing and try to pridict a word which user has entered most number of times taking the previous word into consideration\n";
    cout<<"You can use Backspace to remove a character from the word just like a normal text editor\n";
    cout<<"You have to enter the first word in order for the program to make its prediction\n";

    cout<<"\n\n\t***** YOU CAN SELECT ANY OPTION BY ENTERING 1,2,3,4 *****\n";
    cout<<"\n\n\t\t\t\t\t\tSTART TYPING . . . . .\n";
}
int main(){
    abc a;
    char x, y;
    ifstream read;
    read.open("words_alpha.txt");
    

    srand(time(0));
    string word="", current="", line="", output="";
    string* words;
    a.readFile();
    bool pred = false;
    menu();
    while(true){
        x = getch();
        system("cls");
        
        menu();
        bool pred = false;
        if(x == '\b' && line[line.size()-1] != ' ' && line[line.size()-1] != ',' && line[line.size()-1] != '.'){
            line = line.substr(0, line.size()-1);
            output = output.substr(0, output.size()-1);
            current = current.substr(0, current.size()-1);
        }
        else if(x == '\b'){
            cout<<output;
            cout<<"\n\n\n\n\n\n";
            for(int i=0; i<4; i++){
                cout<<i+1<<". "<<words[i]<<"    ";
            }
            continue;
        }
        if(x == '1' || x == '2' || x == '3' || x == '4'){
            while(line[line.size()-1] != ' '){
                line = line.substr(0, line.size()-1);
                output = output.substr(0, output.size()-1);
            }
            pred = true;
            if(x == '1'){
                line.append(words[0]);
                output.append(words[0]);
                x=' ';
                current = words[0];
            }
            else if(x=='2'){
                line.append(words[1]);
                output.append(words[1]);
                x=' ';
                current = words[1];
            }
            else if(x == '3'){
                line.append(words[2]);
                output.append(words[2]);
                x=' ';
                current = words[2];
            }
            else if(x == '4'){
                line.append(words[3]);
                output.append(words[3]);
                x=' ';
                current = words[3];

            }
        }
        if(x != '\b'){
            line.push_back(x);
            output.push_back(x);
        }
        cout<<output;
        if(!(x == ' ' || x == ',' || x == '.' || x == '!' || x == '?' || x == '\b'))
            current.push_back(x);
        if(x=='.' || x=='!' || x== '?'){
            queue<string> q;
            q = sentenceToWords(line);
            a.addQueue(q);
            a.writeFile(q);
            line = "";
        }
        if( x == ' ' || x == ','){
            if(word != ""){
                queue<string> q;
                q.push(word);
                q.push(current);
                a.addQueue(q);
                
            }
            if(pred == false)
                Write_File(current);
            word = current;
            current = "";
        }
        
        if(word != ""){
            cout<<"\n\n\n\n\n\n";
            words = a.Search(word, current);
            words = randomPrediction(read, words, current);
            for(int i=0; i<4; i++){
                cout<<i+1<<". "<<words[i]<<"    ";
            }
        }
    }
}