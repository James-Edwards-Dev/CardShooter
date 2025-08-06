// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreGameMode.h"
#include "CorePlayerState.h"

void ACoreGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (ACorePlayerState* NewPlayerState = Cast<ACorePlayerState>(NewPlayer->PlayerState))
	{
		AssignTeamToPlayer(NewPlayerState);
	}
}

void ACoreGameMode::AssignTeamToPlayer(ACorePlayerState* PlayerState)
{
	if (TeamCount <= 1)
	{
		PlayerState->SetTeam(ETeam::Neutral);
	}
	else
	{
		uint8 TeamIndex = FMath::RandRange(1, TeamCount);
		ETeam NewTeam = static_cast<ETeam>(TeamIndex);

		PlayerState->SetTeam(NewTeam);
	}
}
