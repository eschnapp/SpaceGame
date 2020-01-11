// Fill out your copyright notice in the Description page of Project Settings.


#include "VBS_PlayerController.h"
#include "VBS_ModuleActor.h"
#include "VBS_PopupMenuUserWidget.h"
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY(LogGame_PlayerController)

AVBS_PlayerController::AVBS_PlayerController()
	: _moduleBeingPlaced(nullptr)			// no module placed by default
	, _placingDistanceFromPlayer(500.0)		// 5.00 meter from the camera
	, _popupMenu( nullptr )
{
	UE_LOG(LogGame_PlayerController, Log, TEXT("Creating new VBS Player Controller"));
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void AVBS_PlayerController::SetupInputComponent()
{
	// VBS_ModulePlacementRotationCCW
	Super::SetupInputComponent();
	InputComponent->BindAction("VBS_ModulePlacementDistanceINC",	IE_Pressed, this, &AVBS_PlayerController::HandleInputEvent_ModulePlacementDistanceINC);
	InputComponent->BindAction("VBS_ModulePlacementDistanceINC",	IE_Pressed, this, &AVBS_PlayerController::HandleInputEvent_ModulePlacementDistanceINC);
	InputComponent->BindAction("VBS_ModulePlacementRotationCW",		IE_Pressed, this, &AVBS_PlayerController::HandleInputEvent_ModulePlacementRotationCW);
	InputComponent->BindAction("VBS_ModulePlacementRotationCCW",	IE_Pressed, this, &AVBS_PlayerController::HandleInputEvent_ModulePlacementRotationCCW);
	InputComponent->BindAction("VBS_ActionPopupMenu",				IE_Pressed, this, &AVBS_PlayerController::HandleInputEvent_ActionPopupMenu);
}

void AVBS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// setup the popup menu...
	if (!PopupMenuUserWidget)
	{
		UE_LOG(LogGame_PlayerController, Error, TEXT("No popup menu widget selected!"));
		return;
	}

	_popupMenu = CreateWidget<UVBS_PopupMenuUserWidget>(this, PopupMenuUserWidget);
	_popupMenu->SetVisibility(ESlateVisibility::Hidden);
	_popupMenu->AddToViewport();
}

// Called every frame
void AVBS_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_moduleBeingPlaced)
	{	//  something is being placed... handle it.
		FVector mouseWorldLocation, mouseWorldDirection;
		if (!DeprojectMousePositionToWorld(mouseWorldLocation, mouseWorldDirection))
		{
			UE_LOG(LogGame_PlayerController, Error, TEXT("Failed to deproject mouse position to world space!"));
		}
		else
		{
			FVector trxLocation = (mouseWorldDirection * _placingDistanceFromPlayer) + mouseWorldLocation;
			float rotationZ = (uint8)(_moduleBeingPlaced->_currentRotationPhase) * 90;
			FTransform newTrx = FTransform(FVector(0, 0, rotationZ).Rotation(), trxLocation, FVector(1, 1, 1));
			_moduleBeingPlaced->SetActorTransform(newTrx);
		}
	}
		

}

bool AVBS_PlayerController::IsModuleBeingPlaced()
{
	if (!_moduleBeingPlaced)
		return false;

	return _moduleBeingPlaced->_isBeingPlaced;
}

void AVBS_PlayerController::StartPlacingModule(AVBS_ModuleActor* moduleToPlace_)
{
	if (!moduleToPlace_)
		return;
	moduleToPlace_->_isBeingPlaced = true;
	_moduleBeingPlaced = moduleToPlace_;
}

void AVBS_PlayerController::StopPlacingCurrentModule()
{
	if (!_moduleBeingPlaced)
		return;

	_moduleBeingPlaced->_isBeingPlaced = false;
	_moduleBeingPlaced = nullptr;
}

void AVBS_PlayerController::SpawnModuleForPlacing(UStaticMesh* moduleStaticMesh_)
{
	AVBS_ModuleActor* newModule = GetWorld()->SpawnActor<AVBS_ModuleActor>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (!newModule)
	{
		UE_LOG(LogGame_PlayerController, Warning, TEXT("Failed to spawn new module actor!"));
		return;
	}

	newModule->_moduleStaticMesh = moduleStaticMesh_;
	newModule->UpdateCurrentMesh();
	StartPlacingModule(newModule);
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

void AVBS_PlayerController::HandleInputEvent_ActionPopupMenu()
{
	if (!_popupMenu)
	{
		UE_LOG(LogGame_PlayerController, Error, TEXT("No popup menu created!"));
		return;
	}

	if (_popupMenu->IsVisible())
	{
		float mX = 0;
		float mY = 0;
		_popupMenu->SetVisibility(ESlateVisibility::Hidden);
		_popupMenu->SetPositionInViewport(FVector2D(mX, mY));
	}
	else
	{
		float mX = 0;
		float mY = 0;
		if (!GetMousePosition(mX, mY))
		{
			UE_LOG(LogGame_PlayerController, Warning, TEXT("Failed to get mouse position"));
		}
		_popupMenu->SetPositionInViewport(FVector2D(mX, mY));
		_popupMenu->SetVisibility(ESlateVisibility::Visible);
	}


}