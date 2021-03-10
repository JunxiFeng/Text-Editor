#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

TextEditor* createTextEditor(Undo* un)
{
    return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo)
{
    m_row=0;
    m_col=0;
    // TODO
}

StudentTextEditor::~StudentTextEditor()
{
    std::list<std::string>:: iterator it=line.begin();
    while(it!=line.end())
    {
        it=line.erase(it);
    }
}

bool StudentTextEditor::load(std::string file)
{
    std::ifstream infile(file);
    if(!infile)
    {
        return false;
    }
    reset();//Reset
    int i=0;
    std::string s;
    while(getline(infile,s))
    {  //Erase the carriage return character
        s.erase(std::remove(s.begin(),s.end(), '\r'),s.end());
        line.push_back(s); //add the line to my data structure
        i+=1;
    }
    
    m_row=0;
    m_col=0;
    it=line.begin();
    return true;
}

bool StudentTextEditor::save(std::string file)
{
    std::ofstream outfile(file);
    if(!outfile)
    {
        return false;
    }
    std::list<std::string>:: iterator it=line.begin();
    while(it!=line.end())
    {
        outfile<<*it<<"\n";
        it++;
    }
    return true;
}

void StudentTextEditor::reset()
{
    std::list<std::string>:: iterator it=line.begin();
    while(it!=line.end())
    {
        it=line.erase(it);
    }
    m_row=0;
    m_col=0;
    it=line.begin();
}

void StudentTextEditor::move(Dir dir)
{
    if(dir==Dir::UP)
    {
        if(m_row==0)
            return;
        m_row=m_row-1;
        it--;
        if(it->empty())
            m_col=(int)it->size();
    }
    if(dir==Dir::DOWN)
    {
        if(m_row==line.size()-1)
            return;
        m_row=m_row+1;
        it++;
        if(it->empty())
            m_col=(int)it->size();
    }
    if(dir==Dir::LEFT)
    {
        if(m_row==0 && m_col==0)
            return;
        if(m_col==0)
        {
            m_row=m_row-1;
            it--;
            m_col=(int)it->size();
            return;
        }
        m_col=m_col-1;
    }
    if(dir==Dir::RIGHT)
    {
        if(m_row==line.size()-1 && m_col==it->size())
            return;
        if(m_col==it->size())
        {
            m_col=0;
            m_row++;
            it++;
            return;
        }
        m_col++;
    }
    if(dir==Dir::HOME)
        m_col=0;
        
    if(dir==Dir::END)
        m_col=(int)it->size();
}

void StudentTextEditor::del()
{
    std::string del="";
    if(m_row==line.size()-1) //If at the last line
    {
        it=line.end();
        if(m_col==(*it).size()) //At the last position of last line
            return;
        else
        {
            getUndo()->submit(Undo::Action::DELETE, m_row, m_col,(*it)[m_col]);
            (*it).erase(m_col,1); //Middle of last line
            return;
        }
    }

    if(m_col==(*it).size()) //At the end of a line
    {
        it++; //Go to next line
        for(int j=0;j<(*it).size();j++) //Store the next line into del
            del=del+(*it)[j];
        it=line.erase(it); //Erase the next line
        it--; //Go back to the original line
        *it+=del;  //Add the stored line to this line
        getUndo()->submit(Undo::Action::JOIN, m_row, m_col);
        return;
    }
    getUndo()->submit(Undo::Action::DELETE, m_row, m_col,(*it)[m_col]);
    (*it).erase(m_col,1);
}

void StudentTextEditor::backspace()
{
    std::string substring="";
    if(m_row==0 && m_col==0)
        return;

    std::string backspace="";
    if(m_col==0 ||(*it).size()==0) //First character of a line or empty line
    {
        
        backspace=*it;
        it=line.erase(it);
        it--;
        m_col=(int)it->size();
        *it+=backspace;
        m_row--;
        getUndo()->submit(Undo::Action::JOIN, m_row, m_col);
        return;
    }
    
    if(m_col>0) //Run in L time
    {
        m_col-=1;
        del();
    }
    
}

