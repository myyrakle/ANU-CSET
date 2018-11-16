#define SFML_STATIC
#include <SFML/Audio.hpp>

class MusicPlayer
{
private:
	sf::Music music;
	bool opened = false;
public:
	operator bool() const
	{
		return opened;
	}
	bool operator!() const
	{
		return !opened;
	}
public:
	bool open_file(const String& filename)
	{
		try 
		{
			std::string temp;

			for (const auto& e : filename)
				temp += e;

			music.openFromFile(temp);
			opened = true;
			return opened;
		}
		catch (...)
		{
			return !opened;
		}
	}
	bool is_opened() const
	{
		return opened;
	}
public:
	int get_music_length() const
	{
		return music.getDuration().asSeconds();
	}
public:
	void set_playing_offset(long long second)
	{
		music.setPlayingOffset(sf::seconds(second));
	}
	long long get_playing_offset() const
	{
		return (music.getPlayingOffset().asSeconds());
	}
public:
	void set_volume(float f) //������ �����մϴ�. �ִ� 100
	{
		music.setVolume(f);
	}
	float get_volume() const //���� ������ �����ɴϴ�.
	{
		return music.getVolume();
	}
	void set_pitch(float f) 
	{
		music.setPitch(f);
	}
	float get_pitch() const
	{
		return music.getPitch();
	}
	bool is_auto_repeatable() const //�ڵ����� �ݺ��ϴ��� Ȯ���մϴ�.
	{
		return music.getLoop();
	}
	void set_auto_repeat(bool _do) //�ڵ��ݺ����θ� �����մϴ�.
	{
		music.setLoop(_do);
	}
public:
	void play() //�񵿱�� ����մϴ�.
	{
		music.play();
	}
	void play_with_block(void(*func)(void) = nullptr) //����� ����մϴ�.
	{
		music.play();
		block_until_ended(func);
	}
	void block_until_ended(void(* func)(void) = nullptr) //����ϰ�, ���������� ���� �̴ϴ�. �ܼ� ����
	{
		while (this->is_playing())
			if (func != nullptr)
				func();
	}
	void pause() //�����մϴ�.
	{
		music.pause();
	}
	void stop() //�ߴ��մϴ�.
	{
		music.stop();
	}
public:
	bool is_playing() const //������θ� Ȯ���մϴ�.
	{
		return music.getStatus() == sf::Music::Status::Playing;
	}
	bool is_paused() const //�������θ� Ȯ���մϴ�.
	{
		return music.getStatus() == sf::Music::Status::Paused;
	}
	bool is_stopped() const //�ߴܿ��θ� Ȯ���մϴ�.
	{
		return music.getStatus() == sf::Music::Status::Stopped;
	}
};