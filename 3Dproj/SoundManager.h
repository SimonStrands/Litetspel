#pragma once
#include "SFML/Audio.hpp"
#include <map>
#include <iostream>
#include "Vec.h"

class SoundManager {
public:
	SoundManager();
	SoundManager(float volume);
	void update(vec3 playerPos, vec3 playerDir, vec3 upVec = vec3(0,1,0));
	void loadSound(std::string filePath, float volume = 1, std::string name = "");
	void playSound(std::string soundName);
	void playSound(std::string soundName, vec3 soundposition);
	void playMusic(std::string filePath, float volume = 1);
	void setMusicLoop(const bool loop);
	float getVolume()const;
private:
	std::map<std::string, sf::Sound> sounds;
	sf::Music music;
	bool musicLoop;
	float volume;
	sf::SoundBuffer buffer;
};