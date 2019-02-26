/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbespalk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 18:45:32 by vbespalk          #+#    #+#             */
/*   Updated: 2018/08/21 18:45:50 by vbespalk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vector	spher_mapping(t_obj *obj, t_vector hit)
{
	t_color		tex_col;
	float		theta;
	float		phi;
	float		u;
	float		v;
	int			xp;
	int			yp;
	Uint32 		*ptr;

	hit -= obj->tr_pos;
	phi = atan2(hit[2], hit[0]);
	if (phi < 0.0f)
		phi += 2.0f * M_PI;
	theta = acos(hit[1] / obj->tr_siz);
	u = phi / 2.0f * M_1_PI;
	v = theta * M_1_PI;
	xp = (int)(obj->texture->w - 1) * u;
	yp = (int)(obj->texture->h - 1) * v;
	ptr = (Uint32 *)obj->texture->pixels;
	if (IN_RANGE(xp, 0, obj->texture->w) && IN_RANGE(yp, 0, obj->texture->h))
	{
		ptr += yp * obj->texture->w + xp;
		ft_memcpy(&(tex_col.col), ptr, sizeof(Uint32));
		return ((t_vector){tex_col.rgba[0], tex_col.rgba[1], tex_col.rgba[2]});
	}
	else
		return ((t_vector){255, 0, 0});
}

t_vector	rect_mapping(t_obj *obj, t_vector hit)
{
	return (obj->col);
}

t_vector	cylin_mapping(t_obj *obj, t_vector hit)
{
	t_color		tex_col;
	float		phi;
	float		u;
	float		v;
	int			xp;
	int			yp;
	Uint32 		*ptr;
	float		hei = 100.0f;

	// printf("hit %f,%f,%f pos %f,%f,%f\n", hit[0], hit[1], hit[2], obj->tr_pos[0], obj->tr_pos[1], obj->tr_pos[2]);
	hit -= obj->tr_pos;
	phi = atan2(hit[2], hit[0]);
	if (phi < 0.0f)
		phi += 2.0f * M_PI;
	u = phi / 2.0f * M_1_PI;
	v = (hit[1] * obj->tr_rot[1]) / hei;
	xp = (int)(obj->texture->w - 1) * u;
	yp = (int)(obj->texture->h - 1) * v;
	ptr = (Uint32 *)obj->texture->pixels;
	// printf("v %f hit %f, ppos %f; tex %d %d xp yp %d %d\n", v, hit[1], obj->tr_pos[1], obj->texture->w, obj->texture->h, xp, yp);
	if (IN_RANGE(xp, 0, obj->texture->w) && IN_RANGE(yp, 0, obj->texture->h))
	{
		ptr += yp * obj->texture->w + xp;
		ft_memcpy(&(tex_col.col), ptr, sizeof(Uint32));
		return ((t_vector){tex_col.rgba[0], tex_col.rgba[1], tex_col.rgba[2]});
	}
	else
		return ((t_vector){0, 255, 0});
}

t_vector	cone_mapping(t_obj *obj, t_vector hit)
{
	return (obj->col);
}