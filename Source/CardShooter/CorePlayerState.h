// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TeamTypes.h"
#include "CorePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CARDSHOOTER_API ACorePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	void SetTeam(ETeam NewTeam);

protected:
	ETeam Team;
};
