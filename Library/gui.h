#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>
#include <functional>

struct Point
{
	int x;
	int y;
};

struct Quad
{
	unsigned int width;
	unsigned int height;
};

namespace gui
{
	using Event = sf::Event::EventType;
	using Keyboard = sf::Keyboard;
	using Color = sf::Color;
	using Font = sf::Font;

	class EventInfo
	{
	private:
		const sf::Event& event;
	public:
		EventInfo(const sf::Event& event) : event(event)
		{}
	public:
		EventInfo() = delete;
		virtual ~EventInfo() = default;
		EventInfo(const EventInfo&) = delete;
		EventInfo& operator=(const EventInfo&) = delete;
		EventInfo(EventInfo&&) = default;
		EventInfo& operator=(EventInfo&&) = default;
	public:
		unsigned int getchar() const //���� ���� ��ȯ
		{
			return event.text.unicode;
		}
		Point get_moved_mouse() const //���� ���콺��ġ ��ȯ
		{
			return Point{ event.mouseMove.x, event.mouseMove.y };
		}
		Quad get_resized_size() const //���� â ũ�� ��ȯ
		{
			return Quad{ event.size.width, event.size.height };
		}
		sf::Keyboard::Key get_key() const //���� Ű
		{
			return event.key.code;
		}
	public:
		/*
		mouse_pressed/mose_released ���� �޼���
		*/
		bool is_left_button() const //���� ���콺 ��ư�� ���ȴٸ� true
		{
			return event.mouseButton.button == sf::Mouse::Button::Left;
		}
		bool is_right_button() const //������ ���콺 ��ư�� ���ȴٸ� true
		{
			return event.mouseButton.button == sf::Mouse::Button::Right;
		}
		Point get_clicked_position() const //���콺 ��ư���� Ŭ���� ��ġ ��ȯ
		{
			return Point{ event.mouseButton.x, event.mouseButton.y };
		}
		Point get_scrolled_position() const
		{
			return Point{ event.mouseWheelScroll.x, event.mouseWheelScroll.y };
		}
		float get_scrolled_ticks() const
		{
			return event.mouseWheelScroll.delta;
		}
	};

	class Window
	{
	private:
		sf::RenderWindow window;
		sf::Event event;
		sf::String title = L"�̸� ����";
		Quad window_size = { 200,200 };
		int window_style = sf::Style::Default;
		bool created = false;
	public:
		Window() : window()
		{
			on_started = [this]()
			{
				this->cover_with_color(sf::Color::White);
				this->display();
			};

			/*	on_resized = [this](Quad sizes)
			{
			this->window.display();
			};*/

			on_closed = [this]()
			{
				this->close();
			};
		}
		virtual ~Window() = default;
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		Window(Window&&) = default;
		Window& operator=(Window&&) = default;
	public:
		void run()
		{
			if (created == false)
				this->create();

			if (on_started != nullptr)
				on_started();

			while (this->is_open())
			{
				if (on_always != nullptr) //�׻� ����
					on_always();

				while (this->poll_event())
				{
					auto current_event = this->get_event(); //���� �̺�Ʈ
					auto info = this->get_event_info(); //�̺�Ʈ �߰�����

					if (pre_translated_events != nullptr)
						pre_translated_events(current_event);

					if (this->before_handling_events != nullptr)
						before_handling_events();

					switch (current_event) //�̺�Ʈ �б�
					{
					case sf::Event::EventType::Closed: //���� �� ����˴ϴ�.
						if (on_closed != nullptr)
							on_closed();
						break;

					case sf::Event::EventType::TextEntered: //���� �Է½� ����˴ϴ�.
						if (on_text_entered != nullptr)
							on_text_entered(info.getchar());
						break;

					case sf::Event::EventType::KeyPressed: //Ű�� ������ �� ����˴ϴ�.
						if (on_key_down)
							on_key_down(info.get_key());
						break;

					case sf::Event::EventType::KeyReleased: //Ű�� ������ �� ����˴ϴ�.
						if (on_key_up)
							on_key_up(info.get_key());
						break;

					case sf::Event::EventType::MouseEntered: //���콺�� ȭ�鿡 �����ϸ� ����˴ϴ�.
						if (on_mouse_entered != nullptr)
							on_mouse_entered();
						break;

					case sf::Event::EventType::MouseLeft: //���콺�� ȭ���� ����� ����˴ϴ�.
						if (on_mouse_left != nullptr)
							on_mouse_left();
						break;

					case sf::Event::EventType::MouseWheelScrolled: //���� ���ư� �� ����˴ϴ�.
						if (on_wheel_scrolled != nullptr)
							on_wheel_scrolled(info.get_scrolled_position(), info.get_scrolled_ticks());
						break;

					case sf::Event::EventType::MouseMoved: //���콺�� ������ �� ����˴ϴ�.
						if (on_mouse_move != nullptr)
							on_mouse_move(info.get_moved_mouse());
						break;

					case sf::Event::EventType::MouseButtonPressed: //��ư�� ���� �� ����˴ϴ�.
					{
						if (info.is_left_button()) //���� ��ư�� ���� ��
						{
							auto pair = info.get_clicked_position();
							if (on_left_button_down != nullptr)
								on_left_button_down({ pair.x, pair.y });
						}
						else if (info.is_right_button()) //������ ��ư�� ���� ��
						{
							auto pair = info.get_clicked_position();
							if (on_right_button_down != nullptr)
								on_left_button_down({ pair.x, pair.y });
						}
						else
						{
							//???
						}
						break;
					}

					case sf::Event::EventType::MouseButtonReleased: //��ư�� ������ �� ȣ��˴ϴ�.
					{
						if (info.is_left_button()) //���� ��ư�� ���� ��
						{
							auto pair = info.get_clicked_position();
							if (on_left_button_up != nullptr)
								on_left_button_up({ pair.x, pair.y });
						}
						else if (info.is_right_button()) //������ ��ư�� ���� ��
						{
							auto pair = info.get_clicked_position();
							if (on_right_button_up != nullptr)
								on_left_button_up({ pair.x, pair.y });
						}
						else
						{
							//???
						}
						break;
					}

					case sf::Event::EventType::GainedFocus: //��Ŀ���� ���� �� ȣ��˴ϴ�,
						if (on_gained_focus != nullptr)
							on_gained_focus();
						break;

					case sf::Event::EventType::LostFocus: //��Ŀ���� ���� �� ȣ��˴ϴ�.
						if (on_lost_focus != nullptr)
							on_lost_focus();
						break;

					case sf::Event::EventType::Resized: //ũ�Ⱑ ����� �� ȣ��˴ϴ�.
						if (on_resized != nullptr)
							on_resized(info.get_resized_size());
						break;

					default: //�̿� �۾����� ȣ��˴ϴ�.
						if (on_default != nullptr)
							on_default();
						break;
					}

					if (this->after_handling_events != nullptr)
						after_handling_events();

					continue;
				}

				/*if (on_no_event != nullptr)
				on_no_event(*this);*/ //�̱���
			}
		}
	public: //�̺�Ʈ ó�����Դϴ�.
		std::function<void()> on_started = nullptr; //���۵ɶ� �ѹ��� ȣ��˴ϴ�.
		std::function<void()> on_always = nullptr; //�׻� ȣ��˴ϴ�.
		std::function<void()> on_paint = nullptr;  //�׸��� �۾��� ����� ��Ƴ��� �մϴ�.
		std::function<void()> on_no_event = nullptr; //�ƹ��� �̺�Ʈ�� ���� �� �߻��մϴ�. //���� �������� �ʾҽ��ϴ�.

