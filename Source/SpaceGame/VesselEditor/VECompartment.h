// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VECompartment.generated.h"

UENUM(BlueprintType)
enum class EVESocketTypeEnum : uint8
{
	VE_XPos	UMETA(DisplayName = "Positive X Direction Socket"),
	VE_XNeg	UMETA(DisplayName = "Negative X Direction Socket"),
	VE_YPos	UMETA(DisplayName = "Positive Y Direction Socket"),
	VE_YNeg	UMETA(DisplayName = "Negative Y Direction Socket"),
	VE_ZPos	UMETA(DisplayName = "Positive Z Direction Socket"),
	VE_ZNeg	UMETA(DisplayName = "Negative Z Direction Socket")
};


UCLASS()
class SPACEGAME_API AVECompartment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVECompartment();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	class AVEModule* _rootModule;
	TMap<EVESocketTypeEnum, AVEModule*> _socketMap;

};
