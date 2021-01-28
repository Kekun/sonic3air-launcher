/* sonic3-rom.c
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
#include "sonic3-rom.h"

#include <glib/gstdio.h>

#define DEST_ROM_PATH "/Sonic3AIR/Sonic_Knuckles_wSonic3.bin"

static gboolean
sonic3_get_is_rom_path_valid (const gchar *rom_path)
{
  GStatBuf stat_buf;
  g_autofree gchar *contents = NULL;
  gsize length = 0;
  g_autofree gchar *checksum = NULL;
  g_autoptr(GError) error = NULL;

  return
    g_file_test (rom_path, G_FILE_TEST_EXISTS) &&
    !g_stat (rom_path, &stat_buf) &&
    stat_buf.st_size == 4194304 &&
    g_file_get_contents (rom_path, &contents, &length, &error) &&
    (checksum = g_compute_checksum_for_data (G_CHECKSUM_SHA256, (const guchar *) contents, length)) &&
    g_strcmp0 (checksum, "fa52ac946dfd576538d00aa858b790b9d81a1217e25aa5193693a4e57f4f89d9") == 0;
}

gboolean
sonic3_get_is_rom_valid (GFile *file)
{
  return sonic3_get_is_rom_path_valid (g_file_peek_path (file));
}

GFile *
sonic3_get_dest_rom (void)
{
  g_autofree gchar *path = g_strconcat (g_get_user_data_dir (), DEST_ROM_PATH, NULL);

  return g_file_new_for_path (path);
}

GFile *
sonic3_get_steam_rom (void)
{
  static const gchar *rom_paths[] = {
    "/.local/share/Steam/steamapps/common/Sega Classics/uncompressed ROMs/Sonic_Knuckles_wSonic3.bin",
    "/.steam/steam/steamapps/common/Sega Classics/uncompressed ROMs/Sonic_Knuckles_wSonic3.bin",
    "/.steam/root/steamapps/common/Sega Classics/uncompressed ROMs/Sonic_Knuckles_wSonic3.bin",
    "/.steam/debian-installation/steamapps/common/Sega Classics/uncompressed ROMs/Sonic_Knuckles_wSonic3.bin",
    "/Steam/steamapps/common/Sega Classics/uncompressed ROMs/Sonic_Knuckles_wSonic3.bin",
    "/.var/app/com.valvesoftware.Steam/.local/share/Steam/steamapps/common/Sega Classics/uncompressed ROMs/Sonic_Knuckles_wSonic3.bin",
    NULL
  };

  for (gint i = 0; rom_paths[i]; i++) {
    g_autofree gchar *rom_path = g_strconcat (g_get_home_dir (), rom_paths[i], NULL);

    if (sonic3_get_is_rom_path_valid (rom_path))
      return g_file_new_for_path (rom_path);
  }

  return NULL;
}

gboolean
sonic3_try_install_rom (GFile *src,
                        GFile *dest)
{
  g_autoptr(GFile) dest_dir = g_file_get_parent (dest);
  g_autoptr(GError) error = NULL;

  if (!sonic3_get_is_rom_valid (src))
    return FALSE;

  if (!g_file_query_exists (dest_dir, NULL))
      g_file_make_directory_with_parents (dest_dir, NULL, &error);
  if (G_UNLIKELY (error)) {
    g_critical ("%s", error->message);
    return FALSE;
  }

  g_file_copy (src, dest, G_FILE_COPY_OVERWRITE, NULL, NULL, NULL, &error);
  if (G_UNLIKELY (error)) {
    g_critical ("%s", error->message);
    return FALSE;
  }

  return TRUE;
}
