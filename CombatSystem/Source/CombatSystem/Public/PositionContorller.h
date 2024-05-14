// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Types\PositionControllerTypes.h"

#include "PositionContorller.generated.h"


class ACombatSystemCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATSYSTEM_API UPositionContorller : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPositionContorller();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = MainInfo)
	FPositionQueueHandle CurrentHandle = {};

private:


	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_HandleOperation();


	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TravelForward(FPositionQueueHandle handle);


	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TargetFollowCamera(FPositionQueueHandle handle);
	

};
