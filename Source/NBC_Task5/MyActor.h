#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class NBC_TASK5_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	//이전 위치와 현재 위치의 거리
	float Distance(FVector& first, FVector& second);

	//액터의 위치 이동
	void Move();

	//이동을 위해 0 또는 1의 값 반환
	int Step();

	//이벤트 발생
	int CreateEvent();

private:
	FVector start;
	TQueue<FVector> moveQueue;
	int evCnt;
	float totDist;
	float deltaSum;
};
