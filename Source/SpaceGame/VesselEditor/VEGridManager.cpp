// Fill out your copyright notice in the Description page of Project Settings.


#include "VEGridManager.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Actor.h"
#include "Engine/Blueprint.h"

DEFINE_LOG_CATEGORY(SpaceGame_VEGridManager);

// Sets default values
AVEGridManager::AVEGridManager()
	:_initialized(false)
	,_DummyModuleClassType(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FString path = TEXT("Blueprint'/Game/VesselEditor/VEModuleBP.VEModuleBP'");
	static ConstructorHelpers::FObjectFinder<UBlueprint> ModuleAsset(*path);
	_DummyModuleClassType = ModuleAsset.Object->GeneratedClass;
	if (!_DummyModuleClassType)
	{
		UE_LOG(SpaceGame_VEGridManager, Warning, TEXT("Failed to find default module BP class at: %s"), *path);
	}

}

// Called when the game starts or when spawned
void AVEGridManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVEGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Initialize the grid manager - must be called after construction (either code or BP)
bool AVEGridManager::InitializeGridManager(FIntVector gridDimentions_, int moduleUnitSize_, FIntVector gridZeroPoint_, bool fillWithDummies_)
{
	UE_LOG(SpaceGame_VEGridManager, Log, TEXT("Initializing the GridManager with size of %s"), *(gridDimentions_.ToString()));
	
	// set defaults
	_GridDimentions = gridDimentions_;
	_ModuleUnitSize = moduleUnitSize_;
	_GridZeroLocation = gridZeroPoint_;

	for (int zidx = 0; zidx < _GridDimentions.Z; zidx++)
	{
		FVEModuleMatrix matrix;
		for (int yidx = 0; yidx < _GridDimentions.Y ; yidx++)
		{
			FVEModuleArray arr;
			for (int xidx = 0; xidx < _GridDimentions.X; xidx++)
			{
				arr._Modules.Add(nullptr);			
			}
			matrix._ModuleArrays.Add(arr);
		}
		_grid._ModuleMatixArray.Add(matrix);
	}


	// try to load dummy class type
	if (fillWithDummies_ && !_DummyModuleClassType)
	{
		UE_LOG(SpaceGame_VEGridManager, Error, TEXT("Dummy Module Class is not defined! cannot fill with dummies"));
		return false;
	}

	UE_LOG(SpaceGame_VEGridManager, Log, TEXT("GridManager Initialization Complete"));
	_initialized = true;
	
	if( fillWithDummies_ )
	{
		FillWithDummies();
	}

	return true;
}

void AVEGridManager::FillWithDummies()
{
	if (!_initialized)
	{
		UE_LOG(SpaceGame_VEGridManager, Warning, TEXT("GridManager is not initialized, skipping funciton call!"));
		return;
	}

	// Initialize the grid itself (create all the module actors and place them correctly on the map)
	for (int zidx = 0; zidx < _GridDimentions.Z; zidx++)
	{
		for (int yidx = 0; yidx < _GridDimentions.Y; yidx++)
		{
			for (int xidx = 0; xidx < _GridDimentions.X; xidx++)
			{
				FIntVector ords(xidx, yidx, zidx);

				AVEModule* module = GetModuleAt(ords);
				if (!module)
				{
					if (!AddModule(_DummyModuleClassType, ords))
					{
						UE_LOG(SpaceGame_VEGridManager, Error, TEXT("Failed to add new module at coords: %s"), *(ords.ToString()));
					}
					UE_LOG(SpaceGame_VEGridManager, Log, TEXT("Created Dummy module at ords: %s"), *(ords.ToString()));
				}
				else
				{
					UE_LOG(SpaceGame_VEGridManager, Verbose, TEXT("Module Exists - not creating dummy. ords: %s"), *(ords.ToString()));
				}
			}
		}
	}
	UE_LOG(SpaceGame_VEGridManager, Log, TEXT("All dummies have been created"));
}

// return the grid coordinates (based on 0,0,0) from the module's array position (always 0 based)
FIntVector AVEGridManager::GetCoordinatesFromOrdinal(FIntVector ordinalCoordinates_)
{
	return ordinalCoordinates_ - _GridZeroLocation;
}

// return the module's array position based on the grid coordinates (given the current zero location)
FIntVector AVEGridManager::GetOrdinalFromCoordinates(FIntVector gridCoordinates_)
{
	return gridCoordinates_ + _GridZeroLocation;
}

