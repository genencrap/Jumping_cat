#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include <math.h>
#include <vector>
#include <time.h>
#include <stdlib.h>

using namespace std;
using namespace sf;

class Score;

void play_sound(const string& filename);
void delay_ms(int ms);

//고양이 클래스
class Cat
{
private:
	Vector2f position;
	float startPosX = 0;
	float startPosY = 0;
	Vector2f velocity;
	float gravity;
	bool isJumping;
	bool isFalling;
	Texture texture;
	Sprite sprite;

	// 떨어지는 동작에 대한 변수들
	float fallSpeed;
	float rotationSpeed;
	float rotation;
public:
	Cat(float x, float y)
		: position(x, y), velocity(0.f, 0.f), gravity(500.f), isJumping(false), isFalling(false)
	{
		texture.loadFromFile("./Data/Image/cat.png");
		Vector2u catTextureSize = texture.getSize();
		sprite.setTexture(texture);
		sprite.setScale((float)159.7 / catTextureSize.x, (float)127.7 / catTextureSize.y);
		sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2); //가운데를 중심점으로 설정
		sprite.setPosition(position);
	}

	Vector2f getPosition() {
		return sprite.getPosition();
	}
	int getPositionX() {
		return (int)sprite.getPosition().x;
	}
	int getPositionY() {
		return (int)sprite.getPosition().y;
	}
	void setPosition(float x, float y)
	{
		position.x = x;
		position.y = y;

		sprite.setPosition(position);
	}

	float getStartPositionX() {
		return startPosX;
	}
	float getStartPositionY() {
		return startPosY;
	}
	void setStartPosition(float x, float y) {
		startPosX = x;
		startPosY = y;
	}

	Vector2f getVelocity() {
		return velocity;
	}
	void setVelocity(Vector2f v) {
		velocity = v;
	}
	void setVelocityX(float velX) {
		velocity.x = velX;
	}
	void setVelocityY(float velY) {
		velocity.y = velY;
	}

	float getTextureSizeX(){
		return (float)texture.getSize().x;
	}
	float getTextureSizeY(){
		return (float)texture.getSize().y;
	}

	float getGravity(){
		return gravity;
	}

	void setIsJumping(bool Jumping){
		isJumping = Jumping;
	}

	bool getIsFalling() {
		return isFalling;
	}

	void setRotation(int a){
		sprite.setRotation((float)a);
	}

	Vector2f getScale() {
		return sprite.getScale();
	}
	void setScale(float factorX, float factorY){
		sprite.setScale((float)factorX, (float)factorY);
	}

	void changeImage(string path){
		texture.loadFromFile(path);
	}

	void jump(Vector2f jumpVelocity)
	{
		velocity = jumpVelocity;
		isJumping = true;
		isFalling = false;
	}

	void startFalling(float speed, float rotateSpeed)
	{
		fallSpeed = speed;
		rotationSpeed = rotateSpeed;
		isJumping = false;
		isFalling = true;
		velocity = Vector2f(0, speed);
		rotation = 0.f;
	}

	void rotateAndFall(float rotateSpeed, float fallSpeed, float deltaTime)
	{
		sprite.setRotation(sprite.getRotation() + rotateSpeed * deltaTime);
		sprite.move(0.f, fallSpeed * deltaTime);
	}

	void update(float deltaTime)
	{
		if (isJumping)
		{
			// 위치 업데이트
			position += velocity * deltaTime;
			velocity.y += gravity * deltaTime;

			// 화면 경계 체크
			if (position.x - sprite.getGlobalBounds().width / 2 <= 0.f) //왼쪽 경계에 닿으면
			{
				velocity.x = -velocity.x;
				if(sprite.getScale().x < 0) //고양이가 왼쪽을 향해 있으면
					sprite.setScale(-sprite.getScale().x, sprite.getScale().y); // 이미지를 좌우 반전
			}
			else if (position.x + sprite.getGlobalBounds().width / 2 >= 1940.f) //오른쪽 경계에 닿으면
			{
				velocity.x = -velocity.x;
				if (sprite.getScale().x > 0) //고양이가 오른쪽을 향해 있으면
					sprite.setScale(-sprite.getScale().x, sprite.getScale().y); // 이미지를 좌우 반전
			}

			if (position.y - sprite.getGlobalBounds().height / 2 <= 0.f || position.y + sprite.getGlobalBounds().height / 2 >= 540.f)
			{
				velocity.y = -(velocity.y + 10);
			}

			sprite.setPosition(position);

			if (position.y >= 600.f) // 고양이가 바닥에 닿으면
			{
				isFalling = false;
				velocity = Vector2f(0.f, 0.f);
				sprite.setRotation(0.f);
			}
		}
		else if (isFalling)
		{
			rotateAndFall(rotationSpeed, fallSpeed, deltaTime);
		}
	}

	void draw(RenderWindow& window)
	{
		// 고양이 그리기
		window.draw(sprite);
	}

	FloatRect getBounds() const
	{
		return sprite.getGlobalBounds();
	}
};

//포물선 클래스
class Arc
{
	//각도와 파워에 따라 포물선 조절(*파워게이지 따로 안 만들어도 됨)
private:
	CircleShape arc;
	//위치좌표, 크기, 각도
	float posX;
	float posY;
	float startArcPosX = 0;
	float startArcPosY = 0;
	float radius;

