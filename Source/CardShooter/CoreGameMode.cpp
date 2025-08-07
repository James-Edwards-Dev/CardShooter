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
	if (TeamCount <= 1)
	{
		PlayerState->SetTeam(ETeam::Neutral);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, "New Count");
		// Count Players Per Team
		TArray<int32> TeamCounts;
		TeamCounts.Init(0, TeamCount);
		for (APlayerState* PlayerStates : GameState->PlayerArray)
		{
			if (ACorePlayerState* CorePlayerStates = Cast<ACorePlayerState>(PlayerStates))
			{
				uint8 TeamIndex = static_cast<uint8>(CorePlayerStates->GetTeam());
				if (TeamIndex > 0)
				{
					TeamCounts[TeamIndex - 1] += 1;
				}
			}
		}

		// Get Min Team Size
		int32 MinTeamSize = MAX_int32;
		for (int32 i = 0; i < TeamCounts.Num(); ++i)
		{
			FString TeamName = UEnum::GetValueAsString(static_cast<ETeam>(i + 1));

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Team %s has %d players"), *TeamName, TeamCounts[i]));

			if (MinTeamSize > TeamCounts[i])
			{
				MinTeamSize = TeamCounts[i];
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Min Team Size is %d players"), MinTeamSize));

		// Get Teams with Min Size
		TArray<ETeam> SelectableTeams;
		for (int32 i = 0; i < TeamCounts.Num(); ++i)
		{
			if (TeamCounts[i] == MinTeamSize)
			{
				SelectableTeams.Add(static_cast<ETeam>(i + 1));
				
				FString TeamName = UEnum::GetValueAsString(static_cast<ETeam>(i + 1));
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Team %s can be selected "), *TeamName));
			}
		}
		
		ETeam NewTeam = SelectableTeams[FMath::RandRange(0, SelectableTeams.Num() -1)];

		PlayerState->SetTeam(NewTeam);
	}
}
