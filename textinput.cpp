#include <textinput.h>
#include <iostream>
#include <splitit.h>

using namespace std;

textInput::textInput(QWidget* parent) : QLineEdit(parent)
{
    setMouseTracking(true);
}

textInput::~textInput()
{
}

void textInput::focusInEvent(QFocusEvent*)
{
    emit enable();
}

void textInput::focusOutEvent(QFocusEvent*)
{
    emit disable();
}
