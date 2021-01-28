/* sonic3-window.c
 *
 * Copyright 2021 Adrien Plazas
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name(s) of the above copyright
 * holders shall not be used in advertising or otherwise to promote the sale,
 * use or other dealings in this Software without prior written
 * authorization.
 */

#include "sonic3-config.h"
#include "sonic3-window.h"

#include "sonic3-rom.h"
#include "sonic3-sound.h"

#define STEAM_URL "https://store.steampowered.com/app/71162/Sonic_3__Knuckles/"

struct _Sonic3Window
{
  AdwApplicationWindow  parent_instance;
};

G_DEFINE_TYPE (Sonic3Window, sonic3_window, ADW_TYPE_APPLICATION_WINDOW)

static void
steam_button_clicked_cb (Sonic3Window *self)
{
  gtk_show_uri (GTK_WINDOW (self), STEAM_URL, GDK_CURRENT_TIME);
}

static void
locate_response_cb (Sonic3Window    *self,
                    int              response,
                    GtkNativeDialog *native)
{
  if (response == GTK_RESPONSE_ACCEPT) {
    GtkFileChooser *chooser = GTK_FILE_CHOOSER (native);
    g_autoptr(GFile) src = gtk_file_chooser_get_file (chooser);
    g_autoptr(GFile) dest = sonic3_get_dest_rom ();

    if (sonic3_try_install_rom (src, dest))
      g_object_unref (G_OBJECT (self));
    else
      sonic3_sound_play_error ();
  }

  g_object_unref (native);
}

static void
locate_button_clicked_cb (Sonic3Window *self)
{
  GtkFileChooserNative *native =
    gtk_file_chooser_native_new ("Open File",
                                 GTK_WINDOW (self),
                                 GTK_FILE_CHOOSER_ACTION_OPEN,
                                 "_Open",
                                 "_Cancel");

  g_signal_connect_swapped (native, "response", G_CALLBACK (locate_response_cb), self);
  gtk_native_dialog_show (GTK_NATIVE_DIALOG (native));
}

static void
sonic3_window_class_init (Sonic3WindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/sonic3air/Sonic3AIR/sonic3-window.ui");
  gtk_widget_class_bind_template_callback (widget_class, steam_button_clicked_cb);
  gtk_widget_class_bind_template_callback (widget_class, locate_button_clicked_cb);
}

static void
sonic3_window_init (Sonic3Window *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}
