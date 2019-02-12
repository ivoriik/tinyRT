/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbespalk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 16:03:58 by vbespalk          #+#    #+#             */
/*   Updated: 2018/09/04 16:04:16 by vbespalk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

// t_vector		get_spec(t_light *light, t_ray *ray, t_obj *obj, t_vector l_dir)
// {
// 	t_vector	r_dir;
// 	t_vector	col;
// 	double		intencity;
// 	double		rdotv;
// 	double		ks;

// 	ks = 0.004f * L_X(obj->reflect / 5, (unsigned int)100);
// 	col = (t_vector){0.0f, 0.0f, 0.0f};
// 	r_dir = vec_scalar_mult(ray->hit_n, 2.0f * \
// 		dot_product(ray->hit_n, l_dir)) - l_dir;
// 	normilize_vec(&r_dir);
// 	rdotv = dot_product(r_dir, vec_scalar_mult(ray->dir, -1.0f));
// 	if (rdotv < 0.0f)
// 		return (col);
// 	intencity = light->rad * pow(rdotv, \
// 		(L_X(obj->reflect, (unsigned int)1024)));
// 	col[0] = light->col[0] / 255.0f * intencity * ks;
// 	col[1] = light->col[1] / 255.0f * intencity * ks;
// 	col[2] = light->col[2] / 255.0f * intencity * ks;
// 	return (col);
// }

t_vector		get_diff(t_light *light, t_ray *ray, t_vector l_dir)
{
	double		intencity;
	t_vector	col;
	double		rad;

	col = (t_vector){0.0f, 0.0f, 0.0f};
	rad = light->rad > 100.0f ? 1.0f : light->rad / 100.0f;
	if ((intencity = rad * dot_product(l_dir, ray->hit_n)) < 0)
		intencity = 0;
	col[0] = light->col[0] / 255.0f * intencity;
	col[1] = light->col[1] / 255.0f * intencity;
	col[2] = light->col[2] / 255.0f * intencity;
	return (col);
}

int				trace_shad(t_vector l_dir, t_ray *ray, t_obj *objs, double dis)
{
	double		t;
	t_obj		*ptr;
	t_vector	vec;

	vec = ray->hit_p + vec_scalar_mult(ray->hit_n, 0.0001f);
	ptr = objs;
	while ((ptr))
	{
		if (ptr->intersect(l_dir, vec, *ptr, &t) && (t * t) < dis)
			return (0);
		ptr = ptr->next;
	}
	return (1);
}

t_vector		get_normal(t_ray *ray, t_obj *obj)
{
	t_vector	hit_n;
	double		m;

	if (obj->type == 0)
		hit_n = vec_scalar_mult((obj->tr_rot), -1.0f);
	if (obj->type == 1)
		hit_n = ray->hit_p - obj->tr_pos;
	if (obj->type == 2)
	{
		m = dot_product(vec_scalar_mult(obj->tr_rot, ray->t), ray->dir) \
		+ dot_product((ray->ori - obj->tr_pos), obj->tr_rot);
		hit_n = ray->hit_p - obj->tr_pos - vec_scalar_mult(obj->tr_rot, m);
		normilize_vec(&(hit_n));
	}
	if (obj->type == 3)
	{
		m = dot_product(vec_scalar_mult(obj->rot, ray->t), ray->dir) \
		+ dot_product((ray->ori - obj->tr_pos), obj->rot);
		hit_n = ray->hit_p - obj->tr_pos - vec_scalar_mult(obj->tr_rot, m);
		hit_n -= vec_scalar_mult(obj->rot, m * obj->tg2);
	}
	normilize_vec(&(hit_n));
	return (hit_n);
}

t_vector		get_light(t_env *env, t_ray *ray, t_obj *obj, t_light *p_light)
{
	t_vector	li;
	// t_vector	spec;
	t_vector	l_dir;
	double		r2;
	int			in_sh;

	li = (t_vector){0.0f, 0.0f, 0.0f};
	// spec = (t_vector){0.0f, 0.0f, 0.0f};
	while (p_light)
	{
		l_dir = p_light->pos - ray->hit_p;
		r2 = dot_product(l_dir, l_dir);
		normilize_vec(&l_dir);
		// if (obj->reflect)
			// spec = get_spec(p_light, ray, obj, l_dir);
		in_sh = trace_shad(vec_scalar_mult(l_dir, 1.0), ray, env->obj, r2);
		li += vec_scalar_mult(get_diff(p_light, ray, l_dir), in_sh); // + vec_scalar_mult(spec, in_sh);
		p_light = p_light->next;
	}
	float pattern = 1.0f;
	if (obj->type == 1)
	{
		float	tex[2];

		tex[0] = (1 + atan2(ray->hit_n[2], ray->hit_n[0]) / M_PI) * 0.5;
		tex[1] = acosf(ray->hit_n[1]) / M_PI;
		pattern = L_N((fmodf(tex[0] * 10, 1) > 0.5) ^ (fmodf(tex[1] * 10, 1) > 0.5), 0.5);
	}
	li = (t_vector){li[0] * pattern + AMBILI, li[1] * pattern + AMBILI, li[2] * pattern + AMBILI};
	return (li);
}
