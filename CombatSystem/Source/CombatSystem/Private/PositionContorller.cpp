// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionContorller.h"


#include <Net\UnrealNetwork.h>

#include "GameFramework\SpringArmComponent.h"
#include "Camera\CameraComponent.h"

#include "CombatSystem\CombatSystemCharacter.h"

UPositionContorller::UPositionContorller()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	bAutoActivate = true;


	//SetIsReplicated(true);
}


// Called when the game starts
void UPositionContorller::BeginPlay()
{
	Super::BeginPlay();

	
}

//---------------------------------------
void UPositionContorller::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const  {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UPositionContorller, CurrentHandle, COND_AutonomousOnly);

}
//---------------------------------------


//---------------------------------------
void UPositionContorller::Multicast_TravelForward_Implementation(FPositionQueueHandle handle)
{

	// Get forward vector from capsule and scale it by 2
	//get the actor location and move it with the forward vector
	//add world delta seconds

	//if (!IsValid(handle.Target)) { return; }

	if (UWorld* world = GetWorld()) {
		if (ACombatSystemCharacter* Char = Cast<ACombatSystemCharacter>(handle.Target)) {
			FVector Direction = Char->GetActorForwardVector() * handle.StartingSpeed;
			FVector NewPosition = Char->GetActorLocation() + Direction + FVector(world->DeltaTimeSeconds);

			handle.Target->SetActorLocation(NewPosition);
		}
	}
}
//---------------------------------------
void UPositionContorller::Multicast_TargetFollowCamera_Implementation(FPositionQueueHandle handle) {

	//Get the camera comp rotation
	//Set target y rotation to the camera comp y rotation
	//Use World delta to make it smooth

	if (UWorld* world = GetWorld()) {
		if (ACombatSystemCharacter* Char = Cast<ACombatSystemCharacter>(handle.Target)) {
			USpringArmComponent* CamSpring = Char->GetCameraBoom();

			FRotator SpringRot = CamSpring->GetComponentRotation();
			FRotator CharRotation = Char->GetActorRotation();

			FRotator NewRotation = FRotator(SpringRot.Yaw, 0, 0);

			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(100, .1, FColor::Red, FString(NewRotation.ToString()));
			}

			Char->SetActorRotation(NewRotation);
		}
	}

}

//---------------------------------------


void UPositionContorller::Server_HandleOperation_Implementation()
{

	if (CurrentHandle.handleType != EPositionHandleType::None) { 
	
		switch (CurrentHandle.handleType) {


		case EPositionHandleType::TravelForward:

			Multicast_TravelForward(CurrentHandle);

			break;
		}
	}
}



//--------------------------------------


//--------------------------------------

// Called every frame
void UPositionContorller::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

