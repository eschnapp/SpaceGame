// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VBS_PopupMenuBase.h"
#include "VBS_ModuleActor.h"
#include "VBS_PlayerController.generated.h"

// declate a log category for the player controller
DECLARE_LOG_CATEGORY_EXTERN(LogGame_PlayerController, Log, All)


/**
 * VBS Player Controller - used to control the entire level's state including all the vessel modules that are being constructed as well 
 * as detecting correct collision and overlap needs for placement and handling the various inputs from keyboard and mouse while placing
 * modules.
 */
UCLASS()
class SPACEGAME_API AVBS_PlayerController : public APlayerController
{
	GENERATED_BODY()

	// Handle input event for incrementing distance of module being placed from the character 
	// TODO: change to axis mapping instead of two action mappings!!!
	UFUNCTION()
	void HandleInputEvent_ModulePlacementDistanceINC();

	// Handle input event for decrementing distance of module being placed from the character 
	// TODO: change to axis mapping instead of two action mappings!!!
	UFUNCTION()
	void HandleInputEvent_ModulePlacementDistanceDEC();

	// Handle input event for rotating the module being placed Clockwise
	// TODO: change to axis mapping instead of two action mappings!!!
	UFUNCTION()
	void HandleInputEvent_ModulePlacementRotationCW();

	// Handle input event for rotating the module being placed Counter Clockwise 
	// TODO: change to axis mapping instead of two action mappings!!!
	UFUNCTION()
	void HandleInputEvent_ModulePlacementRotationCCW();

	// Handle input axis change for turning the character on the horizontal (right ascention) pane
	UFUNCTION()
	void HandleInputAxis_Turn(float value_);

	// Handle input axis change for turning the character on the vertical (declination) pane
	UFUNCTION()
	void HandleInputAxis_LookUp(float value_);

	// Handle menu popup event (only valid for the main popup menu )
	UFUNCTION()
	void HandleActionMenuPopup();

	// Handle module placing event
	UFUNCTION()
	void HandleModulePlacementComplete();

	// Handle cancel module placement
	UFUNCTION()
	void HandleModulePlacementCancel();

	// Handler for action (main) menu
	UFUNCTION()
	void HandleActionMenuItemSelection(EVBS_MenuItems selectedItem_);

	// Handler for new module selection menu
	UFUNCTION()
	void HandleNewModuleMenuItemSelection(EVBS_MenuItems selectedItem_);

	// Handler for showing/hiding the snap (overlap) markers (see comments in VBS_ModuleActor class about overlap and connection snaps)
	// this might be DEV ONLY thing.. not sure im ok to let users see the snap points :/
	UFUNCTION()
	void HandleShowHideSnapMarkers();

	// Handler for showing/hiding the connector markers (see comments in VBS_ModuleActor class about overlap and connection snaps)
	// this might be DEV ONLY thing.. not sure im ok to let users see the snap points :/
	UFUNCTION()
	void HandleShowHideConnectorMarkers();

	// perform a general collision check against other modules in the VBS... can yield one of 3 possible results:
	// 1. Detached - module is not overlapping or connected to any other module in the scene
	// 2. Connected - module is not overlapping but is connected to another one (or more) modules in the scene
	// 3. Overlapping - module is currently overlapping with another module in the scene
	UFUNCTION()
	EVBS_ModuleCollisionState PerformGridCollisionCheck(AVBS_ModuleActor* module_);

public:
	AVBS_PlayerController();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when level is started
	virtual void BeginPlay() override;

	// Setup the input controller's bindings for any hardware action (mouse/keyboard events are translated to input/axis events then captured and handled)
	virtual void SetupInputComponent() override;

	// Returns true if a module is currently being placed or false otherwise.
	UFUNCTION(Category = "Module Placement", BlueprintCallable)
	bool IsModuleBeingPlaced();

	// Start to place a given module.
	UFUNCTION(Category = "Module Placement", BlueprintCallable)
	void StartPlacingModule(class AVBS_ModuleActor* moduleToPlace_);

	// Stop placing the current module being placed (NOTE: overrides collision and overlap detection if used directly!!)
	UFUNCTION(Category = "Module Placement", BlueprintCallable)
	void StopPlacingCurrentModule();

	// Spawn a new module into the scene and optionally start auto-placing it
	UFUNCTION(Category = "Module Placement", BlueprintCallable)
	AVBS_ModuleActor* SpawnModule(FVBS_ModuleDescriptor descriptor_, bool bAutoStartPlacing = false);

private:

	class AVBS_ModuleActor*		_moduleBeingPlaced;				// module being currently placed 
	class AVBS_ModuleActor*		_rootVBSModule;					// root module of the vessel (capsule, maybe later smth else?)
	float						_placingDistanceFromPlayer;		// distance in CM from the player of the module being placed
	class UVBS_PopupMenuBase*	_menu_ActionSelection;			// (main) action selection popup menu
	class UVBS_PopupMenuBase*	_menu_NewModuleSelection;		// new module popup menu
	FInputModeUIOnly			_inputModeUIOnly;				// pointer to defualt UI Only input mode (no game mouse events, only UI menus)
	FInputModeGameAndUI			_inputModeGameAndUI;			// pointer to default GameAnbUI input mode 
	TArray<AVBS_ModuleActor*>	_allModules;					// array containing all modules in the scene (including root)

public:

	// widget for popup (action) menu - should be assigned by overriding clas or level BP / game constructor
	UPROPERTY(Category = "User Menus", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UVBS_PopupMenuBase> PopupMenuUserWidget;

	// widget for new module menu - should be assigned by overriding clas or level BP / game constructor
	UPROPERTY(Category = "User Menus", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UVBS_PopupMenuBase> NewModuleSelection;

	// descriptor struct for the root module of the vessel
	UPROPERTY(Category = "General", EditAnywhere, BlueprintReadWrite)
	FVBS_ModuleDescriptor RootModuleDescriptor;

	// Size for grid snapping (1000.0 cm by default)
	UPROPERTY(Category = "Module Placement", EditAnywhere, BlueprintReadWrite)
	float _snapGridSize;

	// Map of module descriptors for auto-spawning and placement. should 1 entry for each type of module that
	// can be constructed in the VBS
	UPROPERTY(Category = "Module Placement", EditAnywhere, BlueprintReadWrite)
	TMap<EVBS_ModuleTypes, FVBS_ModuleDescriptor> ModuleDescriptorMap;

	// Material instance to use when overlapping with another module
	UPROPERTY(Category = "Module Placement", EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* ModuleOverlapPlacementMaterial;

	// Material instance to use when connected with another module
	UPROPERTY(Category = "Module Placement", EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* ModuleConnectionPlacementMaterial;

	// Material instance to use for markers (overlap/snap and connection)
	UPROPERTY(Category = "Module Placement", EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* ModuleMarkerMaterial;

	// static mesh to use for snap markers
	UPROPERTY(Category = "Module Placement", EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ModuleSnapMarkerMesh;

	// static mesh to use for connection markers
	UPROPERTY(Category = "Module Placement", EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ModuleConnectMarkerMesh;

	// shuld show or hide the snapping (overlap) markers
	UPROPERTY(Category = "Module Placement", VisibleAnywhere, BlueprintReadWrite)
	bool ShouldShowSnapMarkers;

	// shuld show or hide the connection markers
	UPROPERTY(Category = "Module Placement", VisibleAnywhere, BlueprintReadWrite)
	bool ShouldShowConnectionMarkers;
	   	
};
