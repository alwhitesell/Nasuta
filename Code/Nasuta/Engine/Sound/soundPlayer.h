#ifndef SOUND_PLAYER_H
#define SOUND_PLAYER_H

#include "../Resource/resourceHolder.h"
#include "../Resource/resourceIdentifiers.h"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>

#include <list>


class SoundPlayer : private sf::NonCopyable
{
public:
								SoundPlayer();

	void						play(SoundEffect::ID effect);
	void						play(SoundEffect::ID effect, sf::Vector2f position);

	void						removeStoppedSounds();
	void						setListenerPosition(sf::Vector2f position);
	sf::Vector2f				getListenerPosition() const;

private:
	SoundBufferHolder			mSoundBuffers;
	std::list<sf::Sound>		mSounds;
};

#endif