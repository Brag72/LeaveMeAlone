// LeaveMeAlone Game by Netologiya. All RightsReserved


#include "LMADefaultCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/LMAHealthComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ALMADefaultCharacter::ALMADefaultCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = 1400.0f;
	SpringArmComponent->SetRelativeRotation(FRotator(YRotation,0.0f,0.0f));
	SpringArmComponent->bDoCollisionTest = false;
    SpringArmComponent->bEnableCameraLag = true;
	
	CameraComponent->SetFieldOfView(FOV);
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");

	WeaponComponent = CreateDefaultSubobject<ULMAWeaponComponent>("Weapon");
}

void ALMADefaultCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void ALMADefaultCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ALMADefaultCharacter::ZoomCamera()
{
	TotalTime += GetWorldTimerManager().GetTimerRate(ScrollTimerHandle);
	UE_LOG(LogTemp, Display, TEXT("%f"), GetWorldTimerManager().GetTimerRate(ScrollTimerHandle));
	if (TotalTime >= InterpolationTime)
	{
		GetWorldTimerManager().ClearTimer(ScrollTimerHandle);
		TotalTime = 0.0f;
		return;
	}
	if (bZoomInNotOut)
	{
		if (GetWorldTimerManager().IsTimerActive(ScrollTimerHandle))
		{
			SpringArmComponent->TargetArmLength -= MaxCameraSpeed;
			if (SpringArmComponent->TargetArmLength <= MinClampArmLength)
			{
				GetWorldTimerManager().ClearTimer(ScrollTimerHandle);
				TotalTime = 0.0f;
			}
		}
	}
	else
	{
		if (GetWorldTimerManager().IsTimerActive(ScrollTimerHandle))
		{
			SpringArmComponent->TargetArmLength += MaxCameraSpeed;
			if (SpringArmComponent->TargetArmLength >= MaxClampArmLength)
			{
				GetWorldTimerManager().ClearTimer(ScrollTimerHandle);
				TotalTime = 0.0f;
			}
		}
	}
}

void ALMADefaultCharacter::OnZoomIn()
{
	bZoomInNotOut = true;
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("ZoomCamera"));
	if (!ScrollTimerHandle.IsValid() && SpringArmComponent->TargetArmLength > MinClampArmLength)
	{
		GetWorldTimerManager().SetTimer(ScrollTimerHandle, TimerDel, ScrollTime, true);
	}
}

void ALMADefaultCharacter::OnZoomOut()
{
	bZoomInNotOut = false;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ZoomCamera"));
	if (!ScrollTimerHandle.IsValid() && SpringArmComponent->TargetArmLength < MaxClampArmLength)
	{
		GetWorldTimerManager().SetTimer(ScrollTimerHandle, TimerDelegate, ScrollTime, true);
	}
}

void ALMADefaultCharacter::OnDeath()
{
	CurrentCursor->DestroyRenderState_Concurrent();
	PlayAnimMontage(DeathMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

void ALMADefaultCharacter::RotationPlayerOnCursor()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1,true,ResultHit);
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw,0.0f)));
		if (CurrentCursor)
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}
	}
}

void ALMADefaultCharacter::Sprint()
{
	if (Endurance <= 0.0f)
	{
		Walk();
		return;
	}
	auto CharMoveComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	CharMoveComp->MaxWalkSpeed = SprintSpeed;
	bSprintPressed = true;
}

void ALMADefaultCharacter::Walk()
{
	auto CharMoveComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	CharMoveComp->MaxWalkSpeed = WalkSpeed;
	bSprintPressed = false;
}

void ALMADefaultCharacter::ChangeEndurance(bool bRestore, float DeltaTime)
{
	if (bRestore)
	{
		Endurance = FMath::Clamp(Endurance + 5.0f * DeltaTime, 0.0f, 100.0f);
	}
	else
	{
		Endurance = FMath::Clamp(Endurance - 10.0f * DeltaTime, 0.0f, 100.0f);
	}
}

// Called when the game starts or when spawned
void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}
	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnHealthChanged.AddUObject(this, &ALMADefaultCharacter::OnHealthChanged);
	HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter::OnDeath);
}

void ALMADefaultCharacter::OnHealthChanged(float NewHealth)
{
	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red, FString::Printf(TEXT("Health: %f"), NewHealth));
}

// Called every frame
void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!(HealthComponent->IsDead()))
	{
		RotationPlayerOnCursor();
	}

	if (Endurance > 0.0f && bSprintPressed)
	{
		ChangeEndurance(false, DeltaTime);
		UE_LOG(LogTemp, Display, TEXT("%f"), Endurance);
	}

	if (Endurance < 100.0f && !bSprintPressed)
	{
		ChangeEndurance(true, DeltaTime);
		UE_LOG(LogTemp, Display, TEXT("%f"), Endurance);
	}
	
	if (bSprintPressed)
	{
		Sprint();
	}
}

// Called to bind functionality to input
void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward",this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this, &ALMADefaultCharacter::MoveRight);
	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &ALMADefaultCharacter::OnZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &ALMADefaultCharacter::OnZoomOut);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ALMADefaultCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ALMADefaultCharacter::Walk);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Fire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Reload);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &ULMAWeaponComponent::StopFire);
}

bool ALMADefaultCharacter::OnSprint()
{
	return bSprintPressed && Endurance > 0.0f;
}
