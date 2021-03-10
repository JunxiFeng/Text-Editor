#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch)
{
     if(action==DELETE) //Batching: Delete/Join
     {
         
        if(!m_stack.empty())
        {//Delete Key case:fn+delete
        if(m_stack.top().m_action==DELETE && m_stack.top().m_row==row && m_stack.top().m_col==col)
        {
            m_stack.top().addstring(ch);//Add string to previous or current
            return;
        }
         }
     }
    std::string a;
    a+=ch;
    op=new operation(action, row, col, a);
    m_stack.push(*op);
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text)
{
    if(m_stack.empty())
    return Action::ERROR;
    if(m_stack.top().m_action==INSERT)
    {  //Store the top of the stakc, then pop it to check
        int temp_col=m_stack.top().m_col;
        int temp_row=m_stack.top().m_row;
        std::string temp_s=m_stack.top().ch;
        for(;;) //While it's not done
        { //Batching
            m_stack.pop();
            if(m_stack.empty()) //Cant do the following when empty
                break;
            if(temp_col-1== m_stack.top().m_col)
            {
                temp_s+=m_stack.top().ch;
                temp_row=m_stack.top().m_row;
                temp_col=m_stack.top().m_col;
                continue;
            }
            else
                break;
        }
        count=(int)temp_s.size();
        row=temp_row;
        col=temp_col;
        text="";
        return Action::DELETE;
    }
    if(m_stack.top().m_action==DELETE)
    {
        int temp_col=m_stack.top().m_col;
        int secret=temp_col;
        int temp_row=m_stack.top().m_row;
        std::string temp_s=m_stack.top().ch;
        for(;;) //While it's not done
        {//Batching
            m_stack.pop();
            if(m_stack.empty())
                break;
            if(temp_col+1== m_stack.top().m_col)
            {
                temp_s+=m_stack.top().ch;
                temp_row=m_stack.top().m_row;
                temp_col=m_stack.top().m_col;
                continue;
            }
            else
                break;
        }
        count=1;
        row=temp_row;
        col=secret;
        text+=temp_s;
        return Action::INSERT;
    }
    if(m_stack.top().m_action==SPLIT)
    {
        count=1;
        row=m_stack.top().m_row;
        col=m_stack.top().m_col;
        text="";
        if(!m_stack.empty())
            m_stack.pop();
        return Action::JOIN;
    }
    else
    {
        count=1;
        row=m_stack.top().m_row; 
        col=m_stack.top().m_col;
        text="";
        if(!m_stack.empty())
        m_stack.pop();
        return Action::SPLIT;
    }
}

void StudentUndo::clear()
{
    delete op;
	while(!m_stack.empty())
    {
        m_stack.pop();
    }
}