	Vector2f velocity;
	Vector2f startArcVelocity;
	float velocityX;
	float velocityY;
	float acceleration;

	Color color;

public:
	//생성자, 소멸자
	Arc(float x, float y, float r)
	{
		posX = x;
		posY = y;
		radius = r;

		velocityX = 0;
		velocityY = 0;
		velocity.x = velocityX;
		velocity.y = velocityY;
		acceleration = 0;

		arc.setPosition(posX, posY);
		arc.setRadius(radius);
		arc.setFillColor(color.Red);
	}

	//
	CircleShape getArc()
	{
		return arc;
	}

	//위치좌표, 크기, 각도 setter/getter
	void setArcPos(float x, float y)
	{
		posX = x;
		posY = y;
	}
	float getArcPosX()
	{
		return posX;
	}
	float getArcPosY()
	{
		return posY;
	}
	void setRadius(float r)
	{
		radius = r;
	}
	float getRadius()
	{
		return radius;
	}

	void setArcVelocity(Vector2f jumpVelocity)
	{
		velocity = jumpVelocity;
	}
	Vector2f getArcVelocity()
	{
		return velocity;
	}

	void setArcVelocityX(float v_x)
	{
		velocityX = v_x;
		velocity.x = velocityX;
	}
	float getArcVelocityX()
	{
		return velocity.x;
	}

	void setArcVelocityY(float v_y)
	{
		velocityY = v_y;
		velocity.y = velocityY;
	}
	float getArcVelocityY()
	{
		return velocity.y;
	}

	void setArcAcceleration(float a)
	{
		acceleration = a;
	}
	float getArcAcceleration()
	{
		return acceleration;
	}

	void setStartArcPosition(float x, float y)
	{
		startArcPosX = x;
		startArcPosY = y;
	}
	float getStartArcPositionX()
	{
		return startArcPosX;
	}
	float getStartArcPositionY()
	{
		return startArcPosY;
	}

	void setStartArcVelocity(Vector2f startVelocity)
	{
		startArcVelocity = startVelocity;
	}
	Vector2f getStartArcVelocity()
	{
		return startArcVelocity;
	}

	//이동(move)
	void moveArc(float dt)
	{
		setArcPos(getArcPosX() + getArcVelocityX() * dt, getArcPosY() + getArcVelocityY() * dt);
		setArcVelocityY(getArcVelocityY() + getArcAcceleration() * dt);

		arc.setPosition(getArcPosX(), getArcPosY());
	}
};

//통조림 클래스
class Canned_Food
{
	//직사각형으로 구현하면 좋을 듯

private:
  //변수(필드)
  //위치좌표, 크기

	Vector2f position;
	Texture texture;
	Sprite sprite;

	//double posX;
	//double posY;
	double sizeX;
	double sizeY;

	int sp;

public:
  //함수(메소드)
  //생성자, 소멸자
	Canned_Food(){}
	void setinfo(float x, float y,int sp)
	{
		position.x = x;
		position.y = y;
		//캔 스프라이트 생성
		this->sp = sp;
		if(sp == 1)
		    texture.loadFromFile("./Data/Image/canned_food.png"); // 파랑 캔
		else if(sp==2)
			texture.loadFromFile("./Data/Image/canned_food_red.png"); // 빨간 캔
		else if(sp==3)
			texture.loadFromFile("./Data/Image/canned_food_gold.png"); // 금색 캔
		Vector2u textureSize = texture.getSize();
		sprite.setTexture(texture);
		sprite.setScale((float)50 / textureSize.x, (float)50 / textureSize.y);
		sprite.setPosition(position);
    }
	~Canned_Food()
	{

	}
  //위치좌표, 크기 setter/getter
	void setFoodPos(double posX, double posY)
	{
		position.x = posX;
		position.y = posY;
	}
	int getPosition()
	{
		return position.x;
	}
	void setFoodSize(double sizeX, double sizeY)
	{
		this->sizeX = sizeX;
		this->sizeY = sizeY;
	}
	double getFoodSizeX()
	{
		return this->sizeX;
	}
	double getFoodSizeY()
	{
		return this->sizeY;
	}
	int getsp()
	{
		return this->sp;
	}
	Sprite* getsprite()
	{
		return &sprite;
	}

	//충돌 시 획득
	void getFoodScore(Score* score, int size);

	//충돌, 획득 시 visual,sound effect
	void getCannedFoodSound()
	{
		/*play_sound("./Data/Sound/676402__cjspellsfish__score-2.wav");*/
		Music music;
		if (!music.openFromFile("./Data/Sound/676402__cjspellsfish__score-2.wav"))
			cout << "can sound err\n";
		music.play();
		music.setVolume(50.f);
		music.setLoop(false);

	}

	//획득시 제거

	void draw(RenderWindow& window)
	{
			window.draw(sprite);
	}

	FloatRect getBounds() const
	{
		return sprite.getGlobalBounds();
	}
};

//사운드 재생 함수
void play_sound(const string& filename)
{
	SoundBuffer buffer;

	if (!buffer.loadFromFile(filename))
	{
		cout << "sound loadFromFile error" << endl;
		return;
	}
	Sound sound;
	sound.setBuffer(buffer);
	sound.play();
	delay_ms(900);
}

void delay_ms(int ms)
{
	Clock Timer;
	while (Timer.getElapsedTime().asMilliseconds() < ms);
}

