// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>("First Person Mesh");
	FirstPersonMesh->SetupAttachment(RootComponent);

	// Hides the 3rd person mesh for the owner and shows the 1st person mesh only to the owner
	FirstPersonMesh->bOnlyOwnerSee = true;
	GetMesh()->bOwnerNoSee = true;
}

void APlayerCharacter::Server_SetMoveInput_Implementation(FVector2D Input)
{
	Multicast_SetMoveInput(Input);
}

void APlayerCharacter::Multicast_SetMoveInput_Implementation(FVector2D Input)
{
	if (!IsLocallyControlled())
	{
		MoveInput = Input;
	}
}

void APlayerCharacter::Server_SetAiming_Implementation(bool bNewIsAiming)
{
	Multicast_SetAiming(bNewIsAiming);
}

void APlayerCharacter::Multicast_SetAiming_Implementation(bool bNewIsAiming)
{
	if (!IsLocallyControlled())
	{
		bIsAiming = bNewIsAiming;
	}
}

void APlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::Jump);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerCharacter::Move);
		
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		// Aiming
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Aim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &APlayerCharacter::Aim);

		// Shooting/Firing
		EnhancedInputComponent->BindAction(PrimaryFireAction, ETriggerEvent::Started, this, &APlayerCharacter::Start_PrimaryFire);
		EnhancedInputComponent->BindAction(PrimaryFireAction, ETriggerEvent::Triggered, this, &APlayerCharacter::While_PrimaryFire);
		EnhancedInputComponent->BindAction(PrimaryFireAction, ETriggerEvent::Completed, this, &APlayerCharacter::Stop_PrimaryFire);
	}
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FHitResult APlayerCharacter::GetPlayerAimHitByChannel(float TraceDistance, ECollisionChannel Channel)
{
	FHitResult HitResult;

	// Set Start and End Location
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (GetControlRotation().Vector() * TraceDistance);

	// Set Parameters
	FCollisionQueryParams Params;
	// Params.bTraceComplex = true;
	Params.AddIgnoredActor(this);

	// Line Trace
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, Channel, Params);

	// Ray Tracing Debugging
	if (HitResult.bBlockingHit)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Player Hit: " + HitResult.GetComponent()->GetName());

		DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Orange, false, 2.0f);
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 2.0f);
	}

	return HitResult;
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	MoveInput = Value.Get<FVector2D>();

	if (IsLocallyControlled())
	{
		Server_SetMoveInput(MoveInput);
	}

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.Y);
		AddMovementInput(GetActorRightVector(), MoveInput.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::Aim(const FInputActionValue& Value)
{
	bIsAiming = Value.Get<bool>();

	if (IsLocallyControlled())
	{
		Server_SetAiming(bIsAiming);
	}
}

void APlayerCharacter::Start_PrimaryFire()
{
	bIsPrimaryFiring = true;

	// Line trace to check what player has fired at
	FHitResult FireResults = GetPlayerAimHitByChannel();
	// Check if fire results is a player
	if (FireResults.GetActor() != nullptr && FireResults.GetActor()->IsA(APlayerCharacter::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Player Class Hit");
	}
}

void APlayerCharacter::While_PrimaryFire()
{
	// GEngine->AddOnScreenDebugMessage(-1, 0.1, FColor::Red, "Firing Currently");
}

void APlayerCharacter::Stop_PrimaryFire()
{
	bIsPrimaryFiring = false;

	// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Stopped Firing");
}
