// LeaveMeAlone Game by Netologiya. All RightsReserved


#include "LevelActors/Pickup/GrenadePickUPs.h"
#include "LMADefaultCharacter.h"

// Sets default values
AGrenadePickUPs::AGrenadePickUPs()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGrenadePickUPs::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AGrenadePickUPs::GivePickup(ALMADefaultCharacter* Character)
{
	if (!Character) return false;
	const auto WeaponComponent = Character->GetWeaponComponent();
	if (!WeaponComponent) return false;

	WeaponComponent->EquippedGrenades = CountGrenade;
	return Super::GivePickup(Character);
}

void AGrenadePickUPs::PickupWasTaken()
{
	Super::PickupWasTaken();

	Destroy();
}

void AGrenadePickUPs::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	const auto Character = Cast<ALMADefaultCharacter>(OtherActor);
	if (Character && GivePickup(Character))
	{
		PickupWasTaken();
	}
}

// Called every frame
void AGrenadePickUPs::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

