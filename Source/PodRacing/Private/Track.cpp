// Fill out your copyright notice in the Description page of Project Settings.


#include "Track.h"

#include "PodRacer.h"

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

	Finish->OnComponentBeginOverlap.AddDynamic(this, &ATrack::OnOverlapBegin);
	Finish->OnComponentEndOverlap.AddDynamic(this, &ATrack::OnOverlapEnd);

}
void ATrack::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	CanOverlap = true;
}

void ATrack::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (Cast<APodRacer>(OtherActor) && CanOverlap) {
		CanOverlap = false;

		Cast<APodRacer>(OtherActor)->AddLapTime();
		//Cast<APodRacer>(OtherActor)->LapTimes.Add(10.1f);
	}
}


