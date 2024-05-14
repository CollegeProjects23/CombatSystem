// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "Types\CharacterTypes.h"


#include "CombatSystemController.generated.h"


/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API ACombatSystemController : public APlayerController
{
	GENERATED_BODY()


	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	//Sprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Sprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Dodge;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Attack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SecondaryAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Block;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LockOn;

protected:

	ACombatSystemController();



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timer, meta = (AllowPrivateAccess = "true"))
	class UTimer* TimerComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = State)
	EMovementState movementState = EMovementState::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = State)
	ECombatState combatState = ECombatState::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = State)
	EAttackLethality attackLethality = EAttackLethality::NonLethal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = State)
	EAttackPattern attackPattern = EAttackPattern::None;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = State)
	bool bIsSprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = State)
	bool bIsTryingToBlock = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = State)
	int CurrentCombo = 1;


public:

	UFUNCTION(BlueprintCallable)
	void SetUpInputs();


	UFUNCTION(BlueprintCallable)
	void StartPlay();
};
