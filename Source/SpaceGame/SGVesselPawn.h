// All copy rights reserved for RZ East Coast and papa r4z

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VBS/VBS_ModuleActor.h"
#include "SGVesselPawn.generated.h"


USTRUCT(BlueprintType)
struct FSGVesselDescriptorItem
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	EVBS_ModuleTypes ModuleType;		// Module Type

	UPROPERTY(BlueprintReadWrite)
	FIntVector OffsetFromRoot;			// Offset (in grid increments) from ROOT module

	UPROPERTY(BlueprintReadWrite)
	FIntVector RotationPhaseVector;		// Rotation Phase (in 90 degree increments) from module center point

};

UCLASS()
class SPACEGAME_API ASGVesselPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASGVesselPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void ConstructVesselProcedural(TArray<FSGVesselDescriptorItem> DescriptorArray, float GridSnapSize);

	UStaticMesh* GetStaticMeshForModule(EVBS_ModuleTypes ModuleType);
public:

	UPROPERTY(Category = "Vessel Construction", BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* VesselRootModuleComponent;

	TMap<EVBS_ModuleTypes, FVBS_ModuleDescriptor > ModuleConstructionDescriptorMap;
};
