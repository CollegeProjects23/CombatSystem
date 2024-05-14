// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CharacterTypes.h"

#include "GameModeTypes.generated.h"

USTRUCT(BlueprintType)
struct COMBATSYSTEM_API FClientHandle {
public:

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APawn* character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMovementState movementState = EMovementState::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECombatState combatState = ECombatState::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackLethality attackLethality = EAttackLethality::NonLethal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackPattern attackPattern = EAttackPattern::None;


};


/**
 * 
 */
class COMBATSYSTEM_API GameModeTypes
{
public:
	GameModeTypes();
	~GameModeTypes();
};
