// LeaveMeAlone Game by Netologiya. All RightsReserved


#include "LeaveMeAlone/Public/LevelActors/Pickup/LMAHealthPickup.h"

#include "LMADefaultCharacter.h"
#include "Components/LMAHealthComponent.h"
#include "Components/SphereComponent.h"

bool ALMAHealthPickup::GivePickup(ALMADefaultCharacter* Character)
{
	const auto HealthComponent = Character->GetHealthComponent();
	if (!HealthComponent) return false;

	return HealthComponent->AddHealth(HealthFromPickup);
}

void ALMAHealthPickup::PickupWasTaken()
{
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetRootComponent()->SetVisibility(false,true);

	FTimerHandle RespawnTimerHandle;
	GetWorldTimerManager().SetTimer(RespawnTimerHandle,this, &ALMAHealthPickup::RespawnPickup,RespawnTime);
}

void ALMAHealthPickup::RespawnPickup()
{
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	GetRootComponent()->SetVisibility(true,true);
}

// Sets default values
ALMAHealthPickup::ALMAHealthPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	SetRootComponent(SphereComponent);

}

// Called when the game starts or when spawned
void ALMAHealthPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALMAHealthPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	const auto Character = Cast<ALMADefaultCharacter>(OtherActor);
	if (GivePickup(Character))
	{
		PickupWasTaken();
	}
}

// Called every frame
void ALMAHealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

