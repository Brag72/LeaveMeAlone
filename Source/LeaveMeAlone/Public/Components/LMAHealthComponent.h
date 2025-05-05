// LeaveMeAlone Game by Netologiya. All RightsReserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAHealthComponent.generated.h"

class UDamageType;
class AController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAVEMEALONE_API ULMAHealthComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	float Health = 0.0f;

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser);

	
public:	
	// Sets default values for this component's properties
	ULMAHealthComponent();

	UFUNCTION(BlueprintCallable)
	float GetHealth() const {return Health;};

	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth = 100.0f;

	bool IsHealthFull() const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;
	
	FOnHealthChanged OnHealthChanged;
	
	bool AddHealth(float NewHealth);
};
