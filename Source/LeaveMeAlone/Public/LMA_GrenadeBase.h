// LeaveMeAlone Game by Netologiya. All RightsReserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMA_GrenadeBase.generated.h"


UCLASS()
class LEAVEMEALONE_API ALMA_GrenadeBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALMA_GrenadeBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Explosive();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive")
	float ExplosiveRadius = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive")
	float DamageExplosive = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive")
	float TimeToExplode = 2.0f;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float ScaleImpulse = 0.0f;
};
