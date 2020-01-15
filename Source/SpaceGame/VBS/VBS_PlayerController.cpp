// Fill out your copyright notice in the Description page of Project Settings.


#include "VBS_PlayerController.h"
#include "VBS_ModuleActor.h"
#include "VBS_PopupMenuBase.h"
#include "Blueprint/UserWidget.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(LogGame_PlayerController)

AVBS_PlayerController::AVBS_PlayerController()
	: _moduleBeingPlaced(nullptr)			// no module placed by default
	, _placingDistanceFromPlayer(2000.0)	// 20.00 meter from the camera
	, _snapGridSize(1000.0)					// 10 meter grid snap default
	, _menu_ActionSelection(nullptr)		// null action menu
	, _menu_NewModuleSelection(nullptr)		// null new module menu
	, _rootVBSModule(nullptr)				// null root module
{
	UE_LOG(LogGame_PlayerController, Log, TEXT("Creating new VBS Player Controller"));

	// Enable the controller's mouse cursor and click/overlap events so we can capture clicking and overlap on actors in the 3D scene
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void AVBS_PlayerController::SetupInputComponent()
{
	
	Super::SetupInputComponent();

	// Setup all the action binds for the controller
	InputComponent->BindAction("VBS_ModulePlacementDistanceINC",	IE_Pressed, this, &AVBS_PlayerController::HandleInputEvent_ModulePlacementDistanceINC);
	InputComponent->BindAction("VBS_ModulePlacementDistanceINC",	IE_Pressed, this, &AVBS_PlayerController::HandleInputEvent_ModulePlacementDistanceINC);
	InputComponent->BindAction("VBS_ModulePlacementRotationCW",		IE_Pressed, this, &AVBS_PlayerController::HandleInputEvent_ModulePlacementRotationCW);
	InputComponent->BindAction("VBS_ModulePlacementRotationCCW",	IE_Pressed, this, &AVBS_PlayerController::HandleInputEvent_ModulePlacementRotationCCW);
	InputComponent->BindAction("VBS_ActionMenu",					IE_Pressed, this, &AVBS_PlayerController::HandleActionMenuPopup);
	InputComponent->BindAction("VBS_ModulePlacementComplete",		IE_Pressed, this, &AVBS_PlayerController::HandleModulePlacementComplete);
	InputComponent->BindAction("VBS_ModulePlacementCancel",			IE_Pressed, this, &AVBS_PlayerController::HandleModulePlacementCancel);
	InputComponent->BindAction("VBS_ShowSnapMarkers",				IE_Pressed, this, &AVBS_PlayerController::HandleShowHideSnapMarkers);
	InputComponent->BindAction("VBS_ShowConnectorMarkers",			IE_Pressed, this, &AVBS_PlayerController::HandleShowHideConnectorMarkers);
	
	// Setup all the axis binds for the controller
	InputComponent->BindAxis("Turn", this, &AVBS_PlayerController::HandleInputAxis_Turn);
	InputComponent->BindAxis("LookUp", this, &AVBS_PlayerController::HandleInputAxis_LookUp);

}

// Called when level is loaded or when actor is spawned
void AVBS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Setup all the user menus...
	// Main Action Menu
	if (PopupMenuUserWidget)
	{
		_menu_ActionSelection = CreateWidget<UVBS_PopupMenuBase>(this, PopupMenuUserWidget);
		_menu_ActionSelection->SetVisibility(ESlateVisibility::Hidden);
		_menu_ActionSelection->SetPositionInViewport(FVector2D::ZeroVector);
		_menu_ActionSelection->AddToViewport();
		_menu_ActionSelection->OnMenuItemSelected.AddDynamic(this, &AVBS_PlayerController::HandleActionMenuItemSelection);
	}

	// New Module Selection
	if (NewModuleSelection)
	{
		_menu_NewModuleSelection = CreateWidget<UVBS_PopupMenuBase>(this, NewModuleSelection);
		_menu_NewModuleSelection->SetVisibility(ESlateVisibility::Hidden);
		_menu_NewModuleSelection->SetPositionInViewport(FVector2D::ZeroVector);
		_menu_NewModuleSelection->AddToViewport();
		_menu_NewModuleSelection->OnMenuItemSelected.AddDynamic(this, &AVBS_PlayerController::HandleNewModuleMenuItemSelection);
	}

	// Create the "ROOT" capsule...
	_rootVBSModule = SpawnModule(RootModuleDescriptor);
	_rootVBSModule->SetActorLocation(FVector::ZeroVector);

}

