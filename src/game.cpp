#include "game.hpp"
#include "game_state.hpp"

namespace kg {
    Game::Game(int width, int height, std::string title) {
        _data->window.create(sf::VideoMode(width, height), title);

        _data->machine.addState(StateRef(new GameState(_data)));
        run();
    }

    void Game::run() {
        float newTime, frameTime, interpolation;

        float currentTime = _clock.getElapsedTime().asSeconds();
        float accumulator = 0.0f;

        _upsClock.restart();
        _fpsClock.restart();

        while (_data->window.isOpen()) {
            newTime = _clock.getElapsedTime().asSeconds();
            frameTime = newTime - currentTime;

            if (frameTime > 0.25f) {
                frameTime = 0.25f;
            }

            currentTime = newTime;
            accumulator += frameTime;

            while (accumulator >= dt) {
#ifdef DEBUG
                //****************************
                // This is just to display UPS
                _updates++;
                float time = _upsClock.getElapsedTime().asSeconds();
                if (time >= 1.0f) {
                    _data->UPS = _updates;

                    _upsClock.restart();
                    _updates = 0;
                }
                //****************************
#endif
                _data->machine.getActiveState()->handleInput();
                _data->machine.getActiveState()->update(dt);

                accumulator -= dt;
            }

#ifdef DEBUG
            //****************************
            // This is just to display FPS
            _frames++;
            float time = _fpsClock.getElapsedTime().asSeconds();
            if (time >= 1.0f) {
                _data->FPS = _frames;

                _fpsClock.restart();
                _frames = 0;
            }
            //****************************
#endif
            interpolation = accumulator / dt;
            _data->machine.getActiveState()->render(interpolation);

            _data->machine.checkStates();
        }
    }
}
