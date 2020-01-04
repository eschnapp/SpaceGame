// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VEModule.h"
#include "VEGridManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(SpaceGame_VEGridManager, Log, All);


USTRUCT(BlueprintType)
struct FVEModuleArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AVEModule*> _Modules;
};

USTRUCT(BlueprintType)
struct FVEModuleMatrix
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FVEModuleArray> _ModuleArrays;
};

USTRUCT(BlueprintType)
struct FVEModuleGrid
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FVEModuleMatrix> _ModuleMatixArray;
};

UCLASS()
class SPACEGAME_API AVEGridManager : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AVEGridManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Category = "Grid", BlueprintCallable)
	bool InitializeGridManager(FIntVector gridDimentions_, int moduleSize_, FIntVector gridZeroPoint_, bool fillWithDummies_);

	UFUNCTION(Category = "Grid", BlueprintCallable)
	const FIntVector& getGridDimentions() { return _GridDimentions; }

	UFUNCTION(Category = "Grid", BlueprintCallable)
	const FVEModuleGrid& getGrid() { return _grid; }

	UFUNCTION(Category = "Grid", BlueprintCallable)
	const int getModuleUnitSize() { return _ModuleUnitSize; }

	UFUNCTION(Category = "Grid", BlueprintCallable)
	FIntVector GetCoordinatesFromOrdinal(FIntVector ordinalCoordinates_);

	UFUNCTION(Category = "Grid", BlueprintCallable)
	FIntVector GetOrdinalFromCoordinates(FIntVector gridCoordinates_);

	UFUNCTION(Category = "Grid", BlueprintCallable)
	bool AddModule(TSubclassOf<AVEModule> module_, FIntVector ordinals_);

	UFUNCTION(Category = "Grid", BlueprintCallable)
	bool RemoveModule(FIntVector ordinals_);

	UFUNCTION(Category = "Grid", BlueprintCallable)
	AVEModule* GetModuleAt(FIntVector ordinals_);

	UFUNCTION(Category = "Grid", BlueprintCallable)
	void FillWithDummies();

	UFUNCTION(Category = "Grid", BlueprintCallable)
	void PositionModulesInSpace();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FVEModuleGrid _grid;
	FIntVector _GridDimentions;
	int	_ModuleUnitSize;
	bool _initialized;

public:

	UPROPERTY(Category = "Grid", EditAnywhere, BlueprintReadWrite)
	FIntVector _GridZeroLocation;

	UPROPERTY(Category = "Grid", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AVEModule> _DummyModuleClassType;
	


};
