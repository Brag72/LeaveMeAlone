// LeaveMeAlone Game by Netologiya. All RightsReserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon/LMAWeaponComponent.h"
#include "LMADefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ULMAHealthComponent;
class UAnimMontage;

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()
private:
	float YRotation = -75.0f;
	float FOV = 55.0f;
	float TotalTime = 0.0f;
	bool bZoomInNotOut = true;
	
public:
	// Sets default values for this character's properties
	ALMADefaultCharacter();

private:
	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION()
	void ZoomCamera();
	
	void OnZoomIn();
	void OnZoomOut();
	float ScrollTime = 0.1f;

	void OnDeath();
	void RotationPlayerOnCursor();

	UFUNCTION(BlueprintCallable)
	void Sprint();
	
	void Walk();
	void ChangeEndurance(bool bRestore, float DeltaTime);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnHealthChanged(float NewHealth);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MaxCameraSpeed = 2.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MaxClampArmLength = 1400.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MinClampArmLength = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float InterpolationTime = 2.0f;
	
	FTimerHandle ScrollTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ComponentsHealth")
	ULMAHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Sprint")
	float SprintSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Sprint")
	float Endurance = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Sprint")
	float WalkSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Sprint")
	bool bSprintPressed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	ULMAWeaponComponent* WeaponComponent;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const {return HealthComponent;};

	bool OnSprint();
};
