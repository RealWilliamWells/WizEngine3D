//
// Created by william on 11/07/23.
//

#ifndef OSVENGINE_FREEFLYBINDS_H
#define OSVENGINE_FREEFLYBINDS_H

#include <functional>
#include <map>
#include "OSV/input/keyConfigs/configStructs.h"
#include "OSV/input/KeyInputHandler.h"
#include "OSV/rendering/RenderEngine.h"

namespace osv::KeyBinds {
    float delta = 0.f;

    float timeSinceLastPressed = 0.f;
    float pressDelay = 0.5f;

    namespace WindowControl {
        void closeWindow(std::shared_ptr<RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->closeWindow();
        }

        void swapControls(std::shared_ptr<RenderEngine> renderEngine, bool delayPress, float delta) {
            if (delayPress)
                return;

            KeyInputHandler::currentSwitchingBind++;

            KeyInputHandler::currentSwitchingBind = KeyInputHandler::currentSwitchingBind >=
                    KeyInputHandler::switchingInputs.size() ? 0 : KeyInputHandler::currentSwitchingBind;
            renderEngine->setCursorPosCallback(KeyInputHandler::switchingInputs.at(KeyInputHandler::currentSwitchingBind).mousePosCallback);
        }

        void setPointsRendering(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->setRenderOverrideMode(GL_POINTS);
        }

        void setLinesRendering(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->setRenderOverrideMode(GL_LINES);
        }

        void setTrianglesRendering(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->setRenderOverrideMode(GL_TRIANGLES);
        }

        void releaseMouse(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            if (delayPress)
                return;

            renderEngine->toggleMouseRelease();
        }
    }

    InputMode generateWindowBinds() {
        InputMode windowInputs;

#ifndef OS_SWITCH
        windowInputs.keyBinds[GLFW_KEY_ESCAPE].keyActionCallback = WindowControl::closeWindow;
        windowInputs.keyBinds[GLFW_KEY_Q].keyActionCallback = WindowControl::swapControls;

        windowInputs.keyBinds[GLFW_KEY_P].keyActionCallback = WindowControl::setPointsRendering;
        windowInputs.keyBinds[GLFW_KEY_L].keyActionCallback = WindowControl::setLinesRendering;
        windowInputs.keyBinds[GLFW_KEY_T].keyActionCallback = WindowControl::setTrianglesRendering;

#ifndef __EMSCRIPTEN__
        windowInputs.keyBinds[GLFW_KEY_LEFT_CONTROL].keyActionCallback = WindowControl::releaseMouse;
#endif
#endif

        // Joystick controls
        windowInputs.joyButtonBinds[GLFW_GAMEPAD_BUTTON_START].keyActionCallback = WindowControl::closeWindow;
        windowInputs.joyButtonBinds[GLFW_GAMEPAD_BUTTON_BACK].keyActionCallback = WindowControl::swapControls;

//        windowInputs.joyButtonBinds[GLFW_GAMEPAD_BUTTON_DPAD_LEFT].keyActionCallback = WindowControl::setPointsRendering;
        windowInputs.joyButtonBinds[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT].keyActionCallback = WindowControl::setLinesRendering;
        windowInputs.joyButtonBinds[GLFW_GAMEPAD_BUTTON_DPAD_LEFT].keyActionCallback = WindowControl::setTrianglesRendering;

        return windowInputs;
    }

    namespace EditModeControl {
        unsigned int selectedModel = 1;
        float currentScale = 1.f;

        void swapModels(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            if (delayPress) {
                return;
            }

            const std::vector<Model> &models = renderEngine->getModels();

            selectedModel++;
            selectedModel = selectedModel >= models.size() ? 0 : selectedModel;

            currentScale = 1.f;
        }

        void scaleUp(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            currentScale += .25f * delta;

            renderEngine->relativeScaleModel(selectedModel, {currentScale, currentScale, currentScale});
        }

        void scaleDown(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            currentScale -= .25f * delta;

            renderEngine->relativeScaleModel(selectedModel, {currentScale, currentScale, currentScale});
        }

        void rotateLeft(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            if (delayPress) {
                return;
            }

            float rotation = 8.f * delta;

            renderEngine->rotateModel(selectedModel, rotation, {0.f, 1.f, 0.f});
        }

        void rotateRight(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            if (delayPress) {
                return;
            }

            float rotation = -8.f * delta;

            renderEngine->rotateModel(selectedModel, rotation, {0.f, 1.f, 0.f});
        }

