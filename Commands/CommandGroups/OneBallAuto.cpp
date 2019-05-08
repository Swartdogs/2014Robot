#include "../AllCommands.h"

OneBallAuto::OneBallAuto(double timeout) {
	AddParallel(new FrontPickupSetMode(FrontPickup::fShoot));
	AddSequential(new WaitCommand(timeout));

	AddSequential(new BallShooterWaitForMode(BallShooter::sReady));
	
	AddSequential(new FindHotTarget());
	
	AddSequential(new CheckForHot());
	
	AddSequential(new OneBallHotDrive());
	
	AddSequential(new DriveStopMotors());
}
