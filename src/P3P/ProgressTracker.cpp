#include "P3P/ProgressTracker.hpp"
#include <P3P/Level.hpp>


ProgressTracker::ProgressTracker()
{
}
ProgressTracker::~ProgressTracker()
{
	boxSpots.clear ();
}

bool ProgressTracker::checkWin()
{
    bool win = true;

    for (int i = 0, size = boxSpots.size (); i < size; i ++)
    {
        if (!boxSpots [i]->taken)
        {
            win = false;
        }
    }
	if (boxSpots.size() == 0)
	{
		win = false;
	}

    if (win)
    {
		if (_targetMirror != nullptr)
		{
			_targetMirror->setActive(true);
		}
		for (int i = 0, size = doors.size (); i < size; i ++)
		{
			doors [i]->open ();
		}
    }
    return win;
}
