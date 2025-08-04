#pragma once

#include "Core.h"
#include "Math\Vector2.h"
#include "Math/Color.h"
#include "RTTI.h"

// ȭ�鿡 �׷��� ���� ���� ������ (�������� ��)
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

// ���� ���� ����
class Level;

class ENGINE_API Actor : public RTTI
{
	// ���������� ���Ϳ� ������ �� �ֵ��� friend ����
	friend class Level;

	RTTI_DECLARATIONS(Actor, RTTI)

public:
	Actor(
		const char* image = "",
		Color color = Color::White,
		const Vector2& position = Vector2::Zero
	);

	virtual ~Actor();

	// �̺�Ʈ �Լ�
	virtual void BeginPlay();
	virtual void Tick(float deltaTime);
	virtual void Render();

	// BeginPlay ȣ�� ���� Ȯ��
	inline bool HasBeganPlay() const { return hasBeganPlay; }

	// ��ġ ���� �� �� �д� �Լ�
	void SetPosition(const Vector2& newPosition);
	Vector2 Position() const;

	// Sorting Order ����
	void SetSortingOrder(SortingOrder sortingOrder);
	SortingOrder GetSortingOrder() const;

	// ���ʽ� ����(Getter/Setter)
	void SetOwner(Level* newOwner);
	Level* GetOwner() const;

	// ��ü ���� �Լ�
	void Destroy();

	// ���� ���� ��û �Լ�
	void QuitGame();

private:
	// ��ü�� ��ġ
	Vector2 position;

	// �׸� ��
	char* image = nullptr;

	// ���ڿ� ����
	int width = 0;

	// �ؽ�Ʈ ���� ��
	Color color;

	// BeginPlay ȣ���� �Ǿ����� Ȯ��
	bool hasBeganPlay = false;

	// ���� ����
	SortingOrder sortingOrder = SortingOrder::None;

	// ���Ͱ� ���� ��û�Ǿ����� Ȯ���ϴ� ����
	bool isExpired = false;

	// ���� ����(���ʽ�)
	Level* owner = nullptr;
};