class Floodlight
{
private:
	Vector2f position;
	bool isMovingRight;
	Texture texture;
	Sprite sprite;

public:
	Floodlight(float x, float y)
		: position(x, y), isMovingRight(true)
	{
		texture.loadFromFile("./Data/Image/floodlight.png");
		Vector2u lightTextureSize = texture.getSize();
		sprite.setTexture(texture);
		sprite.setScale((float)113.4 / lightTextureSize.x, (float)107.5 / lightTextureSize.y);
		sprite.setOrigin(sprite.getLocalBounds().width / 2, 0); //윗변 가운데를 중심점으로 설정 
		sprite.setPosition(position);
	}
	
	void swing()
	{
		if (isMovingRight)
		{
			sprite.setRotation(sprite.getRotation() - 1);
			if (sprite.getRotation() == 270)
				isMovingRight = false;
		}
		else
		{
			sprite.setRotation(sprite.getRotation() + 1);
			if (sprite.getRotation() == 90)
				isMovingRight = true;
		}
	}

	void draw(RenderWindow& window)
	{
		window.draw(sprite);
	}

	FloatRect getBounds() const
	{
		return sprite.getGlobalBounds();
	}
};

//장애물1 클래스(새장,전등,컵,그릇,다른 고양이 등)
class Obstacle1
{
	//장애물 여러 개 만들 때 상속으로 구현할지 논의 필요
	//직사각형으로 구현하면 좋을 듯

private:
	//변수(필드)
	//위치좌표, 크기
	double posX;
	double posY;
	double sizeX;
	double sizeY;
	//장애물이 움직이지 않고 고정됨

public:
	//함수(메소드)
	//생성자, 소멸자
	Obstacle1() {
		posX = 100;
		posY = 200;
		sizeX = 40;
		sizeY = 50;
	}
	Obstacle1(double posX, double posY, double sizeX, double sizeY) {
		this->posX = posX;
		this->posY = posY;
		this->sizeX = sizeX;
		this->sizeY = sizeY;
	}
	//위치좌표, 크기, 등 setter/getter
	void setObstaclePos(double posX, double posY) {
		this->posX = posX;
		this->posY = posY;
	}
	double getObstaclePosX() {
		return posX;
	}
	double getObstaclePosY() {
		return posY;
	}
	void setObstacleSizeX(double X) {
		sizeX = X;
	}
	void setObstacleSizeY(double Y) {
		sizeY = Y;
	}
	double getObstacleSizeX() {
		return sizeX;
	}
	double getObstacleSizeY() {
		return sizeY;
	}

	//충돌시 visual,sound effect
};

class Cup1:Obstacle1
{
private:
	Vector2f position;
	Texture texture;
	Sprite sprite;
public:
	Cup1(float x, float y)
		: position(x, y)
	{
		texture.loadFromFile("./Data/Image/cup1.jpg");
		Vector2u cup1TextureSize = texture.getSize();
		sprite.setTexture(texture);
		sprite.setScale((float)50 / cup1TextureSize.x, (float)50 / cup1TextureSize.y);
		sprite.setPosition(position);

	}
	Vector2f getPosition() {
		return sprite.getPosition();
	}
	void draw(RenderWindow& window)
	{
		window.draw(sprite);
	}
	FloatRect getBounds() const
	{
		return sprite.getGlobalBounds();
	}
};
class Cup2 :Obstacle1
{
private:
	Vector2f position;
	Texture texture;
	Sprite sprite;
public:
	Cup2(float x, float y)
		: position(x, y)
	{
		texture.loadFromFile("./Data/Image/cup2.jpg");
		Vector2u cup1TextureSize = texture.getSize();
		sprite.setTexture(texture);
		sprite.setScale((float)50 / cup1TextureSize.x, (float)50 / cup1TextureSize.y);
		sprite.setPosition(position);

	}
	Vector2f getPosition() {
		return sprite.getPosition();
	}
	void draw(RenderWindow& window)
	{
		window.draw(sprite);
	}
	FloatRect getBounds() const
	{
		return sprite.getGlobalBounds();
	}
};
class Basket :Obstacle1
{
private:
	Vector2f position;
	Texture texture;
	Sprite sprite;
public:
	Basket(float x, float y)
		: position(x, y)
	{
		texture.loadFromFile("./Data/Image/basket.png");
		Vector2u cup1TextureSize = texture.getSize();
		sprite.setTexture(texture);
		sprite.setScale((float)100 / cup1TextureSize.x, (float)70 / cup1TextureSize.y);
		sprite.setPosition(position);

	}
	Vector2f getPosition() {
		return sprite.getPosition();
	}
	void draw(RenderWindow& window)
	{
		window.draw(sprite);
	}
	FloatRect getBounds() const
	{
		return sprite.getGlobalBounds();
	}
};
class Micro :Obstacle1
{
private:
	Vector2f position;
	Texture texture;
	Sprite sprite;
public:
	Micro(float x, float y)
		: position(x, y)
	{
		texture.loadFromFile("./Data/Image/micro.jpg");
		Vector2u cup1TextureSize = texture.getSize();
		sprite.setTexture(texture);
		sprite.setScale((float)150 / cup1TextureSize.x, (float)100 / cup1TextureSize.y);
		sprite.setPosition(position);

	}
	Vector2f getPosition() {
		return sprite.getPosition();
	}
	void draw(RenderWindow& window)
	{
		window.draw(sprite);
	}
	FloatRect getBounds() const
	{
		return sprite.getGlobalBounds();
	}
};
class Bottle :Obstacle1
{
private:
	Vector2f position;
	Texture texture;
	Sprite sprite;
public:
	Bottle(float x, float y)
		: position(x, y)
	{
		texture.loadFromFile("./Data/Image/bottle.png");
		Vector2u cup1TextureSize = texture.getSize();
		sprite.setTexture(texture);
		sprite.setScale((float)50/ cup1TextureSize.x, (float)100 / cup1TextureSize.y);
		sprite.setPosition(position);

	}
	Vector2f getPosition() {
		return sprite.getPosition();
	}
	void draw(RenderWindow& window)
	{
		window.draw(sprite);
	}
	FloatRect getBounds() const
	{
		return sprite.getGlobalBounds();
	}
};
class Ob_Clock :Obstacle1
{
private:
	Vector2f position;
	Texture texture;
	Sprite sprite;
public:
	Ob_Clock(float x, float y)
		: position(x, y)
	{
		texture.loadFromFile("./Data/Image/clock.png");
		Vector2u cup1TextureSize = texture.getSize();
		sprite.setTexture(texture);
		sprite.setScale((float)100 / cup1TextureSize.x, (float)100 / cup1TextureSize.y);
		sprite.setPosition(position);

	}
	Vector2f getPosition() {
		return sprite.getPosition();
	}
	void draw(RenderWindow& window)
	{
		window.draw(sprite);
	}
	FloatRect getBounds() const
	{
		return sprite.getGlobalBounds();
	}
};


