// LeaveMeAlone Game by Netologiya. All RightsReserved


#include "Weapon/LMAWeaponComponent.h"

#include "Animations/LMAReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.h"

// Sets default values for this component's properties
ULMAWeaponComponent::ULMAWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
	InitAnimNotify();

	Weapon->AmmoEmpty.AddUObject(this, &ULMAWeaponComponent::Reload);
}


// Called every frame
void ULMAWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULMAWeaponComponent::SpawnWeapon()
{
	Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
	if (Weapon)
	{
		const auto Character = Cast<ACharacter>(GetOwner());
		if (Character)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(),AttachmentRules, "r_Weapon_Socket");
		}
	}
}

void ULMAWeaponComponent::Fire()
{
	if (!OnFireHandle.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(OnFireHandle, this, &ULMAWeaponComponent::OnFire, 0.1f, true);		
	}
	OnFire();
}

void ULMAWeaponComponent::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(OnFireHandle);
}

void ULMAWeaponComponent::OnFire()
{
	if (Weapon && !AnimReloading)
	{
		Weapon->Fire();
	}
}

void ULMAWeaponComponent::Reload()
{
	ALMADefaultCharacter* DefChar = Cast<ALMADefaultCharacter>(GetOwner());
	if (!CanReload() || Weapon->CurrentAmmoWeapon.Bullets >= 30 || DefChar->OnSprint())return;
	
	Weapon->ChangeClip();
	AnimReloading = true;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	Character->PlayAnimMontage(ReloadMontage);
}

void ULMAWeaponComponent::InitAnimNotify()
{
	if (!ReloadMontage) return;

	const auto NotifiesEvents = ReloadMontage->Notifies;
	for (auto NotifyEvent : NotifiesEvents)
	{
		auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
		if (ReloadFinish)
		{
			ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished);
			break;
		}
	}
}

void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh)
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (Character->GetMesh() == SkeletalMesh)
	{
		AnimReloading = false;
	}
}

bool ULMAWeaponComponent::CanReload() const
{
	return !AnimReloading;
}