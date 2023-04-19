#include "Model.h"
#include "WorldTransform.h"

Player::Player() {}

Player::~Player() {}

class Player {
public:
	Player();
	~Player();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model,uint32_t textureHandele);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

};
