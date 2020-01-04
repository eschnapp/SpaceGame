// Fill out your copyright notice in the Description page of Project Settings.


#include "VECompartment.h"



// Sets default values
AVECompartment::AVECompartment()
	:_rootModule(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_socketMap.Add(EVESocketTypeEnum::VE_XPos, nullptr);
	_socketMap.Add(EVESocketTypeEnum::VE_XNeg, nullptr);
	_socketMap.Add(EVESocketTypeEnum::VE_YPos, nullptr);
	_socketMap.Add(EVESocketTypeEnum::VE_YNeg, nullptr);
	_socketMap.Add(EVESocketTypeEnum::VE_ZPos, nullptr);
	_socketMap.Add(EVESocketTypeEnum::VE_ZNeg, nullptr);
}

// Called when the game starts or when spawned
void AVECompartment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVECompartment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

