// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Track.generated.h"

UCLASS()
class PODRACING_API ATrack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UBoxComponent* Finish;
};