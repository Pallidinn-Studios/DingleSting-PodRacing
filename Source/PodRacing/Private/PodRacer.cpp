// Fill out your copyright notice in the Description page of Project Settings.


#include "PodRacer.h"
#include "HoverComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
APodRacer::APodRacer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	PodMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pod"));
	RootComponent = PodMesh;

	//NewMesh->SetSkeletalMesh(PodMesh);
}

// Called when the game starts or when spawned
void APodRacer::BeginPlay()
{
	Super::BeginPlay();
	
}

void APodRacer::SetDirectionForce(UHoverComponent* HoverComp, FRotator Direction, float Force) {
	HoverComp->SetRelativeRotation(Direction);
	HoverComp->HoverForce = Force;
}


// Called every frame
void APodRacer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PodSpeed = GetVelocity().Length() * 0.036;
}

// Called to bind functionality to input
void APodRacer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