		std::function<void(sf::Event::EventType&)> pre_translated_events = nullptr; //������ �̺�Ʈ�� �߰����� ���۾��� �մϴ�.
		std::function<void()> before_handling_events = nullptr; //�̺�Ʈ�� �������� ��, �̺�Ʈ���� ó���ϱ� ���� ȣ��˴ϴ�.
		std::function<void()> after_handling_events = nullptr; //�̺�Ʈ�� �������� ��, �̺�Ʈ���� ó���� �Ŀ� ȣ��˴ϴ�.

		std::function<void(unsigned int)> on_text_entered = nullptr; //���� Ű�� ������ �� ȣ��˴ϴ�.
		std::function<void(sf::Keyboard::Key)> on_key_down = nullptr; //Ű���尡 ���� �� ȣ��˴ϴ�.
		std::function<void(sf::Keyboard::Key)> on_key_up = nullptr; //Ű���尡 ���� �� ȣ��˴ϴ�.	

		std::function<void()> on_gained_focus = nullptr; //��Ŀ���� ����� �� ȣ��˴ϴ�.
		std::function<void()> on_lost_focus = nullptr; //��Ŀ���� ���� �� ȣ��˴ϴ�.

		std::function<void(Point)> on_left_button_down = nullptr; //���� ��ư�� ���� �� ȣ��˴ϴ�.
		std::function<void(Point)> on_left_button_up = nullptr; //���� ��ư�� �� �� ȣ��˴ϴ�.
		std::function<void(Point)> on_right_button_down = nullptr; //������ ��ư�� ���� �� ȣ��˴ϴ�.
		std::function<void(Point)> on_right_button_up = nullptr; //������ ��ư�� �� �� ȣ��˴ϴ�.

		std::function<void(Point)> on_mouse_move = nullptr; //���콺�� ������ �� ȣ��˴ϴ�.
		std::function<void()> on_mouse_entered = nullptr; //���콺�� â�� �������� �� ȣ��˴ϴ�.
		std::function<void()> on_mouse_left = nullptr; //���콺�� â���� ������ �� ȣ��˴ϴ�.

		std::function<void(Point, float)> on_wheel_scrolled = nullptr; //���콺 ���� ������ �� ȣ��˴ϴ�.

