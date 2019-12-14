/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gtk_pango_find_base_dir.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 12:32:13 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 12:32:14 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/ui/panel.h>
#include <fribidi.h>

static PangoDirection	gtk_pango_unichar_direction(gunichar ch)
{
    FriBidiCharType	fribidi_ch_type;

    G_STATIC_ASSERT(sizeof(FriBidiChar) == sizeof(gunichar));
    fribidi_ch_type = fribidi_get_bidi_type(ch);
    if (!FRIBIDI_IS_STRONG(fribidi_ch_type))
        return PANGO_DIRECTION_NEUTRAL;
    else if (FRIBIDI_IS_RTL(fribidi_ch_type))
        return (PANGO_DIRECTION_RTL);
    else
        return (PANGO_DIRECTION_LTR);
}

PangoDirection			gtk_pango_find_base_dir(
	const gchar *text, gint length)
{
    PangoDirection	dir;
    const gchar		*p;

    dir = PANGO_DIRECTION_NEUTRAL;
    g_return_val_if_fail(text != NULL || length == 0, PANGO_DIRECTION_NEUTRAL);
    p = text;
    while ((length < 0 || p < text + length) && *p)
    {
        gunichar wc = g_utf8_get_char(p);
        dir = gtk_pango_unichar_direction(wc);
        if (dir != PANGO_DIRECTION_NEUTRAL)
            break ;
        p = g_utf8_next_char(p);
    }
    return (dir);
}
