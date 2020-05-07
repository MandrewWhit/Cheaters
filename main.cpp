#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <locale>

using namespace std;

/*
 * ********************************************************
 * hashtable linked list node struct definition
 *
 * ********************************************************
 */
typedef struct node{
    int data;
    node* next;
}node;
/*
 * ********************************************************
 * Linked list placing cheaters in order based on collisions
 * node definition
 *
 * ********************************************************
 */
typedef struct cheatersNode{
    int collisions;
    string fileNames;
    cheatersNode* next;
}cheatersNode;
/*
 * ********************************************************
 * Linked list placing cheaters in order based on collisions
 * List definition
 *
 * ********************************************************
 */
typedef struct cheatersList{
    cheatersNode* head;
}cheatersList;
/*
 * ********************************************************
 *
 * function definitions
 *
 * ********************************************************
 */
node** createHashTable(int numFiles);
void addToHashtable(const string &s, node* hashTable[], int fileIndex, int sum, int comparedIndex);
void fillEntireHashtable(string file, int c, node** hashTable, int comparedIndex, string path);
void destroyHashTable(node** hashTable);
int hashF(string s);

/*
 * **************************************************************
 * getdir function: fills a vector with file names from a directory
 * preconditions: a vector of strings and directory
 * postconditions: vector filled with file names
 * **************************************************************
 */
int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}




int main(int argc, char *argv[])
{
    string dir = argv[1];
    string path;
    vector<string> files = vector<string>();


    getdir(dir,files);//call get dir

    files.erase(files.begin());
    files.erase(files.begin());




    vector<string> caughtFiles;  //vectors to track caught files
    vector<int> cheatV;



    cheatersList myCheaters;    //set up caught linked list
    myCheaters.head = NULL;


    for(int i=0;i<files.size();i++) {     //loop through based on the number of files




        for (int c = 0; c < files.size(); c++) {     //inside loop to compare every file to the parent file
            auto hashTable = createHashTable(files.size());     //create the hashtable
            fillEntireHashtable(files[i], i, hashTable, i, path);    //fill the hashtable with chunks from a file
            if (i != c) {                                   //take loop if the child file doesn't equal parent file

                fillEntireHashtable(files[c], c, hashTable, i, path);     //fill hashtable with chunks from child file


                int count = 0;                               //count instances of collisions
                for (int i = 0; i < 1000000; i++) {
                    if (hashTable[i] != NULL) {
                        if ((hashTable[i]->next) != NULL) {
                            count++;

                        }
                    }
                }

                if (count > atoi(argv[2])) {                 //  if too many collisions between files add them to caught list
                    bool repeat = false;
                    int nameTotal = 0;
                    caughtFiles.push_back(files[i]);
                    caughtFiles.push_back(files[c]);
                    string cheatersNames = files[i] + "," + files[c];
                    for(int j=0;j<cheatersNames.length();j++){         //determine value of current string
                        cheatersNames.c_str();
                        int nameNum = int(cheatersNames[j]);
                        nameTotal = nameTotal + nameNum;
                    }
                    for(int j=0;j<cheatV.size(); j++){       //check for repitition
                        if(cheatV[j]==nameTotal){
                            repeat = true;
                        }
                    }
                    //add to cheaters linked list
                    if(!repeat) {
                        cheatV.push_back(nameTotal);              //push onto vector to avoid repitition
                        auto cheatPtr = new cheatersNode;
                        cheatersNode *p1 = myCheaters.head;       //add to linked list
                        if (p1 != NULL) {
                            while (p1->next != NULL) {
                                p1 = p1->next;
                            }
                            p1->next = cheatPtr;
                            cheatPtr->collisions = count;
                            string cheatersNames = files[i] + "," + files[c];
                            cheatPtr->fileNames = cheatersNames;
                            cheatPtr->next = NULL;
                        } else {
                            myCheaters.head = cheatPtr;
                            cheatPtr->collisions = count;

                            cheatPtr->fileNames = cheatersNames;
                            cheatPtr->next = NULL;
                        }
                    }


                    //cout << count << ": " << files[i] << ", " << files[c] << endl;

                }
            }
            destroyHashTable(hashTable);

        }
    }



                //print to screen and delete linked list
    while(myCheaters.head!=NULL) {
        cheatersNode *llPtr = myCheaters.head;
        cheatersNode *prevLast = NULL;
        int max = llPtr->collisions;
        if (myCheaters.head != NULL) {
            while (llPtr != NULL) {
                if (llPtr->collisions > max) {
                    max = llPtr->collisions;
                }
                prevLast = llPtr;
                llPtr = llPtr->next;
            }
            llPtr = myCheaters.head;
            prevLast = NULL;
            while (llPtr->collisions != max) {
                prevLast = llPtr;
                llPtr = llPtr->next;
            }
            cout << llPtr->collisions << ": " << llPtr->fileNames << endl;
            if (prevLast != NULL) {
                prevLast->next = llPtr->next;
                delete llPtr;
            } else {
                myCheaters.head = llPtr->next;
                delete llPtr;
            }
        }
    }

    return 0;
}