//
//바닥 클래스 -> 바닥을 스프라이트로 만들려면 Sprite 클래스를 상속받아야 하나?
class Floor
{
private:
	RectangleShape floor;
	//위치좌표,크기
	float posX;
	float posY;
	Vector2f size;
	Color color;

public:
	//생성자,소멸자
	Floor(float x, float y, float s_x, float s_y)
	{
		posX = x;
		posY = y;
		size.x = s_x;
		size.y = s_y;

		floor.setPosition(posX, posY);
		floor.setSize(size);
		floor.setFillColor(color.Black);
	}

	RectangleShape getFloor()
	{
		return floor;
	}

	//위치좌표,크기
	void setFloorPos(float x, float y)
	{
		posX = x;
		posY = y;
	}
	float getFloorPosX()
	{
		return posX;
	}
	float getFloorPosY()
	{
		return posY;
	}
	void setFloorSize(Vector2f s)
	{
		size = s;
	}
	Vector2f getFloorSize()
	{
		return size;
	}

};

//점수 클래스
class Score
{
private:
	//현재점수
	int currScore;
	//최고점수
	int maxScore;
	//위치, 크기(글자 크기 등)
	float posX;
	float posY;
	float size;

public:
	//생성자, 소멸자
	Score(float x, float y, float s)
	{
		posX = x;
		posY = y;
		size = s;

		currScore = 0;
		maxScore = 0;
	}

	//위치, 크기 등 setter/getter
	void setScorePos(float x, float y)
	{
		posX = x;
		posY = y;
	}
	float getScorePosX()
	{
		return posX;
	}
	float getScorePosY()
	{
		return posY;
	}
	void setScoreSize(float s)
	{
		size = s;
	}
	float getScoreSize()
	{
		return size;
	}

	//현재점수 반환
	void setCurrScore(int c_s)
	{
		currScore = c_s;
	}

	//현재점수 업데이트(통조림의 크기, 개수, 파괴한 장애물에 따라 점수 부여)
	int getCurrScore()
	{
		return currScore;
	}

	//최고점수 반환 
	void setMaxScore(int m_s)
	{
		maxScore = m_s;
	}

	//최고점수 업데이트
	int getMaxScore()
	{
		return maxScore;
	}
};

// 
void Canned_Food::getFoodScore(Score* score, int size)
{
	//사이즈별로 점수부여
	//ex) size 1 = 1000점
	//    size 2 = 1500점
	//    size 3 = 3000점 ...
	if (size == 1) {//파랑
		score->setCurrScore(score->getCurrScore()+1000);
	}
	else if (size == 2) {//빨강
		score->setCurrScore(score->getCurrScore() + 1500);
	}
	else if (size == 3) {//골드
		score->setCurrScore(score->getCurrScore() + 3000);
	}
}

//별 클래스
class Star
{
private:
	//위치, 크기

	//현재 별 개수
	int curstar;
	//이 스테이지 누적 최대 별 개수
	int stagemaxstar;//스테이지 종료시 업데이트
public:
	//생성자
	Star(int stagemaxstar) {
		curstar = 0;
		this->stagemaxstar = stagemaxstar;
	}
	//현재 별 개수 setter/getter
	void setStar(int score) {
		int cs = 0;
		if (score >= 30000) // 일정 점수 이상은 별 3개
		{
			cs = 3;
		}
		else if (score >= 20000 && score < 30000)
		{
			cs = 2;
		}
		else if (score >= 10000 && score < 20000)
		{
			cs = 1;
		}

		if (cs > curstar)
			curstar = cs;
	}
	int getStar() {
		return curstar;
	}
	//별 최대 개수 setter/getter
	void setMaxStar()
	{
		if (stagemaxstar < curstar)
			stagemaxstar = curstar;
	}
	int getMaxStar() {
		return stagemaxstar;
	}
};

