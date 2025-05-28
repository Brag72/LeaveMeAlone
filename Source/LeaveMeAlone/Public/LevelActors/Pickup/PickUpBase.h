// LeaveMeAlone Game by Netologiya. All RightsReserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpBase.generated.h"

class USphereComponent;
class ALMADefaultCharacter;

UCLASS()
class LEAVEMEALONE_API APickUpBase : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APickUpBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float RespawnTime = 5.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	USphereComponent* SphereComponent;

	virtual bool GivePickup(ALMADefaultCharacter* Character);
	virtual void PickupWasTaken();
	virtual void RespawnPickup();
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
