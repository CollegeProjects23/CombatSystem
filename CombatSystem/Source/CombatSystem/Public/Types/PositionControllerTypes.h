// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PositionControllerTypes.generated.h"


UENUM(BlueprintType)
enum class EPositionHandleType : uint8
{
	None = 0,
	TravelForward,
	Knockback,
	FollowTarget,
};

UENUM(BlueprintType)
enum class ESpeedLerpType : uint8
{
	None = 0,
	Linear,
	Sine,
	Exponential
};

USTRUCT(BlueprintType)
struct COMBATSYSTEM_API FPositionQueueHandle
{
public:

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPositionHandleType handleType = EPositionHandleType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESpeedLerpType lerpType = ESpeedLerpType::None;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartingSpeed = 0.f;	
};



/**
 * 
 */
//class COMBATSYSTEM_API PositionControllerTypes
//{
//public:
//	PositionControllerTypes();
//	~PositionControllerTypes();
//};