void StudentTextEditor::insert(char ch)
{
    std::string add="";

    if(ch=='\t')
    {
        add="    ";
        (*it).insert(m_col, add);
        getUndo()->submit(Undo::Action::INSERT, m_row, m_col,ch);
        m_col+=4;
    }
    else
    {
        add=add+ch;
        (*it).insert(m_col, add);
        getUndo()->submit(Undo::Action::INSERT, m_row, m_col,ch);
        m_col++;
    }
    
}

void StudentTextEditor::enter()
{
    std::string substring="";
    //Add the character to the substring
    for(int j=m_col;j<(*it).size();j++)
    {
        substring.push_back((*it)[j]);
    }
    
    getUndo()->submit(Undo::Action::SPLIT, m_row, m_col);
    //Erase the character of the line
    for(int i=m_col;i<(*it).size();i++)
        (*it).erase(i);
    m_col=0;
    m_row=m_row+1;
    it++;
    it=line.insert(it, substring);
    
}

void StudentTextEditor::getPos(int& row, int& col) const
{
    row=m_row;
    col=m_col;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const
{
    if(startRow<0 || numRows<0 || startRow>line.size()-1)
        return -1;
    std::vector<std::string>:: iterator m_it=lines.begin();
    for(int i=0;i<lines.size();i++)
        m_it=lines.erase(m_it);
    if(startRow==line.size())
        return 0;
    int temp=m_row;
    std::list<std::string>:: iterator temp_it;//Create a temporary ptr
    temp_it=it;
    while(temp<startRow)
    {
        temp++;
        temp_it++;
    }
    while(temp>startRow)
    {
        temp--;
        temp_it--;
    }
    while(temp<line.size() && temp<numRows)
    {
        lines.push_back(*temp_it);
        temp++;
        temp_it++;
    }
    return (int)lines.size();
}

void StudentTextEditor::undo()
{
    int count=0;
    std::string s="";
    int op_row;
    Undo::Action action=getUndo()->get(op_row, m_col, count, s);
    if(action==Undo::Action::ERROR) //Do nothing when stack is empty
        return;
    if(op_row!=m_row) //Check the cursor position relative to the operation position
    {
    int distance=abs(op_row-m_row);
    for(int i=0;i<distance;i++)
    {
        if(op_row>m_row)
        {//Current cursor is above operation pos
            it++;
            m_row++;
        }
        if(op_row<m_row)
        {//Current cursor is below operation pos
            it--;
            m_row--;
        }
    }
    }
    if(action==Undo::Action::INSERT)
    {
        if(s.size()>1) //If more than one character in the text
        {
            (*it).insert(m_col,s);
            return;
        }
        for(int i=0;i<s.size();i++)
            (*it).insert(m_col, s);
    }
    if(action==Undo::Action::DELETE)
    {
        for(int i=0;i<count;i++)
            (*it).erase(m_col,1);
    }
    if(action==Undo::Action::JOIN)
    {
        std::string del;
        it++; //Go to next line
        for(int j=0;j<(*it).size();j++) //Store the next line into del
            del=del+(*it)[j];
        it=line.erase(it); //Erase the next line
        it--; //Go back to the original line
        *it+=del;  //Add the stored line to this line
    }
    if(action==Undo::Action::SPLIT)
    {
        std::string substring="";
        //Add the character to the substring
        for(int j=m_col;j<(*it).size();j++)
            substring.push_back((*it)[j]);
        //Erase the character of the line
        for(int i=m_col;i<(*it).size();i++)
            (*it).erase(i);
        if(m_col==(*it).size())
            m_col=(int)(*it).size();
        else
            m_col=0;
        it++;
        it=line.insert(it, substring);
        it--;
    }
}
