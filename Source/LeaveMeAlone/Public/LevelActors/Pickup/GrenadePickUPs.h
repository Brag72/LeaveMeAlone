// LeaveMeAlone Game by Netologiya. All RightsReserved

#pragma once

#include "CoreMinimal.h"
#include "LevelActors/Pickup/PickUpBase.h"
#include "GrenadePickUPs.generated.h"

class ALMADefaultCharacter;

UCLASS()
class LEAVEMEALONE_API AGrenadePickUPs : public APickUpBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrenadePickUPs();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual bool GivePickup(ALMADefaultCharacter* Character) override;
	virtual void PickupWasTaken() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grenade")
	int32 CountGrenade = 1;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
