// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AWeapon::AWeapon()
{
	bReplicates = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
	BaseMesh->SetupAttachment(RootComponent);

	Muzzle = CreateDefaultSubobject<UArrowComponent>("Muzzle");
	Muzzle->SetupAttachment(BaseMesh);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::PrimaryFire()
{
	// Start Fire Cooldown
	GetWorld()->GetTimerManager().SetTimer(
		PrimaryFireCooldownHandle,
		this,
		&AWeapon::PrimaryFireCoolDownElapsed,
		FireCoolDown,
		false);

	APlayerCharacter* PlayerCharacter = GetOwner<APlayerCharacter>();
	// RayCast
	FHitResult HitResult = PlayerCharacter->GetAimHitResult(true);
	
	UGameplayStatics::ApplyDamage(
		HitResult.GetActor(),
		10,
		PlayerCharacter->GetController(),
		PlayerCharacter,
		UDamageType::StaticClass());

	DisplayPrimaryFireEffects();
}

void AWeapon::DisplayPrimaryFireEffects_Implementation()
{
	// Spawn Muzzle Flash
	UParticleSystemComponent* MuzzleFlashComponent = UGameplayStatics::SpawnEmitterAttached(
		MuzzleFlash,
		Muzzle,
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget);
	
	MuzzleFlashComponent->SetWorldScale3D(Muzzle->GetComponentScale());
}

void AWeapon::StartPrimaryFire_Implementation()
{
	bPrimaryFireHeld = true;
	// Check if fire is on cooldown
	const FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (!TimerManager.TimerExists(PrimaryFireCooldownHandle) || !TimerManager.IsTimerActive(PrimaryFireCooldownHandle))
	{
		PrimaryFire();
	}
}

void AWeapon::EndPrimaryFire_Implementation()
{
	bPrimaryFireHeld = false;
}

void AWeapon::PrimaryFireCoolDownElapsed()
{
	if (bPrimaryFireHeld && FiringType == EFiringType::FullAuto)
	{
		PrimaryFire();
	}
}
