/*
 ============================================================================
 Name        : Embry_proj01_FINAL
 Author      : Michael Embry
 Version     :
 Copyright   : � Michael Embry 2018
 Description : Trie Autocomplete
 ============================================================================
 */

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Node     //node in trie tree
{
    friend class Trie;
private:
    Node* child[128];         //node ptr for next node/child. containing ASCII value representation of alphabet
    bool isLeaf;             //returns true if the end of word is a leaf
    char letter;
    
public:
    Node();
    ~Node() {}
    
    void setLeaf(int wrd) {this->isLeaf = wrd;}     //setter
    bool isLeafX() {return this->isLeaf;}
    
    void setChar(char c) {this->letter = c;}        //setter
    char getChar() const {return this->letter;}        //getter
};

class Trie
{
private:
    Node* root;
    bool space;         //returns true if user enters a space
    string no_prefix;
    
    
public:
    Trie()
    {root = new Node(); this->space = false;}
    ~Trie() {}
    
    void insert(string key);                                 //insert into trie
    void search(string key);                                 //auto-completion prototype
    
    void is_space(string key);                                 //search for space in user input
    bool getSpace() {return space;}
    
    void remove_prefix(string key);                             //removes the space (if space in string)
    string getNo_prefix() {return no_prefix;}                    //getter
    string break_string(string);                             //returns the space in the string (in print and search)
    
    void print_tree(Node* root, string key, string str);     //prints auto-complete
    void print_tree(Node* root, string str);                 //overloaded recursive method for multiple keys
    
    
};

Node::Node()                         //default Node constructor
{
    this->setChar('\0');
    this->setLeaf(false);
    for (int i = 0; i < 128; i++)
        this->child[i] = NULL;
}

// If not present, inserts key into trie.  If the
// key is prefix of trie node, just marks leaf node
void Trie::insert(string key)
{
    Node* pCrawl = root;
    char word[key.length()];
    
    
    key.copy(word, key.length());
    for (int i = 0; i < key.length(); i++)
    {
        if (pCrawl->child[static_cast<int>(word[i])] != NULL)
            pCrawl = pCrawl->child[static_cast<int>(word[i])];
        
        else
        {
            pCrawl->child[static_cast<int>(word[i])] = new Node();     //else if prefix is not present, insert key in new node
            pCrawl = pCrawl->child[static_cast<int>(word[i])];
            pCrawl->setChar(word[i]);
        }
    }
    pCrawl->setLeaf(true); //mark last node as leaf
}

//searches for prefix in the trie and generates autocomplete
void Trie::search(string key)
{
    Node* pCrawl = root;
    char word[key.length()];
    
    key.copy(word, key.length(), 0);
    for (int i = 0; i < key.length(); i++)                         //move through tree to search for last char in user word
    {
        if (pCrawl->child[static_cast<int>(word[i])] == NULL)     //if input word does not have a next child, throw error
        {
            cout << "COULD NOT FIND STRING" << endl;
            return;
        }
        else
            pCrawl = pCrawl->child[static_cast<int>(word[i])];    //advance pointer
        
        if (pCrawl->getChar() != word[i])
        {
            cout << "COULD NOT FIND STRING" << endl;
            return;
        }
    }
    string str = key;                 //used to pass user query to print_tree method
    print_tree(pCrawl, key, str);    //print tree with ptr that has searched query
}

//checks for a space in user query
void Trie::is_space(string key)
{
    char word[key.length()];
    
    key.copy(word, key.length());
    for (int i = 0; i < key.length(); i++)
    {
        if (word[i] == ' ')
        {
            space = true;
            return;
        }
        else
            continue;
    }
}

//remove prefix from user query. used for autocomplete
void Trie::remove_prefix(string key)

{
    char word[key.length()];
    key.copy(word, key.length());
    
    //if user enters a space stop searching for autocomplete
    for (int i = key.length(); i-- > 0;)
    {
        if (word[i] == ' ')
            break;
        else
            word[i] = '\0';     //escape character signifies end of query string
    }
    no_prefix = word;             //remainder for autocomplete
}

//returns prefix if query contains a space
string Trie::break_string(string key)
{
    char word[key.length()];
    string str;
    key.copy(word, key.length());
    
    for (int i = key.length(); i-- > 0;)
    {
        if (word[i] == ' ')     //break loop if space
            break;
        else
            str.insert(str.begin(),word[i]);     //copies chars in generic string begining at the last character until loop encounters a space
    }
    return str; //returns the prefix only
}

//prints query suggestions
void Trie::print_tree(Node* root, string key, string str)
{
    for (int i = 0; i < 128; i++)     //checks for child
    {
        Node* adv = root;
        if (adv->child[i] != NULL)         //child exists
        {
            adv = adv->child[i];         //advance pointer
            str += adv->getChar();         //add char to prefix
            
            //if reaches a leaf, print word
            if (adv->isLeafX())
            {
                if (this->getSpace() == true) //if the user's search query contained a space, print the search query minus prefix then print word
                    cout << this->getNo_prefix();
                cout << str << endl;
            }
            print_tree(adv, str); //recursive call to keep moving down tree
        }
        str = key; //reset prefix string
    }
}

//overloaded function for recursive calls
void Trie::print_tree(Node* root, string str)
{
    string str_tmp = str;
    
    for (int i = 0; i < 128; i++)
    {
        Node* adv = root;
        if (adv->child[i] != NULL)
        {
            adv = adv->child[i];
            str += adv->getChar();
            if (adv->isLeafX())
            {
                if (this->getSpace() == true)
                    cout << this->no_prefix;
                cout << str << endl;
            }
            print_tree(adv, str);
        }
        str = str_tmp;
    }
}

int main(int argc, char const *argv[])
{
    string key;
    fstream dictionary;
    Trie complete_query;
    
    dictionary.open("Dictionary.txt", fstream::in); //read dictionary file into trie tree structure
    if (dictionary.is_open())
    {
        while (dictionary)
        {
            dictionary >> key;
            complete_query.insert(key);
        }
    }
    else
    {
        cout << "COULD NOT OPEN FILE" << endl;
        return -1;
    }
    
    dictionary.close();
    key.clear();
    
    cout << "Please type search queries: " << endl;
    getline(cin, key);
    
    //checks whether there's a space in user query
    complete_query.is_space(key);
    
    cout << "Your options are:  " << endl;
    
    //if there's a space, remove the prefix and pass to seach method
    if (complete_query.getSpace() == true)
    {
        complete_query.remove_prefix(key);
        complete_query.search(complete_query.break_string(key));
    }
    //else if no space pass query to search method
    else
        complete_query.search(key);
    
    return 0;
}
