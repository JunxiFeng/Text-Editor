#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
SpellCheck* createSpellCheck()
{
    return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck()
{
    m_trie->Free(m_trie->helper());
    delete m_trie;// TODO
}

bool StudentSpellCheck::load(std::string dictionaryFile)
{
    std::ifstream infile(dictionaryFile);
    if(!infile)
    {
        return false;
    }
    std::string s;
    while(getline(infile,s))
    {
        m_trie->insert(s+"."); //It ends with a dot as the end
    }
     
    return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions)
{
    Node *curr=m_trie->helper();//Point to the root of the trie
    word=word+".";
    bool check=true;
    for(int i=0;i<word.size();i++)
    {
        char a=word[i];
        int index=word[i]-97;
        if(index<0) //Upper case
            index=a-65;
        if(word[i]=='\'')
            index=26;
        if(word[i]=='.')
            index=27;
        if(curr->children[index]==nullptr)
        {
            std:: vector<std::string>:: iterator it=suggestions.begin();
            while(it!=suggestions.end())
                it=suggestions.erase(it);
            check=false;
            break;
        }
        else
            curr=curr->children[index];
    }
    
     if(check == false)
     {
        std::string temp; //Make a copy of the word passed in
        for(int i =0; i< word.size(); i++)
        {
            temp=word;
            for(int j =0; j<27; j++)
            {
                if(j==26)
                    temp[i]='\'';
                else
                temp[i] = 'a'+j;//Each position of the word changes accordingly
                Node * theNode = m_trie->helper();
                bool check2 = true;
                
                for(int k=0; k < word.size(); k++)
                {
                    char b = temp[k];
                    int index2 = b-97;
                    if(index2 <0) //Upper Lower case conversion
                        index2 = b-65;
                    if(b == '\'')
                        index2 = 26;
                    if(b == '.')
                        index2 = 27;
                    //if not found
                    if(theNode->children[index2] == nullptr)
                    {
                        check2 = false;
                        break;
                    }
                    else
                        theNode = theNode->children[index2];
                }
                
                if(check2)
                {
                    if(suggestions.size() < max_suggestions)
                    {
                    temp = temp.substr(0,temp.size()-1);
                    transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
                    suggestions.push_back(temp);
                    temp = word;
                    }
                }
            }
        }
        }
        return check;
        
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems)
{
    std::vector<SpellCheck::Position>:: iterator it=problems.begin();
    while(it!=problems.end())
      it=problems.erase(it);
    std::vector<std::string> container;
    std::vector<SpellCheck::Position> index;
    int count=0;
    Position m_pos;
    std::string temp;
    for(int i=0;i<line.size();i++) //Loop through the line
    {
        if(isalpha(line[i]))
        {
            if(temp.size()==0)
                count=i;
            temp+=line[i];
        }
        if(line[i]=='\'')
        {
            if(i>0 && i<line.size()-1)
                if(isalpha(line[i-1]) && (isalpha(line[i+1]) || line[i+1]==' '))
                {
                    temp+=line[i];
                    continue;
                }
        }
        if(temp.size()!=0 && !isalpha(line[i]))
        {
            m_pos.start=count;
            m_pos.end=i-1;
            index.push_back(m_pos);
            container.push_back(temp);
            temp="";
        }
        if(i==line.size()-1 && temp.size())
        {
            m_pos.start=count;
            m_pos.end=i;
            index.push_back(m_pos);
            container.push_back(temp);
            temp="";
        }
    }

    for(int j=0;j<container.size();j++)
    {
        std::string m_word=container[j];
        m_word+='.';
        Node* curr=m_trie->helper();
        for(int i=0;i<m_word.size();i++)
        {
            char a=m_word[i];
            int m_index=a-97;
            if(m_index<0)
                m_index=a-65;
            if(a=='\'')
               m_index=26;
            if(a=='.')
                m_index=27;
            if(curr->children[m_index]==nullptr) //Nothing is found
            {
                problems.push_back(index[j]);
                break;
            }
            else
                curr=curr->children[m_index];
        }
    }
}
