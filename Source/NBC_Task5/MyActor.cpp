#include "MyActor.h"

AMyActor::AMyActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	start = FVector(0,0,0);
	evCnt = 0;
	totDist = 0;
	deltaSum = 0;
}

void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	//초기 위치로
	SetActorLocation(start);

	//10회 이동할 좌표
	int moveCount = 10;
	FVector prevPos = start;
	while (moveCount > 0)
	{
		//X나 Y중 반드시 1개 이상이 1
		int newX = Step();
		int newY = newX == 0 ? 1 : Step();

		FVector newPos = FVector(newX, newY, 0) + prevPos;
		moveQueue.Enqueue(newPos);
		prevPos = newPos;
		
		moveCount--;
	}

	UE_LOG(LogTemp, Display, TEXT("MyActor Start"));
	UE_LOG(LogTemp, Display, TEXT("===================="));
}

void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//0.5초마다 실행
	if (deltaSum < 0.5f)
	{
		deltaSum += DeltaTime;
		return;
	}
	deltaSum -= 0.5f;

	if (moveQueue.IsEmpty())//이동 종료
	{
		//이동 결산
		const FVector currentPos = GetActorLocation();
		UE_LOG(LogTemp, Display, TEXT("===================="));
		UE_LOG(LogTemp, Warning, TEXT("최종 좌표 : (%f, %f)"), currentPos.X, currentPos.Y);
		UE_LOG(LogTemp, Warning, TEXT("총 이동 거리 : %f"), totDist);
		UE_LOG(LogTemp, Warning, TEXT("이벤트 발생 횟수 : %d"), evCnt);
		UE_LOG(LogTemp, Display, TEXT("===================="));
		
		//이제 Tick을 멈춘다.
		SetActorTickEnabled(false);
	}
	else//이동
	{
		Move();

		//이벤트 발생
		int eventValue = CreateEvent();
		if (eventValue != 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Event Triggered : %d"), eventValue);
			evCnt++;
		}
	}
}

float AMyActor::Distance(FVector& first, FVector& second)
{
	float dx = first.X - second.X;
	float dy = first.Y - second.Y;
	
	return FMath::Sqrt(dx * dx +  dy * dy);
}

void AMyActor::Move()
{
	//이동할 좌표
	FVector currentPos = GetActorLocation();
	FVector newPos;
	moveQueue.Dequeue(newPos);

	//두 좌표의 거리
	float distance = Distance(currentPos, newPos);
	totDist += distance;

	//로그
	UE_LOG(LogTemp, Warning, TEXT("좌표 이동 : (%f, %f)"), newPos.X, newPos.Y);
	UE_LOG(LogTemp, Display, TEXT("이동거리 : %f"), distance);

	//이동
	SetActorLocation(newPos);
}

int AMyActor::Step()
{
	return FMath::RandRange(0, 1);
}

int AMyActor::CreateEvent()
{
	int randValue = FMath::RandRange(0, 1);
	if (randValue == 0)
		return 0;

	return FMath::RandRange(1, 10);
}

