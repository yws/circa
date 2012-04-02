
#include "circa/circa.h"

#include <QBrush>
#include <QPen>
#include <QFont>
#include <QFontMetrics>
#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>

void BrushRelease(caValue* value)
{
    delete (QBrush*) circ_get_pointer(value);
}
void PenRelease(caValue* value)
{
    delete (QPen*) circ_get_pointer(value);
}
void FontMetricsRelease(caValue* value)
{
    delete (QFontMetrics*) circ_get_pointer(value);
}
void FontRelease(caValue* value)
{
    delete (QFont*) circ_get_pointer(value);
}
void LinearGradientRelease(caValue* value)
{
    delete (QLinearGradient*) circ_get_pointer(value);
}
void PainterPathRelease(caValue* value)
{
    delete (QPainterPath*) circ_get_pointer(value);
}

QPointF to_qpoint(caValue* value)
{
    float x,y;
    circ_get_vec2(value, &x, &y);
    return QPoint(x,y);
}

QColor to_qcolor(caValue* value)
{
    float r,g,b,a;
    circ_get_vec4(value, &r, &g, &b, &a);
    return QColor(r * 255, g * 255, b * 255, a * 255);
}

QRectF to_qrect(caValue* value)
{
    float x,y,w,h;
    circ_get_vec4(value, &x, &y, &w, &h);
    return QRectF(x,y,w,h);
}

