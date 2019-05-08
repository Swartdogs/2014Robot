#include "../AllCommands.h"

DriveStopMotors::DriveStopMotors() {
	Requires(drive);
}

// Called just before this Command runs the first time
void DriveStopMotors::Initialize() {
	CommandBase::robotLog->LogWrite("Command: Drive StopMotors");
}

// Called repeatedly when this Command is scheduled to run
void DriveStopMotors::Execute() {
	drive->StopMotors();
}

// Make this return true when this Command no longer needs to run execute()
bool DriveStopMotors::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void DriveStopMotors::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveStopMotors::Interrupted() {
}
