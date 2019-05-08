#include "../AllCommands.h"

TwoBallRightAuto::TwoBallRightAuto() {
	AddParallel(new FrontPickupSetMode(FrontPickup::fAutoLoad));		// Store ball and move to Low Deploy
	AddSequential(new BackPickupSetMode(BackPickup::bDeploy));			// Pickup ball behind robot
	
//	AddSequential(new BackPickupWaitForMode(BackPickup::bStore));		// Wait until ball is loaded
	
	AddSequential(new BackPickupWaitForLoad());
	
	AddParallel(new DriveDistance(153, 0.8, true, 72, 23));				// Start drive sequence that shoots ball
	AddSequential(new BallShooterWaitForMode(BallShooter::sLoad));		// Wait until shooter is moving to load position
	
	AddSequential(new FrontPickupSetMode(FrontPickup::fLoad));			// Load front ball
	
	AddSequential(new FrontPickupWaitForMode(FrontPickup::fStore));		// Wait until front pickup is out of the way
	
	AddSequential(new BallShooterFire());								// Shoot ball

	AddSequential(new DriveStopMotors());
}
