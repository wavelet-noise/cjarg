#include "mouse.h"
#include <glm/glm.hpp>

GLFWwindow *Mouse::sm_window = nullptr;

double Mouse::sm_xpos = 0;
double Mouse::sm_ypos = 0;

double Mouse::sm_dxpos = 0;
double Mouse::sm_dypos = 0;

unsigned int Mouse::sm_windowWidth = 0;
unsigned int Mouse::sm_windowHeight = 0;

bool Mouse::sm_stateFixedMousePos = false;
bool Mouse::sm_isCursorClientArea = false;
bool Mouse::sm_isWindowFocused = false;

double Mouse::offset = 0;
double Mouse::last_offset = 0;

void Mouse::Initialize(GLFWwindow *win )
{
    sm_window = win;
    sm_xpos = 1.0;
    sm_ypos = 1.0;
    sm_dxpos = 0.0;
    sm_dypos = 0.0;
    sm_windowWidth = 0;
    sm_windowHeight = 0;
    sm_stateFixedMousePos = false;
    sm_isCursorClientArea = true;
    sm_isWindowFocused = true;

    glfwSetCursorPos(sm_window, sm_xpos, sm_ypos);
}


void Mouse::SetButton( int button, int state ,int c )
{
    Mouse::sm_buttons[button] = state;
}

void Mouse::Scroll(double a){
    offset += a;
}

void Mouse::SetCursorPos( double x, double y )
{
    if(!sm_isWindowFocused)
        return;

    sm_dxpos = x - sm_xpos;
    sm_dypos = y - sm_ypos;
    sm_deltaxpos = sm_dxpos;
    sm_deltaypos = sm_dypos;
    sm_lastxpos = sm_xpos;
    sm_lastypos = sm_ypos;

    if(!sm_isCursorClientArea)
    {
        sm_dxpos = 0.0;
        sm_dypos = 0.0;
    }

    if(sm_stateFixedMousePos)
    {
        sm_xpos = double(sm_windowWidth) / 2.0;
        sm_ypos = double(sm_windowHeight) / 2.0;
        glfwSetCursorPos(sm_window, sm_xpos, sm_ypos);
    }
    else
    {
        sm_xpos = x;
        sm_ypos = y;
    }
}

double Mouse::IsMoveCursorX()
{
    double dx = sm_dxpos;
    sm_dxpos = 0.0;
    return dx;
}

double Mouse::IsMoveCursorY()
{
    double dy = sm_dypos;
    sm_dypos = 0.0;
    return dy;
}

void Mouse::GetCursorPos( double &x, double &y )
{
    x = sm_xpos;
    y = sm_ypos;
}

glm::vec2 Mouse::GetCursorPos()
{
    return glm::vec2(sm_xpos, sm_ypos);
}

void Mouse::SetFixedPosState( bool state )
{
    sm_stateFixedMousePos = state;
    if(sm_stateFixedMousePos)
    {
        sm_xpos = double(sm_windowWidth) / 2.0;
        sm_ypos = double(sm_windowHeight) / 2.0;
        sm_dxpos = 0.0;
        sm_dypos = 0.0;
        glfwSetCursorPos(sm_window, sm_xpos, sm_ypos);
    }
}

void Mouse::SetWindowSize( unsigned int width, unsigned int height )
{
    sm_windowWidth = width;
    sm_windowHeight = height;
}

void Mouse::CursorClientArea( int entered )
{
    if(entered == GL_TRUE)
    {
        sm_isCursorClientArea = true;
        glfwGetCursorPos(sm_window, &sm_xpos, &sm_ypos);
    }
    else
    {
        sm_isCursorClientArea = false;
    }
}

//  reset mouse delta
void Mouse::resetDelta(){
    sm_deltaypos = 0;
    sm_deltaxpos = 0;
    last_offset = offset;
}

bool Mouse::isWheelUp()
{
    return offset > last_offset;
}

bool Mouse::isWheelDown()
{
    return offset < last_offset;
}

void Mouse::WindowFocus( int focused )
{
    if(focused == GL_TRUE)
    {
        sm_isWindowFocused = true;
        glfwGetCursorPos(sm_window, &sm_xpos, &sm_ypos);
    }
    else
    {
        sm_isWindowFocused = false;
    }
}

bool Mouse::IsRightPressed()
{
    if(sm_buttons[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS) {
        sm_buttons[GLFW_MOUSE_BUTTON_RIGHT] = GLFW_REPEAT;
        return true;
    }
    return false;
}

bool Mouse::IsLeftDown()
{
    return sm_buttons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS || sm_buttons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_REPEAT;
}

bool Mouse::IsRightDown()
{
    return sm_buttons[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS || sm_buttons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_REPEAT;
}

bool Mouse::IsLeftPressed()
{
    if(sm_buttons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS) {
        sm_buttons[GLFW_MOUSE_BUTTON_LEFT] = GLFW_REPEAT;
        return true;
    }
    return false;
}

glm::vec2 Mouse::GetCursorDelta()
{
    return glm::vec2(sm_deltaxpos, sm_deltaypos);
}

glm::vec2 Mouse::GetCursorLastPos()
{
    return glm::vec2(sm_lastxpos, sm_lastypos);
}

double Mouse::sm_lastypos;
double Mouse::sm_lastxpos;
double Mouse::sm_deltaypos;
double Mouse::sm_deltaxpos;
int Mouse::sm_buttons[10];

