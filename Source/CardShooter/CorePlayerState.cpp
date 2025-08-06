// Fill out your copyright notice in the Description page of Project Settings.


#include "CorePlayerState.h"

void ACorePlayerState::SetTeam(ETeam NewTeam)
{
	Team = NewTeam;
	
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("Team set to: %s"), *UEnum::GetValueAsString(Team)));
}

ETeam ACorePlayerState::GetTeam() const
{
	return Team;
}
