// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VEModule.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(SpaceGame_VEModule, Log, All);

UCLASS()
class SPACEGAME_API AVEModule : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVEModule();

	UPROPERTY(Category = "Static Mesh", EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* _ModuleStaticMeshComponent;

	UPROPERTY(Category = "Static Mesh", EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* _ModuleDefaultStaticMesh;

	UPROPERTY(Category = "Grid", VisibleAnywhere, BlueprintReadOnly)
	FIntVector _GridCoordinates;

private:
	class AVEGridManager* _gridManager;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Category = "Mouse Handlers", BlueprintCallable)
	void InitializeModule(class AVEGridManager* gridManager_, FIntVector gridCoords_);

	UFUNCTION(Category = "Mouse Handlers")
	void HandleMouseClick(AActor* actor_, FKey mouseButtonKey_);
	
	UFUNCTION(Category = "Mouse Handlers")
	void HandleMouseOver(AActor* actor_);

	UFUNCTION(Category = "Mouse Handlers")
	void HandleMouseOut(AActor* actor_);

};
