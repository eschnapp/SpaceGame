// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VBS_ModuleActor.generated.h"

UENUM()
enum class EVBDS_ModuleRotationPhase : uint8
{
	MR_Phase0	UMETA(DisplayName=  "0 Degree Phase"),
	MR_Phase90	UMETA(DisplayName = "90 Degree Phase"),
	MR_Phase180	UMETA(DisplayName = "180 Degree Phase"),
	MR_Phase270	UMETA(DisplayName = "270 Degree Phase")
};


UCLASS()
class SPACEGAME_API AVBS_ModuleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVBS_ModuleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateCurrentMesh();
	void SetModuleRotation(EVBDS_ModuleRotationPhase newRotationPhase_);
	void RotateModule(bool ccw_);

	UPROPERTY(Category = "Placement", VisibleAnywhere, BlueprintReadOnly)
	bool _isBeingPlaced;

	UPROPERTY(Category = "Module", VisibleAnywhere, BlueprintReadOnly)
	class UStaticMesh* _moduleStaticMesh;

	UPROPERTY(Category = "Module", VisibleAnywhere, BlueprintReadOnly)
	EVBDS_ModuleRotationPhase _currentRotationPhase;
};
