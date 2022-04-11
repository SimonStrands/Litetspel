#include "SoundManager.h"
#include "Keyboard.h"

SoundManager::SoundManager()
{
	this->volume = 1;
	musicLoop = false;
}

SoundManager::SoundManager(float volume)
{
	this->volume = volume;
	musicLoop = false;
}

void SoundManager::update(vec3 playerPos, vec3 playerDir, vec3 upVec)
{
	sf::Listener::setPosition(playerPos.x, playerPos.y, playerPos.z);
	sf::Listener::setDirection(-playerDir.x, -playerDir.y, -playerDir.z);
	sf::Listener::setUpVector(upVec.x, upVec.y, upVec.z);
}

void SoundManager::loadSound(std::string filePath, float volume, std::string name)
{
	if (!buffer.loadFromFile(filePath)) {
		std::cout << "cannot load file:" << filePath << std::endl;
		return;
	}
	sf::Sound newSound;
	newSound.setBuffer(buffer);
	newSound.setVolume(this->volume * volume);
	
	std::string Soundname;
	if (name == "") {
		Soundname = filePath;
	}
	else {
		Soundname = name;
	}
	sounds.insert(std::pair<std::string, sf::Sound>(Soundname, newSound));

}

void SoundManager::playSound(std::string soundName)
{
	if (sounds.find(soundName) == sounds.end()) {
		std::cout << "couldn't find sound: " << soundName << std::endl;
		return;
	}
	sounds.find(soundName)->second.play();
}

void SoundManager::playSound(std::string soundName, vec3 soundposition)
{
	if (sounds.find(soundName) == sounds.end()) {
		std::cout << "couldn't find sound: " << soundName << std::endl;
		return;
	}
	sounds.find(soundName)->second.setPosition(soundposition.x, soundposition.y, soundposition.z);
	sounds.find(soundName)->second.play();
}

void SoundManager::playMusic(std::string filePath, float volume)
{
	music.setVolume(volume * this->volume);
	if (this->music.openFromFile(filePath)) {
		music.play();
		music.setLoop(false);
	}
	else {
		std::cout << "cant find file: " << filePath << std::endl;
	}
}

void SoundManager::setMusicLoop(const bool loop)
{
	this->musicLoop = loop;
}

float SoundManager::getVolume() const
{
	return this->volume;
}
