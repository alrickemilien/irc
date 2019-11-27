#include <client/ui/ui.h>
#include <stdlib.h>

GdkRGBA *gtk_new_rgba(float r, float g, float b, float a)
{
    GdkRGBA *c;

    c = (GdkRGBA *)malloc(sizeof(GdkRGBA));

    if (c == NULL)
        return (NULL);

    c->red = r;
    c->green = g;
    c->blue = b;
    c->alpha = a;

    return (c);
}
