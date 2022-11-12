#include "ObjectListWindow.h"

#include "../../Editor.h"
#include "../../GUI/Widget/Button.h"
#include "../../GUI/Widget/WidgetManager.h"
#include "../../GUI/WindowManager.h"
#include "../../Widget/ObjectList.h"
#include "../WinIDs.h"
#include "NewObjectWindow.h"

extern Editor* g_editor;

void NewObjectButtonAction(Button*) {
    //printf("placeholder: create object here.\n");
    Window* winptr = g_editor->m_winmanager->FindWithID(WINID_NEWOBJECT);
    if(winptr == NULL) {
        g_editor->m_winmanager->Add(new NewObjectWindow());
    }
    else {
        g_editor->m_winmanager->BringOnTop(winptr);
    }
}

ObjectListWindow::ObjectListWindow() : Window() {
    m_id = WINID_OBJECTLIST;
    SetPosition(100, 100);

    m_titlebarcolor = {46, 41, 78, 255};
    m_titlebartext = "Object list";

    m_width = 200;
    m_height = 200;

    m_widgetmanager->Add(new ObjectList(3, 15, 194, 150));

    Button* newbut = new Button(3, 168, 50, 15);
    newbut->SetText("New");
    newbut->SetAction(NewObjectButtonAction);
    m_widgetmanager->Add(newbut);
}