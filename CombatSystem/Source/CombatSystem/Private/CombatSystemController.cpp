// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatSystemController.h"

#include "CombatSystem/CombatSystemCharacter.h"
#include "CombatSystem/CombatSystemGameMode.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"

#include "Net\UnrealNetwork.h"
#include "Timer\Timer.h"


ACombatSystemController::ACombatSystemController()
{
	bReplicates = true;


	TimerComp = CreateDefaultSubobject<UTimer>(TEXT("TimeComp"));
	TimerComp->bAutoActivate = true;
}

void ACombatSystemController::SetUpInputs()
{

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

}

void ACombatSystemController::StartPlay()
{
	this->SetUpInputs();
}

void ACombatSystemController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACombatSystemController, movementState, COND_AutonomousOnly);
	DOREPLIFETIME_CONDITION(ACombatSystemController, combatState, COND_AutonomousOnly);
	DOREPLIFETIME_CONDITION(ACombatSystemController, attackLethality, COND_AutonomousOnly);
	DOREPLIFETIME_CONDITION(ACombatSystemController, attackPattern, COND_AutonomousOnly);

	DOREPLIFETIME_CONDITION(ACombatSystemController, bIsSprinting, COND_AutonomousOnly);
	DOREPLIFETIME_CONDITION(ACombatSystemController, bIsTryingToBlock, COND_AutonomousOnly);
	DOREPLIFETIME_CONDITION(ACombatSystemController, CurrentCombo, COND_AutonomousOnly);
}




