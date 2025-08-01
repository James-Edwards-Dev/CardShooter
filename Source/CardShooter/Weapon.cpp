// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "MeshPaintVisualize.h"
#include "PlayerCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
	bReplicates = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
	BaseMesh->SetupAttachment(RootComponent);
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
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Bang");
}

void AWeapon::StartPrimaryFire_Implementation()
{
	// Check if fire is on cooldown
	const FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (!TimerManager.TimerExists(PrimaryFireCooldownHandle) || !TimerManager.IsTimerActive(PrimaryFireCooldownHandle))
	{
		PrimaryFire();

		if (FiringType == EFiringType::FullAuto)
		{
			
		}	

	}
}

void AWeapon::EndPrimaryFire_Implementation()
{
	bPrimaryFireHeld = false;
}



