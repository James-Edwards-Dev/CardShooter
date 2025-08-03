// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EFiringType : uint8
{
	FullAuto UMETA(DisplayName = "Full Auto"),
	SemiAuto UMETA(DisplayName = "Semi Auto")
};

UCLASS()
class CARDSHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* Muzzle;
	
	// Sets default values for this actor's properties
	AWeapon();

protected:

	// Weapon Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float FireCoolDown = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EFiringType FiringType = EFiringType::FullAuto;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	UParticleSystem* MuzzleFlash;

private:
	FTimerHandle PrimaryFireCooldownHandle;

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	bool bPrimaryFireHeld = false;

public:
	UFUNCTION(Server, Reliable)
	void StartPrimaryFire();

	UFUNCTION(Server, Reliable)
	void EndPrimaryFire();
	
	UFUNCTION(NetMulticast, Reliable)
	void DisplayPrimaryFireEffects();

private:
	void PrimaryFire();

	void PrimaryFireCoolDownElapsed();
};
