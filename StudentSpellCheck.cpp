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
    /*
    Node* curr=m_trie->helper();
    int count=0;
    for(int i=0;i<=27;i++)
    {
        curr=curr->children[i];
        if(curr->letter!=word[i])
            count++;
    }
	return false;
     */
    return false;
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
    for(int i=0;i<line.size();i++) //Loop throught the line
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
