#include "../AllCommands.h"

TwoBallHotAuto::TwoBallHotAuto() {
	AddParallel(new FrontPickupSetMode(FrontPickup::fAutoLoad));		// Store ball and move to Low Deploy
	AddSequential(new BackPickupSetMode(BackPickup::bDeploy));			// Pickup ball behind robot
	
	AddSequential(new BackPickupWaitForMode(BackPickup::bStore));		// Wait until ball is loaded
	
	AddSequential(new FindHotTarget());									// Find Hot Target

	AddParallel(new TwoBallHotDrive());
	AddSequential(new BallShooterWaitForMode(BallShooter::sLoad));
	
	AddSequential(new FrontPickupSetMode(FrontPickup::fLoad));
	
	AddSequential(new FrontPickupWaitForMode(FrontPickup::fStore));
	
	AddSequential(new BallShooterFire());
	
	AddSequential(new DriveStopMotors());
}
