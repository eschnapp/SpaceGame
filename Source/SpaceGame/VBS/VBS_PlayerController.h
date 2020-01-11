// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VBS_PlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGame_PlayerController, Log, All)

/**
 * 
 */
UCLASS()
class SPACEGAME_API AVBS_PlayerController : public APlayerController
{
	GENERATED_BODY()

	void HandleInputEvent_ModulePlacementDistanceINC();
	void HandleInputEvent_ModulePlacementDistanceDEC();
	void HandleInputEvent_ModulePlacementRotationCW();
	void HandleInputEvent_ModulePlacementRotationCCW();
	void HandleInputEvent_ActionPopupMenu();

public:
	AVBS_PlayerController();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	bool IsModuleBeingPlaced();
	void StartPlacingModule(class AVBS_ModuleActor* moduleToPlace_);
	void StopPlacingCurrentModule();
	void SpawnModuleForPlacing(UStaticMesh* moduleStaticMesh_);

private:

	class AVBS_ModuleActor* _moduleBeingPlaced;
	float _placingDistanceFromPlayer;
	class UVBS_PopupMenuUserWidget* _popupMenu;

public:

	UPROPERTY(Category = "General", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UVBS_PopupMenuUserWidget> PopupMenuUserWidget;
	
};
