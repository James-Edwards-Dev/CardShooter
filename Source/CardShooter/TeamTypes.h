#pragma once

#include "CoreMinimal.h"
#include "TeamTypes.generated.h"

UENUM()
enum class ETeam: uint8
{
	Neutral UMETA(DisplayName = "Neutral "),
	Red UMETA(DisplayName = "Red"),
	Black UMETA(DisplayName = "Black"),
	Blue UMETA(DisplayName = "Blue"),
	Green UMETA(DisplayName = "Green")
};
