#include "../AllCommands.h"

BackPickupWaitForLoad::BackPickupWaitForLoad() {
	Requires(backPickup);
}

void BackPickupWaitForLoad::Initialize() {
	CommandBase::robotLog->LogWrite("Command: BackPickup WaitForLoad");
	SetTimeout(3.0);
}

void BackPickupWaitForLoad::Execute() {
}

bool BackPickupWaitForLoad::IsFinished() {
	if (backPickup->GetBackPickupMode() == BackPickup::bStore ) return true;
	return IsTimedOut();
}

void BackPickupWaitForLoad::End() {
	if (backPickup->GetBackPickupMode() != BackPickup::bStore) {
		backPickup->SetPickupMode(BackPickup::bStore);
		frontPickup->SetPickupMode(FrontPickup::fLoad);
	}
}

void BackPickupWaitForLoad::Interrupted() {
}
