#ifndef PROGRESSTRACKER_H
#define PROGRESSTRACKER_H

#include <vector>
#include "P3P/objects/base objects/BoxSpot.hpp"
#include <P3P/objects/Door.hpp>
#include <P3P/objects/base objects/Mirror.hpp>

class ProgressTracker
{
    public:
        ProgressTracker();
		~ProgressTracker();
        std::vector <BoxSpot*> boxSpots;
		std::vector <Door*> doors;
		Mirror * _targetMirror = nullptr;
        bool checkWin();
    protected:

    private:
};

#endif // PROGRESSTRACKER_H