// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VBS_Character.generated.h"

/* Default Character Base class for movement/camera etc... */
UCLASS()
class SPACEGAME_API AVBS_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVBS_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Control the movement on the Forward<->Backwards axis
	UFUNCTION(Category = "Movement", BlueprintCallable)
	void MoveForward(float Value);

	// Control the movement on the Left-Right axis
	UFUNCTION(Category = "Movement", BlueprintCallable)
	void MoveRight(float Value);

	// Control the movement on the Left-Right axis
	UFUNCTION(Category = "Movement", BlueprintCallable)
	void MoveUp(float Value);
};
