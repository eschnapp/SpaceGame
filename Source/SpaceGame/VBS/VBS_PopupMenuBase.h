// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "VBS_PopupMenuBase.generated.h"

/**
 * Enum to define all possible popup menu items, maybe can find a better solution later
 * but for now this will work fine... This value will be passed into the Event handler of each menu that is created
 * so logic can be placed around the result of the menu selection.
 */
UENUM()
enum class EVBS_MenuItems : uint8
{
	MI_Action_NewModule		UMETA(DisplayName = "Action, New Module"),
	MI_NewModule_None		UMETA(DisplayName = "NewModule, None"),
	MI_NewModule_BasicI		UMETA(DisplayName = "NewModule, Basic I Module"),
	MI_NewModule_BasicL		UMETA(DisplayName = "NewModule, Basic L Module"),
	MI_NewModule_BasicT		UMETA(DisplayName = "NewModule, Basic T Module"),
	MI_NewModule_BasicX		UMETA(DisplayName = "NewModule, Basic X Module"),
	MI_NewModule_Capsule	UMETA(DisplayName = "NewModule, Capsule Module"),
	MI_Cancel				UMETA(DisplayName = "Cancel Current Action")
};

// Declate a 1 param multicast delegate to handle the OnMenuSelect event that passes in a SelectedItem result...
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVBS_PopupMenuItemSelected, EVBS_MenuItems, SelectedItem);


/**
 * Generic base class to handle popup menu widgets. Expected to be overridden by either BP or code implementation 
 * for specific menues.
 */
UCLASS()
class SPACEGAME_API UVBS_PopupMenuBase : public UUserWidget
{
	GENERATED_BODY()

public:

	// Show Menu on viewport at a given 2D locaiton (its NOT modal!!)
	UFUNCTION(BlueprintCallable)
	void ShowMenu(FVector2D PositionOnViewport = FVector2D::ZeroVector);

	// Function to handle the menu button event itself... 
	// NOTE: this function MUST be called by the implementation of EACH button's OnClick handler
	// either within the BP's construction script or within the C++ delegates.
	UFUNCTION(BlueprintCallable)
	void HandleMenuButtonEvent(EVBS_MenuItems menuItem_);

	// Delegate to handle the menu's selection result. will be called after a button in the menu was clicked and provide
	// the EVBS_MenuItems of the selected button.
	UPROPERTY(BlueprintAssignable)
	FVBS_PopupMenuItemSelected OnMenuItemSelected;

	// Result of the selection if handler is not registered. (unlikely to be ever needed)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EVBS_MenuItems SelectedMenuItem;
};
