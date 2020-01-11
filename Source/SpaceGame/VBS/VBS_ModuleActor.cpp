// Fill out your copyright notice in the Description page of Project Settings.


#include "VBS_ModuleActor.h"

// Sets default values
AVBS_ModuleActor::AVBS_ModuleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVBS_ModuleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVBS_ModuleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVBS_ModuleActor::UpdateCurrentMesh()
{

}

void AVBS_ModuleActor::SetModuleRotation(EVBDS_ModuleRotationPhase newRotationPhase_)
{

}

void AVBS_ModuleActor::RotateModule(bool ccw_)
{

}