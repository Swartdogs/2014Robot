#include "../AllCommands.h"

ZeroBallAuto::ZeroBallAuto(double timeout, double distance) {
	
    AddSequential(new WaitCommand(timeout));
	AddSequential(new DriveDistance(distance, 0.7, true, 0.0));
	
}
