// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreGameMode.h"
#include "CorePlayerState.h"
#include "GameFramework/GameStateBase.h"

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
	if (NumberOfTeams <= 1)
	{
		PlayerState->SetTeam(ETeam::Neutral);
	}
	else
	{
		TArray<ETeam> SelectableTeams = GetSmallestTeams();
		ETeam NewTeam = SelectableTeams[FMath::RandRange(0, SelectableTeams.Num() -1)];

		PlayerState->SetTeam(NewTeam);
	}
}

TArray<ETeam> ACoreGameMode::GetSmallestTeams() const
{
	// Count Players Per Team
	TArray<int32> PlayersPerTeam;
	PlayersPerTeam.Init(0, NumberOfTeams);
	for (APlayerState* PlayerStates : GameState->PlayerArray)
	{
		if (ACorePlayerState* CorePlayerStates = Cast<ACorePlayerState>(PlayerStates))
		{
			uint8 TeamIndex = static_cast<uint8>(CorePlayerStates->GetTeam());
			if (TeamIndex > 0)
			{
				PlayersPerTeam[TeamIndex - 1] += 1;
			}
		}
	}

	// Get Min Team Size
	int32 MinTeamSize = MAX_int32;
	for (int32 i = 0; i < PlayersPerTeam.Num(); ++i)
	{
		if (MinTeamSize > PlayersPerTeam[i])
		{
			MinTeamSize = PlayersPerTeam[i];
		}
	}

	// Get Teams with Min Size
	TArray<ETeam> SmallestTeams;
	for (int32 i = 0; i < PlayersPerTeam.Num(); ++i)
	{
		if (PlayersPerTeam[i] == MinTeamSize)
		{
			SmallestTeams.Add(static_cast<ETeam>(i + 1));
		}
	}
	return SmallestTeams;
}
