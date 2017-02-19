#ifndef PROGRESSTRACKER_H
#define PROGRESSTRACKER_H

#include <vector>
#include "P3P/objects/base objects/BoxSpot.hpp"
#include <P3P/objects/Door.hpp>

class ProgressTracker
{
    public:
        ProgressTracker();
	~ProgressTracker();
        std::vector <BoxSpot*> boxSpots;
	std::vector <Door*> doors;

        bool checkWin();
    protected:

    private:
};

#endif // PROGRESSTRACKER_H