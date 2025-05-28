// LeaveMeAlone Game by Netologiya. All RightsReserved


#include "LMA_GrenadeBase.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ALMA_GrenadeBase::ALMA_GrenadeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALMA_GrenadeBase::BeginPlay()
{
	Super::BeginPlay();
}

void ALMA_GrenadeBase::Explosive()
{
	UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageExplosive, GetActorLocation(), ExplosiveRadius, nullptr,{this}, this, nullptr, true);
}

// Called every frame
void ALMA_GrenadeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}