void create_brush(caStack* stack)
{
    caValue* out = circ_create_default_output(stack, 0);
    circ_handle_set_object(out, new QBrush(), BrushRelease);
}
void create_pen(caStack* stack)
{
    caValue* c = circ_input(stack, 0);
    float width = circ_get_float(circ_input(stack, 1));
    caValue* style = circ_input(stack, 2);

    QPen* pen = new QPen();
    pen->setColor(to_qcolor(c));
    pen->setWidth(width);
    style; // TODO: support style

    caValue* out = circ_create_default_output(stack, 0);
    circ_handle_set_object(out, new QPen(), PenRelease);
}
void Pen__setStyle(caStack* stack)
{
    QPen* pen = (QPen*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    caValue* style = circ_input(stack, 1);

    pen; style; // TODO: support style
}
void Pen__setWidth(caStack* stack)
{
    QPen* pen = (QPen*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    float width = circ_get_float(circ_input(stack, 1));

    pen->setWidth(width);
}
void Pen__setDashPattern(caStack* stack)
{
    QPen* pen = (QPen*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    caValue* dashes = circ_input(stack, 1);
}
void FontMetrics__width(caStack* stack)
{
    QFontMetrics* fontMetrics = (QFontMetrics*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    const char* text = circ_get_string(circ_input(stack, 1));

    circ_set_float(circ_output(stack, 0), fontMetrics->width(text));
}
void create_font(caStack* stack)
{
    const char* name = circ_get_string(circ_input(stack, 0));
    float size = circ_get_float(circ_input(stack, 1));

    QFont* font = new QFont(name, int(size));

    caValue* out = circ_create_default_output(stack, 0);
    circ_handle_set_object(out, font, FontRelease);
}
void Font__setPixelSize(caStack* stack)
{
    QFont* font = (QFont*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    float size = circ_get_float(circ_input(stack, 1));

    font->setPixelSize(size);
}
void Font__fontMetrics(caStack* stack)
{
    QFont* font = (QFont*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));

    QFontMetrics* metrics = new QFontMetrics(*font);

    caValue* out = circ_create_default_output(stack, 0);
    circ_handle_set_object(out, metrics, FontMetricsRelease);
}
void PainterPath__moveTo(caStack* stack)
{
    QPainterPath* painterPath = (QPainterPath*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    caValue* p = circ_input(stack, 1);

    painterPath->moveTo(to_qpoint(p));
}
void PainterPath__cubicTo(caStack* stack)
{
    QPainterPath* painterPath = (QPainterPath*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    caValue* a = circ_input(stack, 1);
    caValue* b = circ_input(stack, 2);
    caValue* c = circ_input(stack, 3);

    painterPath->cubicTo(to_qpoint(a), to_qpoint(b), to_qpoint(c));
}
void PainterPath__addText(caStack* stack)
{
    QPainterPath* painterPath = (QPainterPath*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    caValue* p = circ_input(stack, 1);
    QFont* f = (QFont*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 2)));
    const char* text = circ_get_string(circ_input(stack, 3));

    painterPath->addText(to_qpoint(p), *f, text);
}
void create_linear_gradient(caStack* stack)
{
    caValue* start = circ_input(stack, 0);
    caValue* stop = circ_input(stack, 1);
    caValue* out = circ_create_default_output(stack, 0);

    QLinearGradient* gradient = new QLinearGradient(to_qpoint(start), to_qpoint(stop));

    circ_handle_set_object(out, gradient, LinearGradientRelease);
}
void LinearGradient__setColorAt(caStack* stack)
{
    QLinearGradient* linearGradient = (QLinearGradient*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    float ratio = circ_get_float(circ_input(stack, 1));
    caValue* c = circ_input(stack, 2);

    linearGradient->setColorAt(ratio, to_qcolor(c));
}
void Painter__save(caStack* stack)
{
    QPainter* painter = (QPainter*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    painter->save();
}
void Painter__restore(caStack* stack)
{
    QPainter* painter = (QPainter*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    painter->restore();
}
void Painter__setBrush(caStack* stack)
{
    QPainter* painter = (QPainter*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    QBrush* brush = (QBrush*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 1)));

    painter->setBrush(*brush);
}
void Painter__setPen(caStack* stack)
{
    QPainter* painter = (QPainter*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    QPen* pen = (QPen*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 1)));

    painter->setPen(*pen);
}
void Painter__rotate(caStack* stack)
{
    QPainter* painter = (QPainter*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    float radians = circ_get_float(circ_input(stack, 1));

    painter->rotate(radians);
}
void Painter__translate(caStack* stack)
{
    QPainter* painter = (QPainter*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    caValue* delta = circ_input(stack, 1);

    painter->translate(to_qpoint(delta));
}
void Painter__drawEllipse(caStack* stack)
{
    QPainter* painter = (QPainter*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    caValue* r = circ_input(stack, 1);

    painter->drawEllipse(to_qrect(r));
}
void Painter__drawText(caStack* stack)
{
    QPainter* painter = (QPainter*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    caValue* r = circ_input(stack, 1);
    caValue* align = circ_input(stack, 2);
    const char* text = circ_get_string(circ_input(stack, 3));

    painter->drawText(to_qrect(r), 0, text);
}
void Painter__drawLine(caStack* stack)
{
    QPainter* painter = (QPainter*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    caValue* a = circ_input(stack, 1);
    caValue* b = circ_input(stack, 2);

    painter->drawLine(to_qpoint(a), to_qpoint(b));
}
void Painter__drawRect(caStack* stack)
{
    QPainter* painter = (QPainter*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    caValue* r = circ_input(stack, 1);

    painter->drawRect(to_qrect(r));
}
void Painter__drawPath(caStack* stack)
{
    QPainter* painter = (QPainter*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    QPainterPath* path = (QPainterPath*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 1)));

    painter->drawPath(*path);
}
void Painter__drawRoundRect(caStack* stack)
{
    QPainter* painter = (QPainter*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    caValue* r = circ_input(stack, 1);

    painter->drawRoundRect(to_qrect(r));
}
void Painter__fillRect(caStack* stack)
{
    QPainter* painter = (QPainter*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 0)));
    caValue* r = circ_input(stack, 1);
    QBrush* b = (QBrush*) circ_get_pointer(circ_handle_get_value(circ_input(stack, 2)));

    painter->fillRect(to_qrect(r), *b);
}
static const caFunctionBinding IMPORTS[] = {
    {"create_brush", create_brush},
    {"create_pen", create_pen},
    {"Pen.setStyle", Pen__setStyle},
    {"Pen.setWidth", Pen__setWidth},
    {"Pen.setDashPattern", Pen__setDashPattern},
    {"FontMetrics.width", FontMetrics__width},
    {"create_font", create_font},
    {"Font.setPixelSize", Font__setPixelSize},
    {"Font.fontMetrics", Font__fontMetrics},
    {"PainterPath.moveTo", PainterPath__moveTo},
    {"PainterPath.cubicTo", PainterPath__cubicTo},
    {"PainterPath.addText", PainterPath__addText},
    {"create_linear_gradient", create_linear_gradient},
    {"LinearGradient.setColorAt", LinearGradient__setColorAt},
    {"Painter.save", Painter__save},
    {"Painter.restore", Painter__restore},
    {"Painter.setBrush", Painter__setBrush},
    {"Painter.setPen", Painter__setPen},
    {"Painter.rotate", Painter__rotate},
    {"Painter.translate", Painter__translate},
    {"Painter.drawEllipse", Painter__drawEllipse},
    {"Painter.drawText", Painter__drawText},
    {"Painter.drawLine", Painter__drawLine},
    {"Painter.drawRect", Painter__drawRect},
    {"Painter.drawPath", Painter__drawPath},
    {"Painter.drawRoundRect", Painter__drawRoundRect},
    {"Painter.fillRect", Painter__fillRect},
};

void qt_bindings_install(caBranch* branch)
{
    circ_install_function_list(branch, IMPORTS);
}
