#include "../AllCommands.h"

BallShooterResetHasBall::BallShooterResetHasBall() {
	Requires(ballShooter);
}

// Called just before this Command runs the first time
void BallShooterResetHasBall::Initialize() {
	ballShooter->ResetHasBall();
}

// Called repeatedly when this Command is scheduled to run
void BallShooterResetHasBall::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool BallShooterResetHasBall::IsFinished() {
	return true;
}

// Called once after isFinished returns true
void BallShooterResetHasBall::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BallShooterResetHasBall::Interrupted() {
}
