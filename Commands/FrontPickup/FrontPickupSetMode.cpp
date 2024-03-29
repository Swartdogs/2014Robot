#include "../AllCommands.h"

FrontPickupSetMode::FrontPickupSetMode(FrontPickup::FrontMode mode) {
	Requires(frontPickup);
	
	m_pickupMode = mode;
}

// Called just before this Command runs the first time
void FrontPickupSetMode::Initialize() {
	CommandBase::robotLog->LogWrite("Command: FrontPickup SetMode");
	frontPickup->SetPickupMode(m_pickupMode);
}

// Called repeatedly when this Command is scheduled to run
void FrontPickupSetMode::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool FrontPickupSetMode::IsFinished() {
	return true;
}

// Called once after isFinished returns true
void FrontPickupSetMode::End() {
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void FrontPickupSetMode::Interrupted() {
}
