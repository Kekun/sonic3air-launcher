/* sonic3-sound.c
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
#include "sonic3-sound.h"

#include <gst/gst.h>

void
sonic3_sound_play_error (void)
{

  GstElement *pipeline, *source, *decoder, *conv, *sink;

    /* Create gstreamer elements */
  pipeline = gst_pipeline_new ("audio-player");
  source = gst_element_factory_make ("giosrc", "file-source");
  decoder = gst_element_factory_make ("wavparse", "wave-decoder");
  conv = gst_element_factory_make ("audioconvert", "converter");
  sink = gst_element_factory_make ("autoaudiosink", "audio-output");

  if (!pipeline || !source || !decoder || !conv || !sink) {
    g_printerr ("One element could not be created. Exiting.\n");
    return;
  }

  /* g_object_set (G_OBJECT (source), "location", "/app/bin/error.wav", NULL); */
  g_object_set (G_OBJECT (source), "location", "resource:///org/sonic3air/Sonic3AIR/sounds/error.wav", NULL);

  gst_bin_add_many (GST_BIN (pipeline),
                    source, decoder, conv, sink, NULL);

  gst_element_link_many (source, decoder, conv, sink, NULL);

  g_print ("Now playing: error.wav\n");
  gst_element_set_state (pipeline, GST_STATE_PLAYING);

}
