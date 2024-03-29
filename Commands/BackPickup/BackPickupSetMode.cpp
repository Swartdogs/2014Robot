#include "../AllCommands.h"

BackPickupSetMode::BackPickupSetMode(BackPickup::BackMode mode) {
	Requires(backPickup);
	m_pickupMode = mode;
}

// Called just before this Command runs the first time
void BackPickupSetMode::Initialize() {
	CommandBase::robotLog->LogWrite("Command: BackPickup SetMode");
	backPickup->SetPickupMode(m_pickupMode);
}

// Called repeatedly when this Command is scheduled to run
void BackPickupSetMode::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool BackPickupSetMode::IsFinished() {
	return m_pickupMode != BackPickup::bManualPass;
}

// Called once after isFinished returns true
void BackPickupSetMode::End() {
	if(m_pickupMode == BackPickup::bManualPass) backPickup->SetPickupMode(BackPickup::bStore);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BackPickupSetMode::Interrupted() {
	if(m_pickupMode == BackPickup::bManualPass) backPickup->SetPickupMode(BackPickup::bStore);
}
