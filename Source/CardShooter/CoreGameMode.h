// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CorePlayerState.h"
#include "GameFramework/GameMode.h"
#include "CoreGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CARDSHOOTER_API ACoreGameMode : public AGameMode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameSettings, meta = (ClampMin = "1", ClampMax = "4", AllowPrivateAccess = "true"))
	uint8 NumberOfTeams = 2;
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	virtual void AssignTeamToPlayer(ACorePlayerState* PlayerState);
	TArray<ETeam> GetSmallestTeams() const;
};
