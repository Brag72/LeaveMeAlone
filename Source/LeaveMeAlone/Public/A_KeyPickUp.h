// LeaveMeAlone Game by Netologiya. All RightsReserved

#pragma once

#include "CoreMinimal.h"
#include "LevelActors/Pickup/PickUpBase.h"
#include "A_KeyPickUp.generated.h"

class ALMADefaultCharacter;

UCLASS()
class LEAVEMEALONE_API AA_KeyPickUp : public APickUpBase
{
	GENERATED_BODY()
public:
	AA_KeyPickUp();
	
protected:
	virtual void BeginPlay() override;
	virtual bool GivePickup(ALMADefaultCharacter* Character) override;
	virtual void PickupWasTaken() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 KeyCount = 1;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};