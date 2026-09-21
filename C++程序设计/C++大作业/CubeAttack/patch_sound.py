import re

with open("Game.cpp", "r", encoding="utf-8") as f:
    c = f.read()

old = '    if (!m_loseTexture.loadFromFile("resources/lose.png")) {'
new = '    // load sounds\n    if (!m_winSoundBuffer.loadFromFile("resources/win.mp3")) {\n        std::cerr << "Failed win.mp3\\n";\n    } else { m_winSound.setBuffer(m_winSoundBuffer); }\n    if (!m_loseSoundBuffer.loadFromFile("resources/lose.wav")) {\n        std::cerr << "Failed lose.wav\\n";\n    } else { m_loseSound.setBuffer(m_loseSoundBuffer); }\n\n' + old
c = c.replace(old, new)

c = c.replace(
    "void Game::showGameOverWindow() {\n    const sf::Texture*",
    "void Game::showGameOverWindow() {\n    if (m_gameWon) m_winSound.play(); else m_loseSound.play();\n\n    const sf::Texture*"
)

with open("Game.cpp", "w", encoding="utf-8") as f:
    f.write(c)
print("Game.cpp patched")
