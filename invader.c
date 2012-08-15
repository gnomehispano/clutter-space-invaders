/*
 * Copyright (C) 2012 Rodrigo Moya
 *
 * Authors: Rodrigo Moya <rodrigo@gnome.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "invader.h"

G_DEFINE_TYPE (Invader, invader, CLUTTER_TYPE_TEXTURE)

static GRand *rand = NULL;

struct _InvaderPrivate {
  ClutterActor *stage;
  ClutterTimeline *timeline;
};

static void
invader_finalize (GObject *object)
{
  Invader *invader = INVADER (object);

  if (invader->priv != NULL)
    {
      if (invader->priv->stage)
	{
	  g_object_unref (invader->priv->stage);
	  invader->priv->stage = NULL;
	}
    }

  G_OBJECT_CLASS (invader_parent_class)->finalize (object);
}

static void
invader_class_init (InvaderClass *klass)
{
}

static void
on_timeline_new_frame (ClutterTimeline *timeline, gint msecs, gpointer user_data)
{
  gfloat next_y;
  Invader *invader = INVADER (user_data);

  next_y = clutter_actor_get_y (CLUTTER_ACTOR (invader)) + 3.0;
  if (next_y >= clutter_actor_get_height (invader->priv->stage))
    {
      clutter_timeline_stop (invader->priv->timeline);
      clutter_actor_remove_child (invader->priv->stage, CLUTTER_ACTOR (invader));
      return;
    }

  clutter_actor_move_by (CLUTTER_ACTOR (invader),
			 g_rand_int_range (rand, 0, 100) > 50 ? 3.0 : -3.0,
			 3.0);
}

static void
invader_init (Invader *invader)
{
  invader->priv = g_new0 (InvaderPrivate, 1);

  if (!rand)
    rand = g_rand_new_with_seed (G_MAXINT);
  invader->priv->timeline = clutter_timeline_new (10000);
  g_signal_connect (invader->priv->timeline, "new-frame", G_CALLBACK (on_timeline_new_frame), invader);
  clutter_timeline_set_loop (invader->priv->timeline, TRUE);
}

ClutterActor *
invader_new (ClutterActor *stage, const gchar *filename)
{
  gfloat width, height;
  GError *error;
  Invader *invader = INVADER (g_object_new (TYPE_INVADER, NULL));

  invader->priv->stage = g_object_ref (stage);

  if (!clutter_texture_set_from_file (CLUTTER_TEXTURE (invader), filename, NULL))
    {
      g_warning ("Could not load %s file: %s\n", filename, error->message);
      g_error_free (error);
      g_object_unref (invader);

      return NULL;
    }

  /* Position the invader */
  clutter_actor_get_size (invader->priv->stage, &width, &height);
  clutter_actor_set_position (CLUTTER_ACTOR (invader),
			      g_rand_double_range (rand, 0.0, width),
			      0);

  clutter_actor_add_child (invader->priv->stage, CLUTTER_ACTOR (invader));
  clutter_actor_show (CLUTTER_ACTOR (invader));

  clutter_timeline_start (invader->priv->timeline);
  
  return CLUTTER_ACTOR (invader);
}

static gboolean
add_invader_timeout (gpointer user_data)
{
  gint cnt = 0;
  ClutterActor *stage = CLUTTER_ACTOR (user_data), *invader;

  cnt++;
  if (cnt % 5 == 0)
    {
      /* Every 5 times is a bigger ennemy */
      if (cnt % 15 == 0)
	{
	  /* ...and every 15, a bigger one */
	  invader = invader_new (stage, "ennemy-big.png");
	}
      else
	invader = invader_new (stage, "ennemy-medium.png");
    }
  else
    invader = invader_new (stage, "ennemy-small.png");

  return TRUE;
}

void
create_invader_army (ClutterActor *stage)
{
  g_timeout_add (1000, (GSourceFunc) add_invader_timeout, stage);
}
