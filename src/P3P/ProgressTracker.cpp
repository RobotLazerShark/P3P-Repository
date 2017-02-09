#include "P3P/ProgressTracker.hpp"
#include <P3P/Level.hpp>


ProgressTracker::ProgressTracker()
{
	boxSpots = std::vector <BoxSpot*> (0);
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

    return win;
}
