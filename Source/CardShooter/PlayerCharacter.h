// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Weapon.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class CARDSHOOTER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* FirstPersonMesh;

	// Enhanced Input 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryFireAction;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Weapon
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<AWeapon> WeaponClass;

	 UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
	AWeapon* CurrentWeapon;
	
	// Server + Multicast RPCs
	UFUNCTION(Server, Reliable)
	void Server_SetMoveInput(FVector2D Input);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetMoveInput(FVector2D Input);

	UFUNCTION(Server, Reliable)
	void Server_SetAiming(bool bNewIsAiming);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetAiming(bool bNewIsAiming);


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void NotifyControllerChanged() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Input Variables, Used in blueprint for animations
	UPROPERTY(BlueprintReadOnly, Category = Player_Input)
	FVector2D MoveInput;

	UPROPERTY(BlueprintReadOnly, Category = Player_Input)
	bool bIsAiming;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	// Aiming Line Trace
	FHitResult GetPlayerAimHitByChannel(float TraceDistance = 100000.0f, ECollisionChannel Channel = ECC_GameTraceChannel1);
	
public:
	// Called for Movement Input
	void Move(const FInputActionValue& Value);
	// Called for looking Input
	void Look(const FInputActionValue& Value);
	// Added for aiming and unaiming
	void Aim(const FInputActionValue& Value);
	// Added for Shotting/Alternative Primary Fire Actions
	void Start_PrimaryFire();
	void While_PrimaryFire();
	void Stop_PrimaryFire();

protected:

	void AttachWeaponToPlayer(AWeapon* Weapon);
	
	UFUNCTION(Server, Reliable)
	void EquipWeapon(TSubclassOf<AWeapon> Weapon);

	UFUNCTION()
	void OnRep_CurrentWeapon();
};
