// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Types\CharacterTypes.h"


#include "CombatSystemCharacter.generated.h"


class ACombatSystemController;
class UPositionContorller;

UCLASS(config=Game)
class ACombatSystemCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;



public:
	ACombatSystemCharacter();



	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "CharacterData")
	bool bIsSprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "CharacterData")
	bool bIsTryingToBlock = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "CharacterData")
	int CurrentCombo = 1;*/





	UFUNCTION(BlueprintCallable)
	class UTimer* GetTimer() { return CombatTimerComp; }


	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components)
	class USphereComponent* PlayerHitbox;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	class UPositionContorller* PositionController;

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


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LockOnSys)
	class ACharacter* LockOnTarget;

	//------------------------------------------

	UFUNCTION(BlueprintCallable)
	void UpdateState(FStateData Data);


	//----------------------------------------------
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_ScanForLockOnTarget();
	//----------------------------------------------


private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timer, meta = (AllowPrivateAccess = "true"))
	class UTimer* CombatTimerComp;


	/** Called for movement input */
	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	UFUNCTION(BlueprintCallable)
	void Look(const FInputActionValue& Value);
};