bool AVEGridManager::AddModule(TSubclassOf<AVEModule> module_, FIntVector ordinals_)
{
	if (!_initialized)
	{
		UE_LOG(SpaceGame_VEGridManager, Warning, TEXT("GridManager is not initialized, skipping funciton call!"));
		return false;
	}

	// try to see whats in the grid now...
	AVEModule* current = GetModuleAt(ordinals_);
	if (current)
	{
		if (current->IsA(_DummyModuleClassType))
		{
			// remove the dummy module from these ordinals...
			RemoveModule(ordinals_);
		}
		else
		{
			UE_LOG(SpaceGame_VEGridManager, Error, TEXT("Module allready exists at requested ordinals cannot add new module there!"));
			return false;
		}
	}

	// create the new module...
	FVector spawnLocation(0, 0, 0);
	FRotator spawnRotation = FRotator::ZeroRotator;
	AVEModule* newActor = GetWorld()->SpawnActor<AVEModule>(module_, spawnLocation, spawnRotation);
	if (!newActor)
	{
		UE_LOG(SpaceGame_VEGridManager, Error, TEXT("Failed to spawn new actor for class type %s"), *(module_->GetName()));
		return false;
	}

	newActor->InitializeModule(this, ordinals_);

	// add new actor to matrix location...
	FVEModuleMatrix matrix;
	if (_grid._ModuleMatixArray.IsValidIndex(ordinals_.Z))
		matrix = _grid._ModuleMatixArray[ordinals_.Z];

	FVEModuleArray arr;
	if (matrix._ModuleArrays.IsValidIndex(ordinals_.Y))
		arr = matrix._ModuleArrays[ordinals_.Y];

	arr._Modules.Insert(newActor, ordinals_.X);
	matrix._ModuleArrays[ordinals_.Y] = arr;
	_grid._ModuleMatixArray[ordinals_.Z] = matrix;
	return true;
}


bool AVEGridManager::RemoveModule(FIntVector ordinals_)
{
	if (!_initialized)
	{
		UE_LOG(SpaceGame_VEGridManager, Warning, TEXT("GridManager is not initialized, skipping funciton call!"));
		return false;
	}

	AVEModule* module = GetModuleAt(ordinals_);
	if (!module)
		return false;

	_grid._ModuleMatixArray[ordinals_.Z]._ModuleArrays[ordinals_.Y]._Modules[ordinals_.X] = nullptr;
	GetWorld()->DestroyActor(module);
	
	return false;
}

void AVEGridManager::PositionModulesInSpace()
{
	if (!_initialized)
	{
		UE_LOG(SpaceGame_VEGridManager, Warning, TEXT("GridManager is not initialized, skipping funciton call!"));
		return;
	}

	for (int zidx = 0; zidx < _grid._ModuleMatixArray.Num(); zidx++)
	{
		FVEModuleMatrix matrix = _grid._ModuleMatixArray[zidx];
		for (int yidx = 0; yidx < matrix._ModuleArrays.Num(); yidx++)
		{
			FVEModuleArray arr = matrix._ModuleArrays[yidx];
			for (int xidx = 0; xidx < arr._Modules.Num(); xidx++)
			{
				AVEModule* module = arr._Modules[xidx];
				if (module)
				{
					FIntVector moduleOrdinals(xidx, yidx, zidx);
					FIntVector moduleCoords = GetCoordinatesFromOrdinal(moduleOrdinals); // calculate based on current "Zero" position (_GridZeroLocation)
					FVector spacialCoords = GetActorLocation() + (FVector(_ModuleUnitSize, _ModuleUnitSize, _ModuleUnitSize) * FVector(moduleCoords));

					UE_LOG(SpaceGame_VEGridManager, Log, TEXT("Detected module at ordinals [%] (coords [%s]). Relocation to spacial location [%s]"), *(moduleOrdinals.ToString()), *(moduleCoords.ToString()), *(spacialCoords.ToString()));

					// relocate module to its new loc in space
					module->SetActorLocation(spacialCoords);
				}
			}
		}
	}
}

AVEModule* AVEGridManager::GetModuleAt(FIntVector ordinals_)
{
	if (!_initialized)
	{
		UE_LOG(SpaceGame_VEGridManager, Warning, TEXT("GridManager is not initialized, skipping funciton call!"));
		return nullptr;
	}

	if (!_grid._ModuleMatixArray.IsValidIndex(ordinals_.Z))
	{
		UE_LOG(SpaceGame_VEGridManager, Error, TEXT("Requested Location ouside of grid Z axis bounds!"));
		return nullptr;
	}

	FVEModuleMatrix matrix = _grid._ModuleMatixArray[ordinals_.Z];

	if (!matrix._ModuleArrays.IsValidIndex(ordinals_.Y))
	{
		UE_LOG(SpaceGame_VEGridManager, Error, TEXT("Requested Location ouside of grid Y axis bounds!"));
		return nullptr;
	}

	FVEModuleArray array = matrix._ModuleArrays[ordinals_.Y];

	if (!array._Modules.IsValidIndex(ordinals_.X))
	{
		UE_LOG(SpaceGame_VEGridManager, Error, TEXT("Requested Location ouside of grid X axis bounds!"));
		return nullptr;
	}

	return array._Modules[ordinals_.X];
}