        void moveRight(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->translateModel(selectedModel,{.5f*delta, 0.f, 0.f});
        }

        void moveLeft(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->translateModel(selectedModel,{-.5f*delta, 0.f, 0.f});
        }

        void moveUp(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->translateModel(selectedModel,{0.f, 0.f, .5f*delta});
        }

        void moveDown(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->translateModel(selectedModel,{0.f, 0.f, -.5f*delta});
        }

        void randPos(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            if (delayPress)
                return;

            renderEngine->setModelPos(selectedModel, {rand() % 20 - 10, 0.f, rand() % 20 - 10});
        }

        // World orientation control keyBinds
        void orientateRight(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->orientateWorld(1.f * delta, {0.0f, 1.f, .0f});
        }

        void orientateLeft(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->orientateWorld(-1.f * delta, {0.f, 1.f, .0f});
        }

        void orientateUp(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->orientateWorld(1.f * delta, {1.f, 0.f, .0f});
        }

        void orientateDown(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->orientateWorld(-1.f * delta, {1.f, 0.f, .0f});
        }

        void resetOrientation(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->resetWorldOrientation();
        }
    }

    InputMode generateEditModeBinds() {
        InputMode modelBinds;

#ifndef OS_SWITCH
        modelBinds.keyBinds[GLFW_KEY_E].keyActionCallback = EditModeControl::swapModels;

        modelBinds.keyBinds[GLFW_KEY_U].keyActionCallback = EditModeControl::scaleUp;
        modelBinds.keyBinds[GLFW_KEY_J].keyActionCallback = EditModeControl::scaleDown;

        modelBinds.keyBinds[GLFW_KEY_Z].keyActionCallback = EditModeControl::rotateLeft;
        modelBinds.keyBinds[GLFW_KEY_X].keyActionCallback = EditModeControl::rotateRight;

        modelBinds.keyBinds[GLFW_KEY_D].keyActionCallback = EditModeControl::moveRight;
        modelBinds.keyBinds[GLFW_KEY_A].keyActionCallback = EditModeControl::moveLeft;
        modelBinds.keyBinds[GLFW_KEY_W].keyActionCallback = EditModeControl::moveUp;
        modelBinds.keyBinds[GLFW_KEY_S].keyActionCallback = EditModeControl::moveDown;

        modelBinds.keyBinds[GLFW_KEY_SPACE].keyActionCallback = EditModeControl::randPos;

        // World orientation control keybinds
        modelBinds.keyBinds[GLFW_KEY_RIGHT].keyActionCallback = EditModeControl::orientateRight;
        modelBinds.keyBinds[GLFW_KEY_LEFT].keyActionCallback = EditModeControl::orientateLeft;
        modelBinds.keyBinds[GLFW_KEY_UP].keyActionCallback = EditModeControl::orientateUp;
        modelBinds.keyBinds[GLFW_KEY_DOWN].keyActionCallback = EditModeControl::orientateDown;

        modelBinds.keyBinds[GLFW_KEY_HOME].keyActionCallback = EditModeControl::resetOrientation;
#endif

        // Joystick controls
        modelBinds.joyButtonBinds[GLFW_GAMEPAD_BUTTON_DPAD_UP].keyActionCallback = EditModeControl::swapModels;

//        modelBinds.keyBinds[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER].keyActionCallback = EditModeControl::scaleUp;
//        modelBinds.keyBinds[GLFW_KEY_J].keyActionCallback = EditModeControl::scaleDown;

//        modelBinds.keyBinds[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER].keyActionCallback = EditModeControl::rotateLeft;
//        modelBinds.keyBinds[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER].keyActionCallback = EditModeControl::rotateRight;

//        modelBinds.joyButtonBinds[GLFW_GAMEPAD_BUTTON_A].keyActionCallback = EditModeControl::moveRight;
        modelBinds.joyButtonBinds[GLFW_GAMEPAD_BUTTON_Y].keyActionCallback = EditModeControl::moveLeft;
        modelBinds.joyButtonBinds[GLFW_GAMEPAD_BUTTON_X].keyActionCallback = EditModeControl::moveUp;
        modelBinds.joyButtonBinds[GLFW_GAMEPAD_BUTTON_B].keyActionCallback = EditModeControl::moveDown;

        modelBinds.joyButtonBinds[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB].keyActionCallback = EditModeControl::randPos;

        // World orientation control keybinds
        modelBinds.joyAxisBinds[GLFW_GAMEPAD_AXIS_LEFT_X].joyStickDir = true;
        modelBinds.joyAxisBinds[GLFW_GAMEPAD_AXIS_LEFT_X].keyActionCallback = EditModeControl::orientateRight;

        modelBinds.joyAxisBinds[GLFW_GAMEPAD_AXIS_LEFT_X].joyStickDir = false;
        modelBinds.joyAxisBinds[GLFW_GAMEPAD_AXIS_LEFT_X].keyActionCallback = EditModeControl::orientateLeft;

        modelBinds.joyAxisBinds[GLFW_GAMEPAD_AXIS_LEFT_Y].joyStickDir = true;
        modelBinds.joyAxisBinds[GLFW_GAMEPAD_AXIS_LEFT_Y].keyActionCallback = EditModeControl::orientateUp;

        modelBinds.joyAxisBinds[GLFW_GAMEPAD_AXIS_LEFT_Y].joyStickDir = false;
        modelBinds.joyAxisBinds[GLFW_GAMEPAD_AXIS_LEFT_Y].keyActionCallback = EditModeControl::orientateDown;

        modelBinds.joyButtonBinds[GLFW_GAMEPAD_BUTTON_LEFT_THUMB].keyActionCallback = EditModeControl::resetOrientation;

        modelBinds.mousePosCallback = MouseInput::editModeInputCallback;

        return modelBinds;
    }

