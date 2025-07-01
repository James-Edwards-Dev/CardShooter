// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class CARDSHOOTER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

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
	
	// Server + Multicast RPCs
	UFUNCTION(Server, Reliable)
	void Server_SetMoveInput(FVector2D Input);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetMoveInput(FVector2D Input);

	UFUNCTION(Server, Reliable)
	void Server_SetAiming(bool bNewIsAiming);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetAiming(bool bNewIsAiming);

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

	UPROPERTY(BlueprintReadOnly, Category = Player_Input)
	bool bIsPrimaryFiring;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
};
