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

#ifndef _INVADER_H_
#define _INVADER_H_

#include <clutter/clutter.h>

G_BEGIN_DECLS

#define TYPE_INVADER                (invader_get_type ())
#define INVADER(obj)                (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_INVADER, Invader))
#define IS_INVADER(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_INVADER))
#define INVADER_CLASS(klass)        (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_INVADER, InvaderClass))
#define IS_INVADER_CLASS(klass)     (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_INVADER))
#define INVADER_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_INVADER, InvaderClass))

typedef struct _InvaderPrivate InvaderPrivate;

typedef struct {
  ClutterTexture parent;
  InvaderPrivate *priv;
} Invader;

typedef struct {
  ClutterTextureClass parent_class;
} InvaderClass;

GType         invader_get_type (void);

ClutterActor *invader_new (ClutterActor *stage, const gchar *filename);

void create_invader_army (ClutterActor *stage);

#endif