    namespace CameraControl {
        void moveCamForward(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            Camera* camera = renderEngine->getCamera();
            camera->moveFrontAndBack(camera->getDefaultMoveSpeed());
        }

        void moveCamBack(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            Camera* camera = renderEngine->getCamera();
            camera->moveFrontAndBack(-camera->getDefaultMoveSpeed());
        }

        void moveCamLeft(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            Camera* camera = renderEngine->getCamera();
            camera->moveSideways(-camera->getDefaultMoveSpeed());
        }

        void moveCamRight(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            Camera* camera = renderEngine->getCamera();
            camera->moveSideways(camera->getDefaultMoveSpeed());
        }

        void moveCamUp(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            Camera* camera = renderEngine->getCamera();
            camera->moveUpAndDown(camera->getDefaultMoveSpeed());
        }

        void moveCamDown(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            Camera* camera = renderEngine->getCamera();
            camera->moveUpAndDown(-camera->getDefaultMoveSpeed());
        }
    }

    InputMode generateFreeFlyBinds() {
        InputMode freeFly;

#ifndef OS_SWITCH
        freeFly.keyBinds[GLFW_KEY_W].keyActionCallback = CameraControl::moveCamForward;
        freeFly.keyBinds[GLFW_KEY_S].keyActionCallback = CameraControl::moveCamBack;

        freeFly.keyBinds[GLFW_KEY_A].keyActionCallback = CameraControl::moveCamLeft;
        freeFly.keyBinds[GLFW_KEY_D].keyActionCallback = CameraControl::moveCamRight;

        freeFly.keyBinds[GLFW_KEY_LEFT_SHIFT].keyActionCallback = CameraControl::moveCamUp;
        freeFly.keyBinds[GLFW_KEY_SPACE].keyActionCallback = CameraControl::moveCamDown;
#endif

        // Joystick control
        freeFly.joyButtonBinds[GLFW_GAMEPAD_BUTTON_X].keyActionCallback = CameraControl::moveCamForward;
        freeFly.joyButtonBinds[GLFW_GAMEPAD_BUTTON_B].keyActionCallback = CameraControl::moveCamBack;

        freeFly.joyButtonBinds[GLFW_GAMEPAD_BUTTON_Y].keyActionCallback = CameraControl::moveCamLeft;
        freeFly.joyButtonBinds[GLFW_GAMEPAD_BUTTON_A].keyActionCallback = CameraControl::moveCamRight;

        freeFly.joyButtonBinds[GLFW_GAMEPAD_BUTTON_DPAD_UP].keyActionCallback = CameraControl::moveCamUp;
        freeFly.joyButtonBinds[GLFW_GAMEPAD_BUTTON_DPAD_DOWN].keyActionCallback = CameraControl::moveCamDown;

        freeFly.mousePosCallback = MouseInput::freeFlyInputCallback;

        return freeFly;
    }
}

#endif //OSVENGINE_FREEFLYBINDS_H