/*
 * *************************************************************************
 * destroyHashTable function: destroys the hash table
 * preconditions: valid hash table
 * postconditions: deallocated hash table
 *
 * *************************************************************************
 */
void destroyHashTable(node** hashTable){
    for(int i=0;i<1000000;i++){
        if(hashTable[i]!=NULL){
            node* ptr = hashTable[i];
            node* prev = NULL;
            while(ptr!=NULL){
                prev = ptr;
                ptr = ptr->next;
                delete prev;
            }
        }
    }
    delete[] hashTable;
}
/*
 * ************************************************************************
 * createHashTable function: generates a hashtable with 1000000 locations each holding a NULL pointer
 * precondition: none
 * postconditions: a hash table filled with NULL pointers
 *
 * ************************************************************************
 */
node** createHashTable(int numFiles){
    auto hashTable = new node*[1000000];
    for(int i=0;i<1000000;i++){
        hashTable[i] = NULL;
    }
    return hashTable;
}
/*
 * **********************************************************************
 * fillEntireHashtable function: fills the hashtable with values from any given file
 * precondition: a valid test file, hash table, and path
 * postcondition: filled hashtable
 *
 * **********************************************************************
 */
void fillEntireHashtable(string file, int c, node** hashTable, int comparedIndex, string path) {
    //string fileLoc = path + file; //"sm_doc_set/"
    string fileLoc = "sm_doc_set/" + file;
    ifstream inFile;
    inFile.open(fileLoc);


    string s;
    string item;
    int firstLen;
    int sLength;
    vector<int> charLength;
    for (int j = 0; j < 6; j++) {
        if (inFile) {
            inFile >> s;
            item = item + s;
            sLength = s.length();
            for (int i = 0; i < item.length(); i++) {
                char replace = tolower(item[i]);
                item[i] = replace;
                if ((int(item[i]) < 97) || (int(item[i]) > 122)) {
                    if ((int(item[i]) < 48) || (int(item[i]) > 57)) {
                        item.erase(i, 1);
                        i = i - 1;
                        sLength = sLength - 1;
                    }
                }
            }
            charLength.push_back(sLength);
        }
    }


    while (inFile) {
        s = "";
        inFile >> s;
        item = item + s;
        sLength = s.length();


        for (int i = 0; i < item.length(); i++) {
            char replace = tolower(item[i]);
            item[i] = replace;
            if ((int(item[i]) < 97) || (int(item[i]) > 122)) {
                if ((int(item[i]) < 48) || (int(item[i]) > 57)) {
                    item.erase(i, 1);
                    i = i - 1;
                    sLength = sLength - 1;
                }
            }
        }

        firstLen = charLength[0];
        charLength.erase(charLength.begin());
        item.erase(0, firstLen);
        charLength.push_back(sLength);


        int index = hashF(item);

        //cout << item << " : " << c << endl;

        addToHashtable(item, hashTable, c, index, comparedIndex);
    }
    //cout << "test55" << endl;
}
/*
 * *********************************************************
 * hash function: takes a string key and produces an index for the hash table
 * precondition: a valid string that with length not zero
 * postcondition: returns a valid index to hash table
 *
 * *********************************************************
 */
int hashF(string s){
    //new

    int power1 = 13;
    int length = s.length();
    int total = 0;
    s.c_str();
    for(int i=0; i < length-1; i++){
        for(int j=0;j<i;j++){
            power1 = power1*power1;
        }
        if((length-i-1)>(-1)){
        total = total + s[length-i-1]*power1;
        }
        power1 = 13;
    }
    if(total<0){
        total = total*(-1);
    }

    total = total%1000000;

    return (total);


}
/*
 * **********************************************************************************
 * addToHashtable function: adds the file index to a hash table at a given index
 * preconditions: valid hashtable, file index, and index to place file index on hashtable
 * postconditions: file index placed in correct location on hash table
 *
 * **********************************************************************************
 */
void addToHashtable(const string &s, node* hashTable[], int fileIndex, int sum, int comparedIndex) {

    //rewrite hash function


    //hash function

    node *ptr = hashTable[sum];
    node *prev = NULL;
    if (ptr == NULL) {
        hashTable[sum] = new node;
        hashTable[sum]->data = fileIndex;
        hashTable[sum]->next = NULL;
    } else {
        //cout << s << " : " << sum << endl;
        while (ptr != NULL) {
            //cout << s << " : " << comparedIndex << " : " << fileIndex << endl;
            prev = ptr;
            ptr = ptr->next;
        }
        ptr = new node;
        ptr->data = fileIndex;
        ptr->next = NULL;
        prev->next = ptr;
    }
}




