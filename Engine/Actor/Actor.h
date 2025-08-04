#pragma once

#include "Core.h"
#include "Math\Vector2.h"
#include "Math/Color.h"
#include "RTTI.h"

// 화면에 그려질 정렬 순서 열거형 (높을수록 위)
enum class SortingOrder : unsigned __int8
{
	None = 0,
	Score,
	Wall,
	Item,
	Enemy,
	Dark,
	Player,
};

// 레벨 전방 선언
class Level;

class ENGINE_API Actor : public RTTI
{
	// 레벨에서는 액터에 접근할 수 있도록 friend 선언
	friend class Level;

	RTTI_DECLARATIONS(Actor, RTTI)

public:
	Actor(
		const char* image = "",
		Color color = Color::White,
		const Vector2& position = Vector2::Zero
	);

	virtual ~Actor();

	// 이벤트 함수
	virtual void BeginPlay();
	virtual void Tick(float deltaTime);
	virtual void Render();

	// BeginPlay 호출 여부 확인
	inline bool HasBeganPlay() const { return hasBeganPlay; }

	// 위치 설정 및 값 읽는 함수
	void SetPosition(const Vector2& newPosition);
	Vector2 Position() const;

	// Sorting Order 설정
	void SetSortingOrder(SortingOrder sortingOrder);
	SortingOrder GetSortingOrder() const;

	// 오너십 설정(Getter/Setter)
	void SetOwner(Level* newOwner);
	Level* GetOwner() const;

	// 객체 삭제 함수
	void Destroy();

	// 게임 종료 요청 함수
	void QuitGame();

private:
	// 개체의 위치
	Vector2 position;

	// 그릴 값
	char* image = nullptr;

	// 문자열 길이
	int width = 0;

	// 텍스트 색상 값
	Color color;

	// BeginPlay 호출이 되었는지 확인
	bool hasBeganPlay = false;

	// 정렬 순서
	SortingOrder sortingOrder = SortingOrder::None;

	// 액터가 삭제 요청되었는지 확인하는 변수
	bool isExpired = false;

	// 소유 레벨(오너십)
	Level* owner = nullptr;
};

