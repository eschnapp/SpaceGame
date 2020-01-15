// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "VBS_ModuleActor.generated.h"

/**
 * VBS Module Descriptor - contains all information for creation of a new module, including mesh and snap points for overlap and connections
 */
USTRUCT(BlueprintType)
struct FVBS_ModuleDescriptor
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString ModuleInstanceID;						// unique instance id

	UPROPERTY(BlueprintReadWrite)
	UStaticMesh* ModuleStaticMesh;					// Static Mesh for the module

	UPROPERTY(BlueprintReadWrite)
	TArray<FIntVector> ModuleSnapPointOffset;		// vector list of snap point offsets 

	UPROPERTY(BlueprintReadWrite)
	TArray<FIntVector> ModuleConnectorPointOffset;  // vector list of connector point offsets 


	// Setters
	void SetModuleStaticMesh(UStaticMesh* NewValue)
	{
		ModuleStaticMesh = NewValue;
	}
	void SetModuleSnapPointOffset(TArray<FIntVector> NewValue)
	{
		ModuleSnapPointOffset = NewValue;
	}
	void SetModuleConnectorPointOffset(TArray<FIntVector> NewValue)
	{
		ModuleConnectorPointOffset = NewValue;
	}
	void SetModuleInstanceId(FString NewValue)
	{
		ModuleInstanceID = NewValue;
	}

	// Getters
	UStaticMesh* GetModuleStaticMesh()
	{
		return ModuleStaticMesh;
	}
	TArray<FIntVector> GetModuleSnapPointOffset()
	{
		return ModuleSnapPointOffset;
	}
	TArray<FIntVector> GetModuleConnectorPointOffset()
	{
		return ModuleConnectorPointOffset;
	}
	FString GetModuleInstanceId()
	{
		return ModuleInstanceID;
	}

	// Ctor
	FVBS_ModuleDescriptor()
	{
		ModuleStaticMesh = nullptr;
		ModuleSnapPointOffset.Empty();
	}
};

/**
 * Module Rotation phases in 90 degree rotation
 */
UENUM(BlueprintType)
enum class EVBS_ModuleRotationPhase : uint8
{
	MR_Phase0	UMETA(DisplayName=  "0 Degree Phase"),
	MR_Phase90	UMETA(DisplayName = "90 Degree Phase"),
	MR_Phase180	UMETA(DisplayName = "180 Degree Phase"),
	MR_Phase270	UMETA(DisplayName = "270 Degree Phase")
};

/**
 * Enum that lists all the module types
 */
UENUM(BlueprintType)
enum class EVBS_ModuleTypes : uint8
{
	Capsule		UMETA(DisplayName = "Capsule"),
	MT_BasicI	UMETA(DisplayName = "Basic I Module"),
	MT_BasicL	UMETA(DisplayName = "Basic L Module"),
	MT_BasicT	UMETA(DisplayName = "Basic T Module"),
	MT_BasicX	UMETA(DisplayName = "Basic X Module")
};

/**
 * Module Collision States
 */
UENUM(BlueprintType)
enum class EVBS_ModuleCollisionState : uint8
{
	MCS_Detached	UMETA(DisplayName = "Detached Module"),
	MCS_Connected	UMETA(DisplayName = "Connected Module"),
	MCS_Overlapping UMETA(DisplayName = "Overlapping Module")
};

/**
 * Main VBS Module actor class - handles all the module specific operations and properties.
 */
UCLASS()
class SPACEGAME_API AVBS_ModuleActor : public AActor
{
	GENERATED_BODY()

	// just in case we wanna keep something private in the module but still access from controller
	friend class AVBS_PlayerController;

public:

	// Static mesh component that holds the module's static mesh. should be specificed in the descriptor for construction
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* _staticMeshComponent;

	// Default material for the module (wtf to do when multi-mat??! :/ )
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface*	_defaultMaterial;

	// Mesh to use for the snap (overlap) point markers
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UStaticMeshComponent*> _snapPointMarkers;
	
	// Mesh to use for the connector point markers
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UStaticMeshComponent*> _connectorPointMarkers;

public:	
	// Sets default values for this actor's properties
	AVBS_ModuleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Post construct initialization code (expected to call this after setting up all the pre-construct properties)
	UFUNCTION(Category = "General", BlueprintCallable, CallInEditor)
	void InitializeModule();

	// Update the current mesh from the mesh property
	UFUNCTION(Category = "General", BlueprintCallable, CallInEditor)
	void UpdateCurrentMesh();
	
	// Update the location in 3D of all the snap/connector markers for the module (shou8ld only be used for dev)
	UFUNCTION(Category = "General", BlueprintCallable, CallInEditor) 
	void UpdateAllSnapMarkers();

	// Get array of grid snap (overlap) points for this module
	UFUNCTION(Category = "Placement", BlueprintCallable)
	TArray<FVector> GetGridSnapPoints();

	// Get array of grid connector points for this module
	UFUNCTION(Category = "Placement", BlueprintCallable)
	TArray<FVector> GetGridConnectorPoints();

	// Is this module overlapping with another given module
	UFUNCTION(Category = "Placement", BlueprintCallable)
	bool IsOverlapingWithModule(AVBS_ModuleActor* otherActor_);

	// Is this module connected with another module
	UFUNCTION(Category = "Placement", BlueprintCallable)
	bool IsConnectingWithModule(AVBS_ModuleActor* otherActor_);

	// show or hide the grid snap markers for this module
	UFUNCTION(Category = "Placement", BlueprintCallable)
	void ShowHideSnapMarkers(bool showMarkers_);

	// show or hide the grid connector markers for this module
	UFUNCTION(Category = "Placement", BlueprintCallable)
	void ShowHideConnectorMarkers(bool showMarkers_);

	// Set the module's rotation phase
	void SetModuleRotation(EVBS_ModuleRotationPhase newRotationPhase_);

	// Rotate the module either CW or CCW in 1 rotation phase
	void RotateModule(bool ccw_);

	// Override the current material with another material
	void OverrideModuleMaterial(UMaterialInterface* overrideMaterial_);

	// Reset to the module's default material
	void ResetModuleMaterial();
	

	// Is module being placed
	UPROPERTY(Category = "Placement", VisibleAnywhere, BlueprintReadOnly)
	bool _isBeingPlaced;

	// Is this module showing snap (overlap) markers
	UPROPERTY(Category = "Placement", VisibleAnywhere, BlueprintReadOnly)
	bool _isShowingSnapMarkers;

	// Is this module showing its connector markers
	UPROPERTY(Category = "Placement", VisibleAnywhere, BlueprintReadOnly)
	bool _isShowingConnectorMarkers;

	// Current collision state for this module (as measured last)
	UPROPERTY(Category = "Placement", VisibleAnywhere, BlueprintReadOnly)
	EVBS_ModuleCollisionState _CurrentCollisionState;

	// List of offset points for snap (overlap) from pivot on this module
	UPROPERTY(Category = "Placement", EditAnywhere, BlueprintReadWrite)
	TArray<FIntVector> _ModuleSnapPointOffsets;

	// List of offset points for connection from the pivot of this module
	UPROPERTY(Category = "Placement", EditAnywhere, BlueprintReadWrite)
	TArray<FIntVector> _ModuleConnectorPointOffsets;

	// Staic mesh to use for this module
	UPROPERTY(Category = "Module", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* _moduleStaticMesh;
};
