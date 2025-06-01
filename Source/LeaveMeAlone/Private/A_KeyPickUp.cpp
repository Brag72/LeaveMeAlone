// LeaveMeAlone Game by Netologiya. All RightsReserved


#include "A_KeyPickUp.h"

#include "LMADefaultCharacter.h"

AA_KeyPickUp::AA_KeyPickUp()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AA_KeyPickUp::BeginPlay()
{
	Super::BeginPlay();
}

bool AA_KeyPickUp::GivePickup(ALMADefaultCharacter* Character)
{
	if (!Character) return false;

	Character->EquippedKey = KeyCount;
	
	return Super::GivePickup(Character);
}

void AA_KeyPickUp::PickupWasTaken()
{
	Super::PickupWasTaken();
	
	Destroy();
}

void AA_KeyPickUp::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	const auto Character = Cast<ALMADefaultCharacter>(OtherActor);
	if (Character && GivePickup(Character))
	{
		PickupWasTaken();
	}
}

void AA_KeyPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}