//점프횟수 클래스
class Jump_number
{
private:
	int maxJump;  //최대 점프 횟수
	int leftJump; //남은 점프 횟수
	int posX;
	int posY;
	int size;
public:
	Jump_number() {  //기본생성자
		maxJump = 3;
		leftJump = 3;
		posX = 0;
		posY = 0;
		size = 20;
	}

	Jump_number(int mJump, int lJump, int x, int y, int s) {
		maxJump = mJump;
		leftJump = lJump;
		posX = x;
		posY = y;
		size = s;
	}

	~Jump_number() {

	}

	//최대 점프 횟수, 남은 점프 횟수, 위치, 크기 setter/getter
	void setMaxJump(int mJump) {
		maxJump = mJump;
	}
	int getMaxJump() {
		return maxJump;
	}

	void setLeftJump(int lJump) {
		leftJump = lJump;
	}
	int getLeftJump() {
		return leftJump;
	}

	void setPosition(int x, int y) {
		posX = x;
		posY = y;
	}
	int getPosX() {
		return posX;
	}
	int getPosY() {
		return posY;
	}

	void setSize(int s) {
		size = s;
	}
	int getSize() {
		return size;
	}

	//점프 횟수 감소 함수
	void reduceJump() {
		leftJump -= 1;
	}
};

//메뉴 클래스
class Menu
{
	// 재생, 일시정지, 다시 시작, 소리조절 등
private:
	//위치, 크기
	
	//현재상태(플레이중, 일시정지)
	
	//소리크기
public:
	//생성자,소멸자
	
	//위치, 크기, 현재상태 ,소리 크기 setter/getter
	
	//다시시작요청
	//상태변환시 visual,sound effect(sfml 이용해 구현)
};


//<시간 여유 시 추가로 구현>
//고양이의 능력(일정 스테이지 이상 클리어 시 새로운 고양이를 준다, 고양이별로 각자의 능력을 가짐, 충돌 전 클릭을 하면 고양이의 능력이 발현(ex-분신술))

//스테이지 클래스
class Stage
{
private:
	//위치좌표, 크기
	//최고로 많이 획득한 별 개수 -> 별 클래스에서 받아오기
	//획득한 최고 점수(default = 0) -> 점수 클래스에서 받아오기
public:
	//생성자, 소멸자 
	//위치, 크기, 최고점수, 별개수 setter/getter
};

//생명 클래스
class Heart
{
	//레벨 클리어 실패 시 하나씩 감소
	//30분마다 하나씩 증가
private:
	//위치좌표, 크기
	//현재 남은 생명 개수
public:
	//생성자, 소멸자 
	//위치, 크기, 생명 개수 setter/getter
};

//사용자 레벨 클래스
class Level
{
	//별을 일정 개수 이상 모으면 레벨업
private:
	//위치좌표, 크기
	//레벨 배열 생성, 배열의 인덱스 번호는 레벨을 뜻함, 배열의 값은 레벨업까지 모아야할 별 개수를 뜻함
	//현재 레벨
	//모은 별 개수 -> 별 클래스에서 받아와서 누적, 현재 레벨의 레벨업까지 모아야할 별 개수와 같은지 비교 후 같다면 현재 레벨 + 1
	//레벨업까지 모아야할 별 개수
public:
	//생성자, 소멸자 
	//위치, 크기, 레벨, 별개수 setter/getter
};

