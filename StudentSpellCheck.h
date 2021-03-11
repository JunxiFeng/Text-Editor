#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>

class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck() { }
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
    struct Node
    {
        Node(const char & word)
        {
            letter=word;
            for(int i=0;i<28;i++) //Initialize all children to null
                children[i]=nullptr;
        }
        
        Node* children[28];
        char letter;
    };
    
    class Trie
    {
    public:
        Trie()
        {
            root=new Node(' '); //Constructor initializes root pointer to empty
        }
        void insert(const std::string &value)
        {
            Node* curr=root;
            for(int i=0;i<value.size();i++)
            {
                int index=value[i]-97;
                if(value[i]=='\'')
                    index=26;
                if(value[i]=='.')
                    index=27;
                if(index<=27 && index>=0) //Stripping all the non-letter, non-apostrophe characters
                    if(curr->children[index]==nullptr)
                        curr->children[index]=new Node(value[i]);
                curr=curr->children[index];
            }
        }
        void Free(Node* cur)
        {
            if(cur==nullptr)
                return;
            for(int i=0;i<28;i++)
                Free(cur->children[i]);
            delete cur;
        }
        
        Node* helper()
        {
            return root;
        }
    private:
        Node* root;
    };
    Trie* m_trie=new Trie();
};

#endif  // STUDENTSPELLCHECK_H_
