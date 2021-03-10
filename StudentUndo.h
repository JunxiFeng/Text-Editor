#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"
#include <stack>
class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
    struct operation
    {
        operation(Action theaction,int x,int y, std::string letter)
        {
            m_row=x;
            m_col=y;
            ch=letter;
            m_action=theaction;
        }
        void addstring(char s)
        {
            ch=ch+s;
        }
        int m_row, m_col;
        std::string ch;
        Action m_action;
    };
    std::stack<operation> m_stack;
    operation* op;
};

#endif // STUDENTUNDO_H_
