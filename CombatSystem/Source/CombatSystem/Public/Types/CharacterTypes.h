// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.generated.h"
/**
 * 
 */

UENUM(BlueprintType)
enum class ECombatState: uint8
{

	None = 0,
	Dodging,
	Attacking,
	Blocking,

};

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	None = 0 ,
	Walking,
	Running
};

UENUM(BlueprintType)
enum class EAttackPattern : uint8
{
	None = 0,
	Left,
	Right
};

UENUM(BlueprintType)
enum class EAttackLethality : uint8
{
	NonLethal = 0,
	Lethal,
};

/*
	WalkSpeed
	Acceleration speed
	Decceleration speed

*/
USTRUCT(BlueprintType)
struct COMBATSYSTEM_API FStateData {
public:

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Acceleration = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Deceleration = 0;

};

USTRUCT(BlueprintType)
struct COMBATSYSTEM_API FHeadTrackingSettings {
public:

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector offset = FVector(0, 0, 0);
};