		std::function<void()> on_closed = nullptr; //���� �� ȣ��˴ϴ�.
		std::function<void(Quad)> on_resized = nullptr; //â ũ�Ⱑ �ٲ� �� ȣ��˴ϴ�.
		std::function<void()> on_default = nullptr; //�⺻ �����Դϴ�.
	public:
		void paint()
		{
			if (on_paint != nullptr)
				on_paint();
		}
	public:
		sf::Event::EventType get_event() const //���� �̺�Ʈ ��ȯ
		{
			return event.type;
		}
		bool poll_event() //�̺�Ʈ Ȯ��
		{
			return window.pollEvent(this->event);
		}
	public:
		bool is_open() const
		{
			return window.isOpen();
		}
		void close()
		{
			window.close();
		}
	public:
		bool has_focus() const
		{
			return window.hasFocus();
		}
		void set_visible(bool want = true)
		{
			return window.setVisible(want);
		}
		void set_position(int x, int y)
		{
			window.setPosition(sf::Vector2i(x, y));
		}
	public:
		void create()
		{
			this->create(window_size.width, window_size.height, title, window_style); //����
		}
		void create(unsigned int _width, unsigned int _height, std::wstring _title, int _style) //������ �Լ�
		{
			this->created = true;
			window.create(sf::VideoMode(_width, _height), _title, _style);
		}
	public:
		void resizable(bool want) //true�� ��� �������� ũ�⸦ ������ �� �ֽ��ϴ�.
		{
			if (want)
				window_style |= sf::Style::Resize;
			else
				window_style &= ~sf::Style::Resize;
		}
		void use_titlebar(bool want) //true�� ��� Ÿ��Ʋ�ٰ� ǥ�õ˴ϴ�.
		{
			if (want)
				window_style |= sf::Style::Titlebar;
			else
				window_style &= ~sf::Style::Titlebar;
		}
		void use_close_button(bool want) //true�� ��� �ݱ� ��ư�� ǥ�õ˴ϴ�.
		{
			if (want)
				window_style |= sf::Style::Close;
			else
				window_style &= ~sf::Style::Close;
		}
		void use_default_window()
		{
			window_style = sf::Style::Default;
		}
		void use_fullscreen()
		{
			window_style = sf::Style::Fullscreen;
		}
		void use_none_decoration()
		{
			window_style = sf::Style::None;
		}
	public: //â ũ�� ����
		void set_size(unsigned int width, unsigned int height)
		{
			window_size = { width, height };
			window.setSize(sf::Vector2u(width, height));
		}
		void set_size(const Quad& size)
		{
			set_size(size.width, size.height);
		}
		Quad get_size() const
		{
			return Quad{ window.getSize().x, window.getSize().y };
		}
		unsigned int get_width() const
		{
			return window_size.width;
		}
		unsigned int get_height() const
		{
			return window_size.height;
		}
	public:
		void set_title(std::wstring title)
		{
			this->title = title;
			window.setTitle(title);
		}
		const sf::String& get_title() const
		{
			return this->title;
		}

		/*�׷��� ó��*/
	public:
		void clear(sf::Color color = sf::Color::White)
		{
			window.clear(color);
		}
		void cover_with_color(sf::Color color = sf::Color::White)
		{
			window.clear(color);
		}
		void add_drawable(sf::Drawable& drawable)
		{
			window.draw(drawable);
		}
		template<class SF_Drawable>
		void add_drawables(Vector<SF_Drawable>& drawables)
		{
			for (auto& e : drawables)
				window.draw(e);
		}
		void display() //����
		{
			window.display();
		}
	public:
		static bool is_key_pressed(sf::Keyboard::Key key)
		{
			return sf::Keyboard::isKeyPressed(key);
		}
		static bool is_left_button_down()
		{
			return sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
		}
		static bool is_right_button_down()
		{
			return sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
		}
		static Point get_mouse_position() //���� ���콺 ��ǥ ��ȯ
		{
			auto pos = sf::Mouse::getPosition();
			return Point{ pos.x, pos.y };
		}
	public:
		EventInfo get_event_info() const //�̺�Ʈ ���� ó�� ����
		{
			return EventInfo(this->event);
		}
	};


	class ThinLine
	{
	private:
		sf::VertexArray line = sf::VertexArray(sf::LinesStrip, 2);
	public:
		ThinLine() = default;
		virtual ~ThinLine() = default;
		ThinLine(Point from, Point to, sf::Color color = sf::Color::Black)
		{
			set_from_and_to(from, to);
			set_color(color);
		}
		void set_color(sf::Color color)
		{
			line[0].color = color;
			line[1].color = color;
		}
		void set_from_and_to(Point from, Point to)
		{
			line[0].position = { (float)from.x, (float)from.y };
			line[1].position = { (float)to.x, (float)to.y };
		}
		operator sf::Drawable&()
		{
			return line;
		}
	};
}