// Called every frame
void AVBS_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_moduleBeingPlaced) //  something is being placed... handle it.
	{	
		FVector mouseWorldLocation, mouseWorldDirection;
		if (!DeprojectMousePositionToWorld(mouseWorldLocation, mouseWorldDirection))
		{
			UE_LOG(LogGame_PlayerController, Error, TEXT("Failed to deproject mouse position to world space!"));
		}
		else
		{
			// compute new transform location based on mouse projection and direction
			FVector trxLocation = (mouseWorldDirection * _placingDistanceFromPlayer) + mouseWorldLocation;

			// snap to the closest grid location
			trxLocation = trxLocation.GridSnap(_snapGridSize);
			_moduleBeingPlaced->SetActorLocation(trxLocation);

			// perform collision check against other modules
			EVBS_ModuleCollisionState mcs = PerformGridCollisionCheck(_moduleBeingPlaced);
			if (mcs != _moduleBeingPlaced->_CurrentCollisionState)
			{
				switch (mcs)
				{
				case EVBS_ModuleCollisionState::MCS_Connected:
					_moduleBeingPlaced->OverrideModuleMaterial(ModuleConnectionPlacementMaterial);
					break;
				case EVBS_ModuleCollisionState::MCS_Overlapping:
					_moduleBeingPlaced->OverrideModuleMaterial(ModuleOverlapPlacementMaterial);
					break;
				case EVBS_ModuleCollisionState::MCS_Detached:
				default:
					_moduleBeingPlaced->ResetModuleMaterial();
					break;
				}

				FString msg = FString::Printf(TEXT("Prev MCS: %d, New MCS: %d"), (uint8)_moduleBeingPlaced->_CurrentCollisionState, (uint8)mcs);
				if (GEngine)
				{
					int uid = 318423223;
					GEngine->AddOnScreenDebugMessage(uid, 5.0f, FColor::Red, msg);
				}
				_moduleBeingPlaced->_CurrentCollisionState = mcs;
			}
		}
	}
}

// check if there is a module that is being placed right now
bool AVBS_PlayerController::IsModuleBeingPlaced()
{
	if (!_moduleBeingPlaced)
		return false;

	return _moduleBeingPlaced->_isBeingPlaced;
}

// start placing a module that has been spawned into the scene
void AVBS_PlayerController::StartPlacingModule(AVBS_ModuleActor* moduleToPlace_)
{
	if (!moduleToPlace_)
		return;
	moduleToPlace_->_isBeingPlaced = true;
	_moduleBeingPlaced = moduleToPlace_;
}

// stop placing the current module being placed
void AVBS_PlayerController::StopPlacingCurrentModule()
{
	if (!_moduleBeingPlaced)
		return;

	_moduleBeingPlaced->_isBeingPlaced = false;
	_moduleBeingPlaced = nullptr;
}

// spawn and register a new module in the scene, optionally auto start to place it
AVBS_ModuleActor* AVBS_PlayerController::SpawnModule(FVBS_ModuleDescriptor descriptor_, bool bAutoStartPlacing)
{
	// spawn actor of the module
	FActorSpawnParameters asp;
	asp.Name = FName(*descriptor_.GetModuleInstanceId());
	AVBS_ModuleActor* newModule = GetWorld()->SpawnActor<AVBS_ModuleActor>(FVector::ZeroVector, FRotator::ZeroRotator, asp);
	if (!newModule)
	{
		UE_LOG(LogGame_PlayerController, Warning, TEXT("Failed to spawn new module actor!"));
		return nullptr;
	}

	// setup static mesh and snap points
	newModule->_moduleStaticMesh = descriptor_.GetModuleStaticMesh();
	newModule->_ModuleSnapPointOffsets = descriptor_.GetModuleSnapPointOffset();
	newModule->_ModuleConnectorPointOffsets = descriptor_.GetModuleConnectorPointOffset();
	
	// initialize the module (post construct)
	newModule->InitializeModule();

	// add to modules array
	_allModules.Add(newModule);

	if (bAutoStartPlacing)
		StartPlacingModule(newModule);

	return newModule;
}

// Perform grid collision check for both overlap and connection status (see header for more info)
EVBS_ModuleCollisionState AVBS_PlayerController::PerformGridCollisionCheck(AVBS_ModuleActor* module_)
{
	// default return
	EVBS_ModuleCollisionState result = EVBS_ModuleCollisionState::MCS_Detached;

	for (TCheckedPointerIterator<AVBS_ModuleActor*, int32> moduleIter = _allModules.begin(); moduleIter != _allModules.end(); moduleIter.operator++())
	{
		// check connected...
		if (_moduleBeingPlaced->IsConnectingWithModule(*moduleIter))
		{
			result = EVBS_ModuleCollisionState::MCS_Connected;
		}

		// check overlap
		if (_moduleBeingPlaced->IsOverlapingWithModule(*moduleIter))
		{
			result = EVBS_ModuleCollisionState::MCS_Overlapping;
			break;
		}
	}

	return result;
}

