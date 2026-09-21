#include "Game.h"
#include <windows.h>
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#pragma comment(linker, "/ENTRY:mainCRTStartup")
#include "Utils.h"
#include <iostream>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

void showError(const std::string& msg) {
    sf::RenderWindow errorWindow(sf::VideoMode(sf::Vector2u(400, 200)), "Error");
    tgui::Gui errorGui(errorWindow);
    tgui::Font font("resources/wryh.ttf");
    errorGui.setFont(font);

    auto label = tgui::Label::create();
    label->setPosition(20, 50);
    label->setText(msg);
    label->setTextSize(18);
    auto renderer = label->getRenderer();
    renderer->setTextColor(sf::Color::Red);
    errorGui.add(label);

    auto okButton = tgui::Button::create();
    okButton->setPosition(150, 120);
    okButton->setSize(100, 40);
    okButton->setText("OK");
    okButton->onPress([&] {
        errorWindow.close();
        });
    errorGui.add(okButton);

    while (errorWindow.isOpen()) {
        while (const auto optEvent = errorWindow.pollEvent()) {
            if (optEvent->is<sf::Event::Closed>()) {
                errorWindow.close();
            }
            errorGui.handleEvent(*optEvent);
        }
        errorWindow.clear(sf::Color::Black);
        errorGui.draw();
        errorWindow.display();
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(400, 300)), "Cube Attack - Settings");
    tgui::Gui gui(window);
    tgui::Font font("resources/wryh.ttf");
    gui.setFont(font);

    auto editMapSize = tgui::EditBox::create();
    editMapSize->setPosition(50, 50);
    editMapSize->setSize(300, 50);
    editMapSize->setDefaultText("Map Size (4~20)");
    gui.add(editMapSize);

    auto editRounds = tgui::EditBox::create();
    editRounds->setPosition(50, 100);
    editRounds->setSize(300, 30);
    editRounds->setDefaultText("Total Rounds");
    gui.add(editRounds);

    auto editHp = tgui::EditBox::create();
    editHp->setPosition(50, 150);
    editHp->setSize(300, 30);
    editHp->setDefaultText("Defense HP");
    gui.add(editHp);

    auto startButton = tgui::Button::create();
    startButton->setPosition(150, 220);
    startButton->setSize(100, 40);
    startButton->setText("S T A R T");
    gui.add(startButton);

    std::unique_ptr<Game> game = nullptr;
    startButton->onPress([&] {
        int n = tgui::String(editMapSize->getText()).toInt();
        int rounds = tgui::String(editRounds->getText()).toInt();
        int hp = tgui::String(editHp->getText()).toInt();

        if (n < 4 || n > 20) {
            showError("Map size must be between 4 and 20.");
            return;
        }
        if (rounds < 1 || hp < 1) {
            showError("Rounds and HP must be >= 1.");
            return;
        }

        game = std::make_unique<Game>(n, rounds, hp);
        window.close();
        });

    while (window.isOpen()) {
        while (const auto optEvent = window.pollEvent()) {
            if (optEvent->is<sf::Event::Closed>()) {
                window.close();
            }
            gui.handleEvent(*optEvent);
        }
        window.clear(sf::Color::Cyan);
        gui.draw();
        window.display();
    }

    if (game) {
        game->run();
    }

    return 0;
}
