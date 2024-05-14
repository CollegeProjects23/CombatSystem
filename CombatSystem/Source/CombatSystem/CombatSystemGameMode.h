// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Public\Types\GameModeTypes.h"

#include "GameFramework/GameModeBase.h"
#include "CombatSystemGameMode.generated.h"

class ANavigationObjectBase;


UCLASS(minimalapi)
class ACombatSystemGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACombatSystemGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ServerInfo)
	TSoftObjectPtr<ANavigationObjectBase> SpawnLocation;


protected:




};