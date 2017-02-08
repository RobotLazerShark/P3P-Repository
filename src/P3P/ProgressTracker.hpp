#ifndef PROGRESSTRACKER_H
#define PROGRESSTRACKER_H

#include "P3P/objects/base objects/BoxSpot.hpp"
class ProgressTracker
{
    public:
        ProgressTracker();
        std::vector <BoxSpot*> boxSpots;

        void checkWin();
    protected:

    private:
};

#endif // PROGRESSTRACKER_H
