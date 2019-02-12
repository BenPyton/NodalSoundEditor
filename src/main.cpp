#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "Core.h" // custom classes and functions (like Input, Button, etc.)
#include "Signal.h"
#include "SignalRenderer.h"
#include "Components/SinusComponent.h"
#include "Components/SquareComponent.h"
#include "Components/SawToothComponent.h"
#include "Components/TriangleComponent.h"
#include "Components/RandomComponent.h"
#include "Components/ComponentRenderer.h"
#include "Components/OutputComponent.h"

#define SAMPLERATE 48000

using namespace std;

int main()
{
	Window::Create(sf::VideoMode(800, 600), "Sound Generator");

	// Load font
	sf::Font font;
	if (!font.loadFromFile("../../../data/font/consola.ttf"))
	{
		printf("Error when loading font");
	}

	sf::Color lightGrey(200, 200, 200);

	UIStyle style;
	style.setFont(font, 16);
	style.mapStyle(UI_NORMAL, lightGrey, sf::Color::Black, lightGrey, 1.0f);
	style.mapStyle(UI_HOVERED, lightGrey, sf::Color(50, 50, 50), lightGrey, 3.0f);
	style.mapStyle(UI_CLICKED, sf::Color::Black, lightGrey, lightGrey, 1.0f);
	style.mapStyle(UI_DISABLED, lightGrey, sf::Color::Red, lightGrey, 1.0f);
	style.mapStyle(UI_FOCUSED, lightGrey, sf::Color::Blue, lightGrey, 1.0f);


	// Button Example
	Button button(10, -10, 180, 30, style);
	button.setText("Generate");
	button.setMargins(10, 10, 10, 10);
	button.setAnchorMin(sf::Vector2f(0.0f, 1.0f));
	button.setAnchorMax(sf::Vector2f(0.0f, 1.0f));
	button.setPivot(sf::Vector2f(0.0f, 1.0f));


	Label labelDuration(0, 0, 100, 30, style);
	labelDuration.setText("Duration");
	labelDuration.setAlignement(TextAlign::ALIGN_RIGHT);

	// InputField Example
	InputField input(10, 90, 150, 30, style);
	input.setMaxLength(10);
	input.setFloat(0.1f);
	input.setPlaceholder("Test");


	Button button1(-10, 10, 150, 30, style);
	button1.setText("Sinusoidal");

	Button button2(10, 10, 150, 30, style);
	button2.setText("Square");

	Button button3(-10, 10, 150, 30, style);
	button3.setText("Triangle");

	Button button4(-10, 10, 150, 30, style);
	button4.setText("SawTooth");

	Button button5(-10, 10, 150, 30, style);
	button5.setText("Random");

	Button button6(-10, 10, 150, 30, style);
	button6.setText("Modulated Sinus");


	//Button btnPlay(10, 10, 70, 30, style);
	//btnPlay.setText("Play");

	IconButton btnLoad(10, 10, 30, 30, style);
	btnLoad.setIconSize(32, 32);
	btnLoad.setIcon("../../../data/Images/icon_open.png");

	IconButton btnSave(10, 10, 30, 30, style);
	btnSave.setIconSize(32, 32);
	btnSave.setIcon("../../../data/Images/icon_save.png");

	IconButton btnStart(10, 10, 30, 30, style);
	btnStart.setIconSize(32, 32);
	btnStart.setIcon("../../../data/Images/icon_start.png");

	IconButton btnPlay(10, 10, 30, 30, style);
	btnPlay.setIconSize(32, 32);
	btnPlay.setIcon("../../../data/Images/icon_play.png");

	IconButton btnPause(10, 10, 30, 30, style);
	btnPause.setIconSize(32, 32);
	btnPause.setIcon("../../../data/Images/icon_pause.png");

	IconButton btnStop(10, 10, 30, 30, style);
	btnStop.setIconSize(32, 32);
	btnStop.setIcon("../../../data/Images/icon_stop.png");

	IconButton btnEnd(10, 10, 30, 30, style);
	btnEnd.setIconSize(32, 32);
	btnEnd.setIcon("../../../data/Images/icon_end.png");


	Label labelVolume(0, 0, 100, 30, style);
	labelVolume.setText("Volume");
	labelVolume.setAlignement(TextAlign::ALIGN_RIGHT);

	InputField inputVolume(90, 50, 70, 30, style);
	inputVolume.setMaxLength(5);
	inputVolume.setFloat(10.0f);
	inputVolume.setPlaceholder("Volume");

	VerticalLayout vLayout(0, 100, 200, 200);
	vLayout.setAnchorMin(sf::Vector2f(0, 0));
	vLayout.setAnchorMax(sf::Vector2f(0, 1));
	vLayout.setMargins(10, 10, 150, 10);
	vLayout.setSpacing(10);
	vLayout.setPaddings(10, 10, 10, 10);
	vLayout.add(button1);
	vLayout.add(button2);
	vLayout.add(button3);
	vLayout.add(button4);
	vLayout.add(button5);
	vLayout.add(button6);

	HorizontalLayout toolLayout(10, 10, 0, 30);
	toolLayout.setAnchorMin(sf::Vector2f(0, 0));
	toolLayout.setAnchorMax(sf::Vector2f(1, 0));
	toolLayout.setMargins(10, 10, 10, 10);
	toolLayout.setSpacing(10);
	toolLayout.add(btnLoad);
	toolLayout.add(btnSave);
	toolLayout.add(btnStart);
	toolLayout.add(btnPlay);
	toolLayout.add(btnPause);
	toolLayout.add(btnStop);
	toolLayout.add(btnEnd);
	toolLayout.add(labelVolume);
	toolLayout.add(inputVolume);
	toolLayout.add(labelDuration);
	toolLayout.add(input);

	// View
	View view(0, 0, 0, 0, style);
	view.setAnchorMin(sf::Vector2f(0, 0));
	view.setAnchorMax(sf::Vector2f(1, 1));
	view.setMargins(200, 10, 150, 10);

	// Window layout
	Layout rootLayout(0, 0, Window::GetWidth(), Window::GetHeight());
	Window::SetLayout(rootLayout);
	rootLayout.add(button);
	//rootLayout.add(input);
	rootLayout.add(vLayout);
	rootLayout.add(view);
	//rootLayout.add(btnPlay);
	//rootLayout.add(inputVolume);
	rootLayout.add(toolLayout);




	vector<sf::Int16> samples;
	/*sf::VertexArray vertices;
	vertices.setPrimitiveType(sf::PrimitiveType::LineStrip);*/

	Signal signal;
	SignalRenderer signalRenderer(10, 50, 100, 80, style);
	signalRenderer.setSignal(signal);
	signalRenderer.setAnchorMin(sf::Vector2f(0, 0));
	signalRenderer.setAnchorMax(sf::Vector2f(1, 0));
	signalRenderer.setMargins(200, 10, 10, 10);
	rootLayout.add(signalRenderer);

	/*float fadein_first = 0.2;
	float fadein_last = 0.6f;
	float fadeout_first = 0.4f;
	float fadeout_last = 0.8f;*/

	sf::Vector2f prevMousePos;

	SinusComponent sinusGenerator;
	sinusGenerator.getInput("Frequency")->setDefaultValue(200);


	SinusComponent sinusGenerator2;
	sinusGenerator.getInput("Frequency")->setDefaultValue(200);

	SinusComponent sinusAttenuation;
	sinusAttenuation.getInput("Frequency")->setDefaultValue(1.5f);
	sinusAttenuation.getInput("Offset")->setDefaultValue(0.5f);
	sinusAttenuation.getInput("Amplitude")->setDefaultValue(0.5f);

	sinusGenerator2.getInput("Amplitude")->setComponent(&sinusAttenuation);


	SquareComponent squareGenerator;
	squareGenerator.getInput("Frequency")->setDefaultValue(200);
	//squareGenerator.getInput("Amplitude")->setDefaultValue(0.5f);
	//squareGenerator.getInput("Offset")->setDefaultValue(0.5f);

	//sinusGenerator.getInput("Amplitude")->setComponent(&squareGenerator);
	//squareGenerator.getInput("Amplitude")->setComponent(&sinusGenerator);

	TriangleComponent triangleGenerator;
	triangleGenerator.getInput("Frequency")->setDefaultValue(200);

	SawToothComponent sawToothGenerator;
	sawToothGenerator.getInput("Frequency")->setDefaultValue(200);
	//sawToothGenerator.getInput("Amplitude")->setDefaultValue(0.5f);
	//SawToothsawToothGenerator.getInput("Offset")->setDefaultValue(0.5f);


	RandomComponent randomGenerator;


	OutputComponent output;

	// Component renderers
	ComponentRenderer box(100, 100, 200, 100, style);
	ComponentRenderer box2(0, 0, 200, 100, style);
	ComponentRenderer outputRenderer(-100, -200, 200, 100, style);
	view.add(box);
	view.add(box2);
	view.add(outputRenderer);

	box.setComponent(&sinusAttenuation);
	box2.setComponent(&squareGenerator);
	outputRenderer.setComponent(&output);

	bool playing = false;
	bool paused = false;
	float maxZoomIn = 0.1f;
	float maxZoomOut = 10.0f;

	// ///////////////////////////// APPLICATION LOOP
	while (Window::IsOpen())
	{
		prevMousePos = Input::GetMousePosition();
		Window::Update();

		// ///////////////////////// GAME LOGIC
		if (Input::GetKeyDown(sf::Keyboard::Escape))
		{
			Window::Close();
		}

		Window::GetWindow()->setTitle("Sound Generator | FPS: " + floatToStr(Time::GetFps(), 0));

		if (button.click())
		{
			samples.clear();
			//vertices.clear();

			float duration = input.getFloat();

			//float step = Window::GetWidth() / (duration * SAMPLERATE);
			//float frequency = input.getInt();
			//float attenuation = 0.5f;

			sf::Uint64 totalSample = duration * SAMPLERATE;

			for (int i = 0; i < totalSample; i++)
			{
				//samples.push_back(Random::Range(-128, 128));
				float x = (float)i / SAMPLERATE;
				//attenuation = 1.0f;
				/*if (x > fadeout_last || x < fadein_first)
				{
					attenuation = 0;
				}
				else {
					if (x >= fadein_first && x <= fadein_last)
					{
						attenuation *= mapValue(x, fadein_first, fadein_last, 0, 1);
					}
					if (x >= fadeout_first && x <= fadeout_last)
					{
						attenuation *= mapValue(x, fadeout_first, fadeout_last, 1, 0);
					}
				}*/
				//samples.push_back(0x8000 * 0.999f * attenuation * sinf(frequency * 2 * 3.1415926f * x));

				samples.push_back(0x7FFF * clamp(output.getOutput(x), -1.0f, 1.0f));
				//vertices.append(sf::Vertex(sf::Vector2f(i * step, 0.5f * Window::GetWidth() + mapValue(samples[i], -512, 512, -200, 200)), sf::Color::White));
			}
			//buffer.loadFromSamples(samples.data(), SAMPLERATE, 1, SAMPLERATE);
			signal.setData(samples);
			cout << "Signal sample count: " << signal.getSampleCount() << endl;
		}

		if (button1.click())
		{
			signal.getSound()->stop();
			playing = false;
			samples.clear();
			float duration = input.getFloat();
			sf::Uint64 totalSample = duration * SAMPLERATE;
			for (int i = 0; i < totalSample; i++)
			{
				float x = (float)i / SAMPLERATE;
				samples.push_back(0x7FFF * clamp(sinusGenerator.getOutput(x), -1.0f, 1.0f));
			}
			signal.setData(samples);
		}

		if (button2.click())
		{
			signal.getSound()->stop();
			playing = false;
			samples.clear();
			float duration = input.getFloat();
			sf::Uint64 totalSample = duration * SAMPLERATE;
			for (int i = 0; i < totalSample; i++)
			{
				float x = (float)i / SAMPLERATE;
				samples.push_back(0x7FFF * clamp(squareGenerator.getOutput(x), -1.0f, 1.0f));
			}
			signal.setData(samples);
		}

		if (button3.click())
		{
			signal.getSound()->stop();
			playing = false;
			samples.clear();
			float duration = input.getFloat();
			sf::Uint64 totalSample = duration * SAMPLERATE;
			for (int i = 0; i < totalSample; i++)
			{
				float x = (float)i / SAMPLERATE;
				samples.push_back(0x7FFF * clamp(triangleGenerator.getOutput(x), -1.0f, 1.0f));
			}
			signal.setData(samples);
		}

		if (button4.click())
		{
			signal.getSound()->stop();
			playing = false;
			samples.clear();
			float duration = input.getFloat();
			sf::Uint64 totalSample = duration * SAMPLERATE;
			for (int i = 0; i < totalSample; i++)
			{
				float x = (float)i / SAMPLERATE;
				samples.push_back(0x7FFF * clamp(sawToothGenerator.getOutput(x), -1.0f, 1.0f));
			}
			signal.setData(samples);
		}

		if (button5.click())
		{
			signal.getSound()->stop();
			playing = false;
			samples.clear();
			float duration = input.getFloat();
			sf::Uint64 totalSample = duration * SAMPLERATE;
			for (int i = 0; i < totalSample; i++)
			{
				float x = (float)i / SAMPLERATE;
				samples.push_back(0x7FFF * clamp(randomGenerator.getOutput(x), -1.0f, 1.0f));
			}
			signal.setData(samples);
		}

		if (button6.click())
		{
			signal.getSound()->stop();
			playing = false;
			samples.clear();
			float duration = input.getFloat();
			sf::Uint64 totalSample = duration * SAMPLERATE;
			for (int i = 0; i < totalSample; i++)
			{
				float x = (float)i / SAMPLERATE;
				samples.push_back(0x7FFF * clamp(sinusGenerator2.getOutput(x), -1.0f, 1.0f));
			}
			signal.setData(samples);
		}

		if (btnPlay.click() && (!playing || paused))
		{
			float volume = inputVolume.getFloat();
			signal.getSound()->setVolume(volume);
			signal.getSound()->play();
			playing = true;
			paused = false;
		}
		if(btnStop.click() && playing)
		{
			signal.getSound()->stop();
			playing = false;
		}
		if (btnPause.click() && (playing && !paused))
		{
			signal.getSound()->pause();
			paused = true;
		}


		if (view.hovered(Input::GetMousePosition()))
		{
			float delta = Input::GetMouseWheelScroll();
			if (delta != 0)
			{
				view.setZoom(clamp(view.getZoom() * (1.0f - 0.05f * delta), maxZoomIn, maxZoomOut));
			}

			if (Input::GetMouseButton(sf::Mouse::Middle))
			{
				view.setCenter(view.getCenter() - view.mapScreenVectorToView(prevMousePos, Input::GetMousePosition()));
			}
		}

		if (Input::GetMouseButtonDown(sf::Mouse::Left))
		{
			sf::Vector2f mousePos = Input::GetMousePosition();
			//cout << "Mouse position: (" << mousePos.x << "," << mousePos.y << ")" << endl;
		}

		// ///////////////////////// START DRAW
		Window::Clear();


		Window::Draw(); // Draw active layout (with all its children)

		// ///////////////////////// END DRAW
		Window::Display();
	}

	return 0;
}
