﻿#include "ShapeNumber.h"
#include "../WindowMain.h"
#include "../ToolSub.h"
#include "../AppFont.h"
#include "ShapeDragger.h"
#include "numbers"
static int num = 1;

ShapeNumber::ShapeNumber(const int &x, const int &y) : ShapeBase(x, y)
{
    initParams();
}

ShapeNumber::~ShapeNumber()
{
    if (num > number) {
        num = number;
    }
}

bool ShapeNumber::OnMouseDown(const int &x, const int &y)
{
    hoverX = x;
    hoverY = y;
    return false;
}

bool ShapeNumber::OnMouseUp(const int &x, const int &y)
{
    auto win = WindowMain::get();
    auto canvasBack = win->surfaceBack->getCanvas();
    Paint(canvasBack);
    isWip = false;
    setDragger();
    return false;
}

bool ShapeNumber::OnMouseMove(const int &x, const int &y)
{
    auto flag = isMouseOver(x, y);
    if (flag)
    {
        auto draggers = ShapeDragger::get();
        setDragger();
        Icon::myCursor(Icon::cursor::all);
        HoverIndex = 8;
        return true;
    }
    return false;
}

bool ShapeNumber::OnMoseDrag(const int &x, const int &y)
{
    if (HoverIndex == 0)
    {
        endX = x;
        endY = y;
        makePath(startX, startY, endX, endY);
    }
    else
    {
        auto xSpan = x - hoverX;
        auto ySpan = y - hoverY;
        startX += xSpan;
        startY += ySpan;
        endX += xSpan;
        endY += ySpan;
        makePath(startX, startY, endX, endY);
        hoverX = x;
        hoverY = y;
    }
    auto win = WindowMain::get();
    auto canvas = win->surfaceFront->getCanvas();
    canvas->clear(SK_ColorTRANSPARENT);
    Paint(canvas);
    WindowMain::get()->Refresh();
    return false;
}

void ShapeNumber::Paint(SkCanvas *canvas)
{
    SkPaint paint;
    paint.setAntiAlias(true);
    if (stroke)
    {
        paint.setStroke(true);
        paint.setStrokeWidth(strokeWidth);
    }
    paint.setColor(color);
    canvas->drawPath(path, paint);
    canvas->drawCircle(SkPoint::Make(startX, startY), r, paint);
    auto str = std::to_string(number);
    auto font = AppFont::Get()->fontText;
    font->setSize(r);
    paint.setColor(SK_ColorWHITE);
    SkRect textBounds;
    font->measureText(str.c_str(), str.size(), SkTextEncoding::kUTF8, &textBounds);
    SkScalar x = startX - textBounds.width() / 2 - textBounds.left();
    SkScalar y = startY + textBounds.height() / 2 - textBounds.bottom();
    canvas->drawSimpleText(str.c_str(), str.size(), SkTextEncoding::kUTF8, x, y, *font, paint);
}

bool ShapeNumber::isMouseOver(const int &x, const int &y)
{
    auto flag1 = path.contains(x, y);
    if (flag1) {
        return true;
    }
    SkPath pathCircle;
    pathCircle.addCircle(startX, startY, r);
    auto flag2 = pathCircle.contains(x, y);
    if (flag2) {
        return true;
    }
    return false;
}

void ShapeNumber::initParams()
{
    number = num;
    num += 1;
    HoverIndex = 0;
    auto tool = ToolSub::get();
    stroke = !tool->getFill();
    if (stroke)
    {
        auto stroke = tool->getStroke();
        if (stroke == 1)
        {
            strokeWidth = 4;
        }
        else if (stroke == 2)
        {
            strokeWidth = 8;
        }
        else
        {
            strokeWidth = 16;
        }
    }
    color = tool->getColor();
    draggers.push_back(SkRect::MakeEmpty());
}

void ShapeNumber::makePath(const int &x1, const int &y1, const int &x2, const int &y2)
{
    path.reset();
    SkPoint arrowPoint, centerPoint;
    arrowPoint.fX = x2; //箭头顶点
    arrowPoint.fY = y2;
    centerPoint.fX = x1; //圆心
    centerPoint.fY = y1;
    auto x = x2 - x1;
    auto y = y1 - y2;
    r = std::sqrt(x * x + y * y);//圆心到箭头顶点的长度
    auto height = r / 3.0f; //箭头高度
    r = r - height; //半径
    auto radint = std::atan2(y, x); //反正切
    auto angle = radint * 180 / std::numbers::pi; //角度
    auto angleSpan = 16.f; //半角
    auto angle1 = (angle + angleSpan) * std::numbers::pi / 180;//弧度
    auto angle2 = (angle - angleSpan) * std::numbers::pi / 180;//弧度
    auto X1 = centerPoint.fX + r * cos(angle1);//箭头与圆的交接点1
    auto Y1 = centerPoint.fY - r * sin(angle1);
    auto X2 = centerPoint.fX + r * cos(angle2);//箭头与圆的交接点2
    auto Y2 = centerPoint.fY - r * sin(angle2);
    path.moveTo(arrowPoint.fX, arrowPoint.fY);
    path.lineTo(X1, Y1);
    path.lineTo(X2, Y2);
    path.close();
    path.setFillType(SkPathFillType::kWinding);
}

void ShapeNumber::setDragger()
{
    auto shapeDragger = ShapeDragger::get();
    unsigned half = shapeDragger->size / 2;
    shapeDragger->setDragger(0, endX - half, endY - half);
    shapeDragger->cursors[0] = Icon::cursor::all;
    for (size_t i = 1; i < 8; i++)
    {
        shapeDragger->setDragger(i, -100, -100);
    }
}
