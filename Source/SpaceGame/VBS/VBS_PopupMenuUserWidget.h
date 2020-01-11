// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VBS_PopupMenuUserWidget.generated.h"

UENUM(BlueprintType)
enum class EVBS_PopupMenuButtonType : uint8
{
	PMBT_AddModule UMETA(DisplayName = "Add New Module")
};

/**
 * User widget for popup menu of the VBS
 */
UCLASS()
class SPACEGAME_API UVBS_PopupMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

private:

public:

	UFUNCTION(BlueprintCallable)
	void ProcessMenuButtonClick(class UButton* button_);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UButton*> MenuButtons;
	//TODO: add list of buttons to shohow/hide....
	
};
