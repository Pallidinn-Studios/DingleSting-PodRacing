// Fill out your copyright notice in the Description page of Project Settings.


#include "Track.h"

// Sets default values
ATrack::ATrack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Finish = CreateDefaultSubobject<UBoxComponent>("Finish line");
	RootComponent = Finish;

}

// Called when the game starts or when spawned
void ATrack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Finish->OnComponentBeginOverlap.AddDynamic(this, &ATrack::OverlapEvent);

}

void ATrack::OverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor) {
	
}


