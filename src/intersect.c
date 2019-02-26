/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbespalk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/11 14:45:17 by vbespalk          #+#    #+#             */
/*   Updated: 2018/09/11 14:45:29 by vbespalk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int			solve_qvadratic(double a, double b, double c, double t[2])
{
	double q;
	double dis;
	double t0;
	double t1;

	t[0] = FLT_MIN;
	t[1] = FLT_MIN;
	dis = b * b - 4 * c * a;
	if (dis < 0)
		return (0);
	if (dis == 0)
	{
		t[0] = -0.5 * b / a;
		return (1);
	}
	q = (b >= 0) ? -0.5 * (b + sqrt(dis)) : -0.5 * (b - sqrt(dis));
	t0 = q / a;
	t1 = c / q;
	if (t0 > t1 || t0 < T_COEF)
		ft_swap(&t0, &t1, sizeof(t0));
	t[0] = t0;
	t[1] = t1;
	return (1);
}

int			s_intersect(t_vector dir, t_vector orig, t_obj obj, double *t)
{
	double		a;
	double		b;
	double		c;
	t_vector	pos;
	double		t1t2[2];
	t_vector	hit;
	double		phi;

	pos = orig - obj.tr_pos;
	a = dot_product(dir, dir);
	b = 2.0f * dot_product(dir, pos);
	c = dot_product(pos, pos) - (double)(obj.tr_siz * obj.tr_siz);
	if (!solve_qvadratic(a, b, c, t1t2))
		return (0);
	if (t1t2[0] > T_COEF)
	{
		hit = orig + dir * (t_vector){t1t2[0], t1t2[0], t1t2[0]} - obj.tr_pos;
		phi = atan2(hit[0], hit[2]);
		if (phi < 0.0)
			phi += 2.0f * M_PI;
		if ((phi >= obj.min_phi && phi <= obj.max_phi) &&
			hit[1] <= obj.min_thcos * obj.tr_siz &&
			hit[1] >= obj.max_thcos * obj.tr_siz)
			return (*t = t1t2[0]);
	}
	if (t1t2[1] > T_COEF)
	{
		hit = orig + dir * (t_vector){t1t2[1], t1t2[1], t1t2[1]}  - obj.tr_pos;
		phi = atan2(hit[0], hit[2]);
		if (phi < 0.0)
			phi += 2.0f * M_PI;
		if ((phi >= obj.min_phi && phi <= obj.max_phi) &&
			hit[1] <= obj.min_thcos * obj.tr_siz &&
			hit[1] >= obj.max_thcos * obj.tr_siz)
			return (*t = t1t2[1]);
	}
	return (0);
}

int			p_intersect(t_vector dir, t_vector orig, t_obj obj, double *t)
{
	double		denom;
	t_vector	vec;

	vec = obj.tr_pos - orig;
	if ((denom = dot_product(obj.tr_rot, dir)) < 0.0000001)
		return (0);
	*t = (dot_product(obj.tr_pos, obj.tr_rot) - dot_product(orig, obj.tr_rot)) \
	/ denom;
	t_vector hit = orig + dir * (t_vector){*t, *t, *t} - obj.tr_pos;
	// if (!IN_RANGE(dot_product(hit, hit), 13 * 13, 16 * 16))
	// 	return (0);
	return (*t >= 0.01f);
}

int			cy_intersect(t_vector dir, t_vector orig, t_obj obj, double *t)
{
	double		a;
	double		b;
	double		c;
	t_vector	x;
	t_vector	v;
	double		t1t2[2];
	t_vector	hit;
	double		phi;
	double		m;

	v = obj.tr_rot;
	x = orig - obj.tr_pos;
	a = dot_product(dir, dir) - dot_product(dir, v) * dot_product(dir, v);
	b = 2.0f * (dot_product(x, dir) - dot_product(dir, v) * dot_product(x, v));
	c = dot_product(x, x) - dot_product(x, v) * dot_product(x, v) - \
	obj.tr_siz * obj.tr_siz;
	if (!solve_qvadratic(a, b, c, t1t2))
		return (0);
	if (t1t2[0] > T_COEF)
	{
		hit = orig + dir * (t_vector){t1t2[0], t1t2[0], t1t2[0]} - obj.tr_pos;
		phi = atan2(hit[0], hit[2]);
		if (phi < 0.0)
			phi += 2.0f * M_PI;
			// IN_RANGE(dot_product(hit - orig, obj.tr_rot), 0, 50))
		if ((phi >= obj.min_phi && phi <= obj.max_phi) && \
			IN_RANGE(dot_product(obj.tr_rot, hit), 0, 100))
			{
			// printf("projection %f\n", dot_product(obj.tr_rot, hit));
			return (*t = t1t2[0]);
			}
	}
	if (t1t2[1] > T_COEF)
	{
		hit = orig + dir * (t_vector){t1t2[1], t1t2[1], t1t2[1]}  - obj.tr_pos;
		phi = atan2(hit[0], hit[2]);
		if (phi < 0.0)
			phi += 2.0f * M_PI;
			// IN_RANGE(dot_product(hit - orig, obj.tr_rot), 0, 50))
		if ((phi >= obj.min_phi && phi <= obj.max_phi) && \
			IN_RANGE(dot_product(obj.tr_rot, hit), 0, 100))
			{
			// printf("projection %f\n", dot_product(obj.tr_rot, hit));
			return (*t = t1t2[1]);
			}
	}
	return (0);
}

int			co_intersect(t_vector dir, t_vector orig, t_obj obj, double *t)
{
	double		a;
	double		b;
	double		c;
	t_vector	x;
	t_vector	v;
	double		t1t2[2];

	v = obj.tr_rot;
	x = orig - obj.tr_pos;
	a = dot_product(dir, dir) - (1.0f + obj.tg2) * dot_product(dir, v) \
	* dot_product(dir, v);
	b = 2.0f * (dot_product(dir, x) - (1.0f + obj.tg2) * dot_product(dir, v) \
		* dot_product(x, v));
	c = dot_product(x, x) - (1.0f + obj.tg2) * dot_product(x, v) * \
	dot_product(x, v);
	if (!solve_qvadratic(a, b, c, t1t2) || t1t2[0] < T_COEF)
		return (0);
	*t = t1t2[0];
	return (1);
}
