#include "ToolSub.h"
#include "State.h"
ToolSub * toolSub;

ToolSub::~ToolSub()
{
}

void ToolSub::init()
{
    if (!toolSub)
    {
        toolSub = new ToolSub();
    }
}

ToolSub *ToolSub::get()
{
    return toolSub;
}

bool ToolSub::OnMouseDown(int x, int y)
{
    return false;
}

bool ToolSub::OnMouseUp(int x, int y)
{
    return false;
}

bool ToolSub::OnMouseMove(int x, int y)
{
    return false;
}

bool ToolSub::OnPaint(SkCanvas *base, SkCanvas *board, SkCanvas *canvas)
{
    return false;
}

bool ToolSub::OnPaintFinish(SkCanvas *base)
{

    return false;
}

ToolSub::ToolSub()
{
}
