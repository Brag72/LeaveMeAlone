// LeaveMeAlone Game by Netologiya. All RightsReserved


#include "LevelActors/Pickup/PickUpBase.h"

#include "Components/SphereComponent.h"

// Sets default values
APickUpBase::APickUpBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	SetRootComponent(SphereComponent);
}

// Called when the game starts or when spawned
void APickUpBase::BeginPlay()
{
	Super::BeginPlay();
}

bool APickUpBase::GivePickup(ALMADefaultCharacter* Character)
{
	return true;
}

void APickUpBase::PickupWasTaken(){}

void APickUpBase::RespawnPickup(){}

void APickUpBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
}

// Called every frame
void APickUpBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

