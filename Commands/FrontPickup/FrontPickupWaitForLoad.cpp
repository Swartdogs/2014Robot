#include "../AllCommands.h"

FrontPickupWaitForLoad::FrontPickupWaitForLoad() {
	Requires(frontPickup);
}

void FrontPickupWaitForLoad::Initialize() {
}

void FrontPickupWaitForLoad::Execute() {
}

bool FrontPickupWaitForLoad::IsFinished() {
	if (frontPickup->GetFrontPickupMode() == FrontPickup::fLoad) return false;
	if (frontPickup->GetFrontPickupMode() == FrontPickup::fDropInShooter) return false;
	return true;
}

void FrontPickupWaitForLoad::End() {
}

void FrontPickupWaitForLoad::Interrupted() {
}
