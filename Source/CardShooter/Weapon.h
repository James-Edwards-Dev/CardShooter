// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EFiringType : uint8
{
	SemiAuto UMETA(DisplayName = "Semi Auto"),
	FullAuto UMETA(DisplayName = "Full Auto")
};

UCLASS()
class CARDSHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BaseMesh;
	
	// Sets default values for this actor's properties
	AWeapon();

protected:

	// Weapon Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EFiringType FiringType;

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

private:
	void PrimaryFire();
};
