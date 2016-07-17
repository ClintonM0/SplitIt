#ifndef TEXTINPUT_H
#define TEXTINPUT_H
#include <QLineEdit>
#include <QFocusEvent>
#include <QEvent>

class textInput : public QLineEdit
{
    Q_OBJECT
public:
    explicit textInput(QWidget* parent=0);
    ~textInput();

signals:
    void enable();
    void disable();

protected:
    virtual void focusInEvent(QFocusEvent*);
    virtual void focusOutEvent(QFocusEvent*);
};

#endif // TEXTINPUT_H
