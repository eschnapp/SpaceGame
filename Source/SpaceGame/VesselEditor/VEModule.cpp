// Fill out your copyright notice in the Description page of Project Settings.


#include "VEModule.h"
#include "VEGridManager.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY(SpaceGame_VEModule);

// Sets default values
AVEModule::AVEModule()
{
	UE_LOG(SpaceGame_VEModule, Log, TEXT("Constructing a new VEModule"));

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// register the mouse handling event callbacks
	OnClicked.AddUniqueDynamic(this, &AVEModule::HandleMouseClick);
	OnBeginCursorOver.AddUniqueDynamic(this, &AVEModule::HandleMouseOver);
	OnEndCursorOver.AddUniqueDynamic(this, &AVEModule::HandleMouseOut);

	// create the default static mesh component
	_ModuleStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ModuleStaticMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/VesselEditor/DefaultModuleMesh.DefaultModuleMesh'"));
	UStaticMesh* Asset = MeshAsset.Object;
	_ModuleStaticMeshComponent->SetStaticMesh(Asset);


}

// Called when the game starts or when spawned
void AVEModule::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVEModule::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVEModule::InitializeModule(class AVEGridManager* gridManager_, FIntVector gridCoords_)
{
	_gridManager = gridManager_;
	_GridCoordinates = gridCoords_;
}

void AVEModule::HandleMouseClick(AActor* actor_, FKey mouseButtonKey_)
{
	UE_LOG(SpaceGame_VEModule, Log, TEXT("Module OnClick Handler Called"));
}

void AVEModule::HandleMouseOver(AActor* actor_)
{
	UE_LOG(SpaceGame_VEModule, Log, TEXT("Module OnMouseOver Handler Called"));
}


void AVEModule::HandleMouseOut(AActor* actor_)
{
	UE_LOG(SpaceGame_VEModule, Log, TEXT("Module OnMouseOut Handler Called"));
}