void AVBS_PlayerController::HandleInputEvent_ModulePlacementDistanceINC()
{
	if (IsModuleBeingPlaced())
	{
		_placingDistanceFromPlayer = FGenericPlatformMath::Min<float>(5000.0, (_placingDistanceFromPlayer + 10.0));
	}
}

void AVBS_PlayerController::HandleInputEvent_ModulePlacementDistanceDEC()
{
	if (IsModuleBeingPlaced())
	{
		_placingDistanceFromPlayer = FGenericPlatformMath::Max<float>(100.0, (_placingDistanceFromPlayer - 10.0));
	}
}

void AVBS_PlayerController::HandleInputEvent_ModulePlacementRotationCW()
{
	if (IsModuleBeingPlaced())
	{
		
		_moduleBeingPlaced->RotateModule(false);
	}
}

void AVBS_PlayerController::HandleInputEvent_ModulePlacementRotationCCW()
{
	if (IsModuleBeingPlaced())
	{
		_moduleBeingPlaced->RotateModule(true);
	}
}

void AVBS_PlayerController::HandleInputAxis_Turn(float value_)
{
	if(IsInputKeyDown(EKeys::MiddleMouseButton))
		AddYawInput(value_);
}

void AVBS_PlayerController::HandleInputAxis_LookUp(float value_)
{
	if (IsInputKeyDown(EKeys::MiddleMouseButton))
		AddPitchInput(value_);
}

void AVBS_PlayerController::HandleActionMenuItemSelection(EVBS_MenuItems selectedItem_)
{
	if (selectedItem_ == EVBS_MenuItems::MI_Action_NewModule)
	{
		if (_menu_NewModuleSelection)
		{
			float x = 0, y = 0;
			FVector2D pos = FVector2D::ZeroVector;
			if (GetMousePosition(x, y))
				pos = FVector2D(x, y);
			_menu_NewModuleSelection->ShowMenu(pos);
		}
	}
}

void AVBS_PlayerController::HandleNewModuleMenuItemSelection(EVBS_MenuItems selectedItem_)
{
	switch (selectedItem_)
	{
	case EVBS_MenuItems::MI_NewModule_BasicI:
		SpawnModule(*ModuleDescriptorMap.Find(EVBS_ModuleTypes::MT_BasicI), true);
		break;

	case EVBS_MenuItems::MI_NewModule_BasicT:
		SpawnModule(*ModuleDescriptorMap.Find(EVBS_ModuleTypes::MT_BasicT), true);
		break;
	case EVBS_MenuItems::MI_NewModule_BasicX:
		SpawnModule(*ModuleDescriptorMap.Find(EVBS_ModuleTypes::MT_BasicX), true);
		break;

	default:
		// do nothing..
		break;
	}
}

void AVBS_PlayerController::HandleActionMenuPopup()
{
	if (IsModuleBeingPlaced())
		return; // dont show menu if placing item (later will deal with cancel placement)

	if (_menu_ActionSelection)
	{
		float x = 0, y = 0;
		FVector2D pos = FVector2D::ZeroVector;
		if (GetMousePosition(x, y))
			pos = FVector2D(x, y);

		_menu_ActionSelection->ShowMenu(pos);
	}
}

void AVBS_PlayerController::HandleModulePlacementComplete()
{
	if (IsModuleBeingPlaced())
	{
		if (_moduleBeingPlaced->_CurrentCollisionState == EVBS_ModuleCollisionState::MCS_Connected)
		{
			AVBS_ModuleActor* moduleX = _moduleBeingPlaced;
			StopPlacingCurrentModule();
			moduleX->ResetModuleMaterial();
		}
		
	}
}

void AVBS_PlayerController::HandleModulePlacementCancel()
{
	if (IsModuleBeingPlaced())
	{
		AVBS_ModuleActor* moduleBeingPlaced = _moduleBeingPlaced;
		StopPlacingCurrentModule();
		moduleBeingPlaced->Destroy();
	}
}

void AVBS_PlayerController::HandleShowHideSnapMarkers()
{
	ShouldShowSnapMarkers = !ShouldShowSnapMarkers;

	for (TCheckedPointerIterator<AVBS_ModuleActor*, int32> iter = _allModules.begin();
		iter != _allModules.end();
		iter.operator++())
	{
		(*iter)->ShowHideSnapMarkers(ShouldShowSnapMarkers);
	}
}

void AVBS_PlayerController::HandleShowHideConnectorMarkers()
{
	ShouldShowConnectionMarkers = !ShouldShowConnectionMarkers;
	for (TCheckedPointerIterator<AVBS_ModuleActor*, int32> iter = _allModules.begin();
		iter != _allModules.end();
		iter.operator++())
	{
		(*iter)->ShowHideConnectorMarkers(ShouldShowConnectionMarkers);
	}
}

