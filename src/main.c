/* main.c
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

#include <glib/gi18n.h>

#include "sonic3-config.h"
#include "sonic3-window.h"

#include <gst/gst.h>
#include "sonic3-rom.h"
#include "sonic3-sound.h"

static void
on_startup (GtkApplication *app)
{
  g_autoptr(GtkCssProvider) css_provider = gtk_css_provider_new ();
  GtkSettings *settings = gtk_settings_get_default ();

  g_object_set (settings, "gtk-application-prefer-dark-theme", TRUE, NULL);

  adw_init ();

  gtk_css_provider_load_from_resource (css_provider, "/org/sonic3air/Sonic3AIR/style.css");
  gtk_style_context_add_provider_for_display (gdk_display_get_default (),
                                              GTK_STYLE_PROVIDER (css_provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

static void
on_activate (GtkApplication *app)
{
  GtkWindow *window;

  /* It's good practice to check your parameters at the beginning of the
   * function. It helps catch errors early and in development instead of
   * by your users.
   */
  g_assert (GTK_IS_APPLICATION (app));

  /* Get the current window or create one if necessary. */
  window = gtk_application_get_active_window (app);
  if (window == NULL)
    window = g_object_new (SONIC3_TYPE_WINDOW,
                           "application", app,
                           NULL);

  /* Ask the window manager/compositor to present the window. */
  gtk_window_present (window);
}

int
main (int   argc,
      char *argv[])
{
  g_autoptr(GtkApplication) app = NULL;
  g_autoptr(GFile) dest = NULL;
  g_autoptr(GFile) steam = NULL;
  gint result;

  /* Set up gettext translations */
  bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);

  dest = sonic3_get_dest_rom ();
  if (sonic3_get_is_rom_valid (dest))
    return 0;

  steam = sonic3_get_steam_rom ();
  if (steam && sonic3_try_install_rom (steam, dest))
    return 0;

  gst_init (&argc, &argv);

  app = gtk_application_new ("org.sonic3air.Sonic3AIR", G_APPLICATION_FLAGS_NONE);

  g_signal_connect (app, "startup", G_CALLBACK (on_startup), NULL);
  g_signal_connect (app, "activate", G_CALLBACK (on_activate), NULL);

  result = g_application_run (G_APPLICATION (app), argc, argv);
  if (result)
    return result;

  return !sonic3_get_is_rom_valid (dest);
}
