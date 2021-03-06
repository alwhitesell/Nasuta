#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "../../Engine/SceneNode/entity.h"

#include "Trambo/Resources/resourceHolder.h"

#include <SFML/Graphics/Sprite.hpp>


class Projectile : public Entity
{
public:
	enum class Type
	{
		AlliedBullet,
		EnemyBullet,
		Missile,
		TypeCount
	};


public:
							Projectile(Type type, const trmb::TextureHolder& textures);

	void					guideTowards(sf::Vector2f position);
	bool					isGuided() const;

	virtual unsigned int	getCategory() const;
	virtual sf::FloatRect	getBoundingRect() const;
	float					getMaxSpeed() const;
	int						getDamage() const;


private:
	virtual void			updateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	std::string				toString(Type type) const;
	Textures::ID			toTexture(const std::string& str) const;
	void					readXML(Type type);


private:
	struct Data
	{
		int					damage;
		float				speed;
		Textures::ID		texture;
		sf::IntRect			textureRect;
	};


private:
	Type					mType;
	sf::Sprite				mSprite;
	sf::Vector2f			mTargetDirection;

	Data					mData;
};

#endif
