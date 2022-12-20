#include "LayerList.h"

#include "../DragObjectIDs.h"
#include "../Editor.h"
#include "../Layer/GridLayer.h"
#include "../Layer/Layer.h"
#include "../Layer/LayerIDs.h"
#include "../GUI/Mouse/DragAndDropObject.h"
#include "../GUI/Mouse/MouseObject.h"
#include "../GUI/Window.h"
#include "../GUI/WindowManager.h"
#include "../Level.h"
#include "../ParseFunctions.h"
#include "../Windows/Layer/LayerInfoWindow.h"
#include "../Windows/WinIDs.h"

#include <stdio.h>

extern Editor* g_editor;

LayerList::LayerList(int x, int y, int w, int h) : ElementList(x, y, w, h) {
    m_firstelementindex = 0;
    m_canselectelement = true;
    m_elementheight = 30;
}



void LayerList::RightClickOn(int clickindex) {
    printf("right click on index %i, layer address %p\n", clickindex, g_editor->m_level->GetLayer(clickindex));

    bool needcreation = true;   //does the window need to be created ?

    for(unsigned int i = 0; i < g_editor->m_winmanager->m_wincount; i++) {
        Window* win = g_editor->m_winmanager->Get(i);
        if(win->m_id == WINID_LAYERINFO) {      //the window exist
            if(((LayerInfoWindow*)win)->m_layerpointer == g_editor->m_level->GetLayer(clickindex)) {
                //the window already exist
                g_editor->m_winmanager->BringOnTop(win);
                needcreation = false;
                break;
            }
        }
    }
    if(needcreation) {      //the window don't exist
        g_editor->m_winmanager->Add(new LayerInfoWindow(g_editor->m_level->GetLayer(clickindex)));
    }

}


void LayerList::LeftClickOn(int clickindex) {
    printf("left click on index %i, layer address %p\n", clickindex, g_editor->m_level->GetLayer(clickindex));
    if(g_mouse->m_x < m_x + m_width - 25) {
        g_editor->m_level->m_selectedlayer = clickindex;

        //for layer d&d
        g_mouse->GiveDragObject(DragAndDropObject(DRAG_OBJECT_LAYER, clickindex, "Layer " + g_editor->m_level->GetLayer(clickindex)->m_name));
    }
    else {
        Layer* curlayer = g_editor->m_level->GetLayer(clickindex);
        curlayer->m_visible = !curlayer->m_visible;
    }

}

void LayerList::LeftReleaseOn(int releaseindex) {
    //type layer
    if(g_mouse->m_dragobject.m_type == DRAG_OBJECT_LAYER) {
        printf("swapping %i and %i\n", releaseindex, g_mouse->m_dragobject.m_data.as_int);
        g_editor->m_level->SwapLayers(releaseindex, g_mouse->m_dragobject.m_data.as_int);
        g_editor->m_level->m_selectedlayer = releaseindex;
    }
    //type texture
    else if(g_mouse->m_dragobject.m_type == DRAG_OBJECT_TEXTURE) {
        g_editor->m_level->GetLayer(releaseindex)->m_havetexture = true;
        g_editor->m_level->GetLayer(releaseindex)->m_textureobj = (TextureObject*) g_mouse->m_dragobject.m_data.as_ptr;
    }
}

void LayerList::DrawElement(int painterx, int paintery, int elementindex) {
    Layer* curlayer = g_editor->m_level->GetLayer(elementindex);

    //Show the layer type on the left
    switch (curlayer->m_type)
    {
        case LAYERID_GRID:
            DrawText("[T]", painterx+5, paintery+10, 10, RED);
        break;
        
        case LAYERID_INSTANCE:
            DrawText("[I]", painterx+5, paintery+10, 10, {159, 187, 204, 255});
        break;

        default:
            DrawText("[?]", painterx+5, paintery+10, 10, GRAY);
        break;
    }

    //Show the layer's name
    if(curlayer->m_name.empty()) {
        DrawText("[no name]", painterx+23, paintery+10, 10, WHITE);
    } else {
        DrawText(curlayer->m_name.c_str(), painterx+23, paintery+10, 10, WHITE);
    }

    //Show if it is visible or hidden
    if(curlayer->m_visible) {
        DrawText("[V]", painterx + m_width - 25, paintery + 10, 10, WHITE);
    }
    else {
        DrawText("[H]", painterx + m_width - 25, paintery + 10, 10, GRAY);
    }
}

int LayerList::GetElementCount() {
    return g_editor->m_level->m_layercount;
}

int LayerList::GetSelectedElement() {
    return g_editor->m_level->m_selectedlayer;
}

void LayerList::SetSelectedElement(int newselection) {
    g_editor->m_level->m_selectedlayer = newselection;
}


void LayerList::PreInputCheck() {
    Level* level = g_editor->m_level;
    //Drag and drop file to list
    if(g_mouse->m_havefiles) {
        if(g_mouse->m_fileslist.count == 1) {
            GridLayer* tempgridlayer = ParseTLMPFile(g_mouse->m_fileslist.paths[0]);

            if(tempgridlayer != NULL) {
                printf("LayerList : yay\n");
                //"Resize" the layer to match the level's size
                GridLayer* finalgridlayer = new GridLayer(level->m_width, level->m_height, tempgridlayer->m_name);

                //copy
                for(int y = 0; y < level->m_height; y++) {
                    for(int x = 0; x < level->m_width; x++) {
                        unsigned short tile = tempgridlayer->GetBoxValue(x, y);
                        if(tile == (unsigned short)-1) tile = 0;

                        finalgridlayer->SetBoxValue(x, y, tile);
                    }
                }

                level->AddLayer(finalgridlayer);
                delete tempgridlayer;
            }
            else {
                printf("LayerList : null :(\n");
            }
        }
    }
}