int main()
{
	// 창 생성
	RenderWindow window(VideoMode(960, 540), "Jumping cat");
	window.setFramerateLimit(60);	//프레임 정해주기

	//포물선 클래스 생성
	Arc arc(180, 450, 3);
	arc.setStartArcPosition(arc.getArcPosX(), arc.getArcPosY());

	// 바닥 스프라이트 생성
	Floor floor(430, 510, 100, 20);
	Texture floorTexture;
	//floorTexture.loadFromFile("./Data/Image/floor.png");
	Sprite floorSprite(floorTexture);

	//고양이 스프라이트 생성
	Cat cat(180.f, 450.f);
	cat.setStartPosition(cat.getPositionX(), cat.getPositionY());
	Vector2f dragStartPosition;
	Vector2f dragEndPosition;

	//사람 스프라이트 생성
	Texture girlTexture;
	girlTexture.loadFromFile("./Data/Image/girl.png");
	Vector2u girlTextureSize = girlTexture.getSize();
	Sprite girlSprite(girlTexture);
	girlSprite.setScale((float)200 / girlTextureSize.x, (float)200 / girlTextureSize.y);
	girlSprite.setPosition(0, 340);

	//조명등 스프라이트 생성
	Floodlight floodlight(800, 0); 


	Score score(0, 30, 20);

	//남은 점프 횟수 표시할 text 설정
	Jump_number jn;
	Text text;
	Font font;

	if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
		return 42; // Robust error handling!
	}

	text.setFont(font);
	text.setString("Chance: " + to_string(jn.getLeftJump()));
	text.setCharacterSize(jn.getSize());
	text.setFillColor(Color::Black);
	text.setPosition((float)jn.getPosX(), (float)jn.getPosY());

	Text game_score;
	game_score.setFont(font);
	game_score.setString("Score: " + to_string(score.getCurrScore()));
	game_score.setCharacterSize((int) score.getScoreSize());
	game_score.setFillColor(Color::Blue);
	game_score.setPosition(score.getScorePosX(), score.getScorePosY());

	// 게임 오버 또는 클리어 여부 표시할 text 설정
	Text game_status;
	game_status.setFont(font);
	game_status.setCharacterSize(40);
	game_status.setPosition(350, 260);

	Clock clock;

	//효과음, 배경음악
	Music music;
	if (!music.openFromFile("./Data/Sound/background_music.ogg"))
		return -1;
	music.play();
	music.setVolume(10.f);
	music.setLoop(true);

	Music cmusic;
	if (!cmusic.openFromFile("./Data/Sound/676402__cjspellsfish__score-2.wav"))
		cout << "can sound err\n";

	Music dragSound;
	if (!dragSound.openFromFile("./Data/Sound/meow.wav"))
		cout << "drag sound err\n";
	dragSound.setVolume(70);

	Music jumpSound;
	if (!jumpSound.openFromFile("./Data/Sound/jump.wav"))
		cout << "jump sound err\n";

	//배경화면 스프라이트 생성
	Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("./Data/Image/background.png"))
		return -1;
	Vector2u backgroundSize = backgroundTexture.getSize();
	Sprite backgroundSprite;
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setScale((float)(window.getSize().x*2) / backgroundSize.x, (float)window.getSize().y / backgroundSize.y);

	//배경 시점
	View view(Vector2f(window.getSize().x/2, window.getSize().y / 2), Vector2f(window.getSize().x, window.getSize().y));
	//backgroundSprite.setPosition(-cat.getPositionX(), 0);

	//별 스프라이트 생성
	Texture staryTexture;
	staryTexture.loadFromFile("./Data/Image/star_y.png");
	Vector2u starySize = staryTexture.getSize();
	Sprite starySprite(staryTexture);
	starySprite.setScale((float)100 / starySize.x, (float)100 / starySize.y);
	starySprite.setPosition(800, 150);

	Texture stargTexture;
	stargTexture.loadFromFile("./Data/Image/star_g.png");
	Vector2u stargSize = stargTexture.getSize();
	Sprite stargSprite(stargTexture);
	stargSprite.setScale((float)100 / stargSize.x, (float)100 / stargSize.y);
	stargSprite.setPosition(window.getSize().x/2, window.getSize().y*0.3);

	//전등
	Texture lightTexture;
	lightTexture.loadFromFile("./Data/Image/light1.png");
	Vector2u lightSize = lightTexture.getSize();
	Sprite lightSprite(lightTexture);
	lightSprite.setScale((float)100 / lightSize.x, (float)100 / lightSize.y);
	lightSprite.setPosition(800, 150);

	//후광
	Texture backlightTexture;
	backlightTexture.loadFromFile("./Data/Image/backlight.png");
	Vector2u backlightSize = backlightTexture.getSize();
	Sprite backlightSprite(backlightTexture);
	backlightSprite.setScale((float)100 / backlightSize.x, (float)100 / backlightSize.y);
	backlightSprite.setOrigin(backlightSize.x / 2, backlightSize.y / 2);
	backlightSprite.setPosition(660, 290);
	backlightSprite.setColor(Color(255, 255, 255, 128));

	//장애물
	Cup1 cup1(50, 100);
	Cup2 cup2(100, 100);
	Basket basket(200, 100);
	Bottle bottle(300, 100);
	Micro micro(400, 100);
	Ob_Clock ob_clock(500, 100);

	// can
	Canned_Food blue_can[8];
	Canned_Food red_can[7];
	Canned_Food gold_can[4];
	 /* 1000 * 8 + 1500 * 7 + 3000 * 4 = 30500*/
	blue_can[0].setinfo(660,290, 1); blue_can[1].setinfo(800, 400, 1); blue_can[2].setinfo(750, 250, 1); blue_can[3].setinfo(900, 100, 1);
	blue_can[4].setinfo(860, 230, 1); blue_can[5].setinfo(940, 240, 1); blue_can[6].setinfo(1100, 350, 1); blue_can[7].setinfo(1800, 400, 1);

	red_can[0].setinfo(1000, 200, 2); red_can[1].setinfo(1100, 170, 2); red_can[2].setinfo(1200, 180, 2);
	red_can[3].setinfo(1300, 250, 2); red_can[4].setinfo(1400, 330, 2); red_can[5].setinfo(1500, 300, 2); red_can[6].setinfo(1600,260, 2);

	gold_can[0].setinfo(1200, 400, 3); gold_can[1].setinfo(1700, 220, 3); gold_can[2].setinfo(1500, 410, 3); gold_can[3].setinfo(1800, 180, 3);
	//마우스
	Vector2f mcm;
	mcm = Vector2f(5,0);
	int mouseispressed = 0;

	bool reset = false;

	bool cat_is_clicked = false;  //마우스로 고양이를 클릭했는지 저장할 변수
	while (window.isOpen())
	{
		// 이벤트 처리
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (jn.getLeftJump() > 0) {
				if (event.type == Event::MouseButtonPressed)
				{
					if (event.mouseButton.button == Mouse::Left)
					{
						dragStartPosition = Vector2f((float)event.mouseButton.x, (float)event.mouseButton.y);
						if (dragStartPosition.x > cat.getPositionX() - cat.getTextureSizeX() / 2 && dragStartPosition.x < cat.getPositionX() + cat.getTextureSizeX() / 2 && dragStartPosition.y > cat.getPositionY() - cat.getTextureSizeY() / 2 && dragStartPosition.y < cat.getPositionY() + cat.getTextureSizeY() / 2)
							cat_is_clicked = true;
						dragSound.play();
					}
					mouseispressed = 1;
				}
				else if (event.type == Event::MouseButtonReleased)
				{
					if (event.mouseButton.button == Mouse::Left)
					{
						dragEndPosition = Vector2f((float)event.mouseButton.x, (float)event.mouseButton.y);
						Vector2f dragDistance = dragStartPosition - dragEndPosition;

						float jumpVelocityScale = 3.5f;
						Vector2f jumpVelocity = jumpVelocityScale * dragDistance;

						cat.setRotation(0);
						cat.setScale((float)159.7 / cat.getTextureSizeX(), (float)127.7 / cat.getTextureSizeY()); //고양이 크기 원래상태로 되돌리기

						jumpSound.play();

						// 드래그를 너무 조금하거나 오른쪽으로 하면 무시하고 아니면 날아감
						if ((abs(dragDistance.x) <= 20 && abs(dragDistance.y) <= 20) || dragDistance.x < 0) {
							cat_is_clicked = false;
						}
						else {
							if (cat_is_clicked == true) {
								cat_is_clicked = false;
								reset = false;

								//포물선 그리기 위한 속도, 가속도, 위치 세팅
								arc.setArcVelocity(jumpVelocity);
								arc.setStartArcVelocity(arc.getArcVelocity());
								arc.setArcAcceleration(cat.getGravity());
								
								arc.setArcPos(arc.getStartArcPositionX(), arc.getStartArcPositionY());
								

								//날아가는 코드 구현
								cat.jump(jumpVelocity);
							}
						}
						int x = 0, x2 = 0, y = 0, y2 = 0;  //좌표 초기화
					}
					mouseispressed = 0;
				}
			}
		}

		// 게임 로직
		float deltaTime = clock.restart().asSeconds();

		floodlight.swing();

		if (cat.getPositionY() >= floor.getFloorPosY())
		{
			cat.changeImage("./Data/Image/cat.png"); //texture 사용은 매우 무거운 작업이므로, 고양이가 떨어진 후 한 번만 실행되어야 함. 그래서 위치 변경함:)
			reset = true;

			jn.reduceJump(); //점프횟수 감소
			text.setString("Chance: " + to_string(jn.getLeftJump()));
		}

		if (reset == false)
		{
			cat.update(deltaTime);
		}
		else
		{
			if (cat.getScale().x < 0)
				cat.setScale(abs(cat.getScale().x), cat.getScale().y);
			cat.setPosition(cat.getStartPositionX(), cat.getStartPositionY());
			cat.setRotation(0);
			cat.jump(Vector2f(0, 0));
			view.setCenter(window.getSize().x / 2, window.getSize().y / 2); // 화면도 초기화
		}
		

		if (cat_is_clicked == true) {
			int x1 = Mouse::getPosition(window).x;
			int y1 = Mouse::getPosition(window).y;
			int move_pos_x = (int)dragStartPosition.x - x1;
			int move_pos_y = (int)dragStartPosition.y - y1;

			float ang = (float)(atan2(double(y1 - dragStartPosition.y), double(x1 - dragStartPosition.x)) * 180 / 3.141592);
			if (ang < 0) ang += 360;
			if (move_pos_x > 0)
				cat.setRotation((int)ang + 180);

			float drag_dis = (float)sqrt(pow(dragStartPosition.x - x1, 2) + pow(dragStartPosition.y - y1, 2)); //드래그한 거리
			if (drag_dis <= 70)
				cat.setScale((float)(159.7 + drag_dis) / cat.getTextureSizeX(), (float)(127.7 - drag_dis) / cat.getTextureSizeY());
		}

		
		//전등과 부딪히면 떨어짐
		if (cat.getBounds().intersects(floodlight.getBounds()))
		{
			cat.setIsJumping(false);
			cat.changeImage("./Data/Image/dizzycat.png"); //눈이 빙글빙글 도는 고양이 이미지로 바꿈
			cat.startFalling(200.f, 300.f);
		}
		cat.update(deltaTime);

		//장애물과 충돌시 튕겨나감.
		Vector2f direction;
		if (cat.getBounds().intersects(cup1.getBounds()) || cat.getBounds().intersects(cup2.getBounds()) ||
			cat.getBounds().intersects(basket.getBounds()) || cat.getBounds().intersects(bottle.getBounds()) ||
			cat.getBounds().intersects(ob_clock.getBounds()) || cat.getBounds().intersects(micro.getBounds()))
		{
			// 고양이와 컵1 충돌 체크
			if (cat.getBounds().intersects(cup1.getBounds())) {
				// 충돌 방향 계산
				direction = cat.getPosition() - cup1.getPosition();
			}
			// 고양이와 컵2 충돌 체크
			else if (cat.getBounds().intersects(cup2.getBounds())) 
				direction = cat.getPosition() - cup2.getPosition();

			// 고양이와 바구니 충돌 체크
			else if (cat.getBounds().intersects(basket.getBounds())) 
				direction = cat.getPosition() - basket.getPosition();

			// 고양이와 병 충돌 체크
			else if (cat.getBounds().intersects(bottle.getBounds()))
				direction = cat.getPosition() - bottle.getPosition();
		
			// 고양이와 시계 충돌 체크
			else if (cat.getBounds().intersects(ob_clock.getBounds()))
				direction = cat.getPosition() - ob_clock.getPosition();

			// 고양이와 전자레인지 충돌 체크
			else if (cat.getBounds().intersects(micro.getBounds()))
				direction = cat.getPosition() - micro.getPosition();

			// 단위 벡터로 정규화
			direction = direction / sqrtf(direction.x * direction.x + direction.y * direction.y);
			// 튕겨나가는 속도 설정
			float bounceSpeed = 500.0f;
			Vector2f bounceVelocity = direction * bounceSpeed;
			// 속도 업데이트
			cat.setVelocity(bounceVelocity);
		}

		//can과 충돌
		if (cat.getIsFalling() == false)//전등에 부딪힌 후 떨어질 때는 통조림 안 먹어짐.
		{
			for (int i = 0; i < 8; i++) {
				if (cat.getBounds().intersects(blue_can[i].getBounds())) 
				{
					blue_can[i].getFoodScore(&score, 1);                //파란캔
					blue_can[i].getsprite()->setPosition(5000, 5000);
					cout << "coll with blue can\n";
					blue_can[i].getCannedFoodSound();
					//can1.~Canned_Food();
				}
			}
			for (int i = 0; i < 7; i++) {
				if (cat.getBounds().intersects(red_can[i].getBounds()))
				{
					red_can[i].getFoodScore(&score, 2);               //빨간캔
					red_can[i].getsprite()->setPosition(5000, 5000);
					cout << "coll with red can\n";
					red_can[i].getCannedFoodSound();
					//can1.~Canned_Food();
				}
			}
			for (int i = 0; i < 4; i++) {
				if (cat.getBounds().intersects(gold_can[i].getBounds()))
				{
					gold_can[i].getFoodScore(&score, 3);               //금캔
					gold_can[i].getsprite()->setPosition(5000, 5000);
					cout << "coll with gold can\n";
					cmusic.play();
					/*gold_can[i].getCannedFoodSound();*/
					//can1.~Canned_Food();
				}
			}
		}
		//배경 시점 변경
		if (cat.getPositionX() > window.getSize().x*0.5 && cat.getPositionX() < (backgroundSize.x-window.getSize().x*0.5)) {
			//backgroundSprite.move(-cat.getVelocity().x/75, 0);
			view.setCenter(cat.getPositionX(), window.getSize().y / 2);
		}
		//if(mouseispressed)
		//{
		//	if (event.mouseButton.x > window.getSize().x * 0.7)
		//	{
		//		cout << event.mouseButton.x << "\n";
		//		view.move(mcm);
		//	}
		//	else if (event.mouseButton.x < window.getSize().x * 0.3)
		//	{
		//		cout << event.mouseButton.x << " --\n\n";
		//		view.move(-mcm);
		//	}

		//}
		backlightSprite.setRotation(backlightSprite.getRotation()-1);

		// 그리기
		window.clear(Color::White);

		if (jn.getLeftJump() <= 0) {
			// 일정 점수 이상이면 게임 클리어라고 뜨도록 해야함 + 메인화면
			// 클리어한 경우 (새창 사용? or 화면 새로 그리기?)
			if (score.getCurrScore() >= 10000)
			{
				game_status.setString("Game Clear");
				game_status.setFillColor(Color::Blue);

				window.draw(game_status);
				window.draw(starySprite);
			}
			else {
				// 클리어하지 못한 경우
				game_status.setString("Game Over");
				game_status.setFillColor(Color::Red);

				window.draw(game_status);
				window.draw(stargSprite);
			}
		}
		else {
			window.setView(view);
			window.draw(backgroundSprite);
			window.draw(text);
			window.draw(game_score);
			//window.draw(floorSprite);
			window.draw(floor.getFloor());
			window.draw(backlightSprite);
			floodlight.draw(window);
			for (int i = 0; i < 8; i++){
				if (blue_can[i].getPosition() != 5000)
					blue_can[i].draw(window);
			}
			for (int i = 0; i < 7; i++){
				if (red_can[i].getPosition() != 5000)
					red_can[i].draw(window);
			}
			for (int i = 0; i < 4; i++){
				if (gold_can[i].getPosition() != 5000)
					gold_can[i].draw(window);
			}
			cat.draw(window);
			window.draw(girlSprite);
			cup1.draw(window);
			cup2.draw(window);
			bottle.draw(window);
			basket.draw(window);
			micro.draw(window);
			ob_clock.draw(window);

			//포물선 화면에 그려주기
			arc.setArcVelocity(arc.getStartArcVelocity());

			arc.setArcPos(arc.getStartArcPositionX(), arc.getStartArcPositionY());

			int ArcDrawCount = 0;

			for (float tldt = 0; tldt <= 0.02; tldt += 0.0001)
			{
				if (ArcDrawCount == 0 || ArcDrawCount % 10 == 0)
				{
					window.draw(arc.getArc());
				}

				arc.moveArc(tldt);
				ArcDrawCount++;
			}
		}

		window.display();


	}

	return 0;
}
