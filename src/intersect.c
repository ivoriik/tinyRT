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

int			solve_qvadratic(double a, double b, double c, double *t)
{
	double q;
	double dis;
	double t0;
	double t1;

	dis = b * b - 4 * c * a;
	if (dis < 0)
		return (0);
	if (dis == 0)
	{
		*t = -0.5 * b / a;
		return (1);
	}
	q = (b >= 0) ? -0.5 * (b + sqrt(dis)) : -0.5 * (b - sqrt(dis));
	t0 = q / a;
	t1 = c / q;
	if (t0 > t1 || t0 < T_COEF)
		ft_swap(&t0, &t1, sizeof(t0));
	*t = t0;
	return (1);
}

int			s_intersect(t_vector dir, t_vector orig, t_obj obj, double *t)
{
	double		a;
	double		b;
	double		c;
	t_vector	pos;

	pos = orig - obj.tr_pos;
	a = dot_product(dir, dir);
	b = 2.0f * dot_product(dir, pos);
	c = dot_product(pos, pos) - (double)(obj.tr_siz * obj.tr_siz);
	if (!solve_qvadratic(a, b, c, t) || *t < T_COEF)
		return (0);
	return (1);
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
	return (*t >= 0.01f);
}

int			cy_intersect(t_vector dir, t_vector orig, t_obj obj, double *t)
{
	double		a;
	double		b;
	double		c;
	t_vector	x;
	t_vector	v;

	v = obj.tr_rot;
	x = orig - obj.tr_pos;
	a = dot_product(dir, dir) - dot_product(dir, v) * dot_product(dir, v);
	b = 2.0f * (dot_product(x, dir) - dot_product(dir, v) * dot_product(x, v));
	c = dot_product(x, x) - dot_product(x, v) * dot_product(x, v) - \
	obj.tr_siz * obj.tr_siz;
	if (!solve_qvadratic(a, b, c, t) || *t < T_COEF)
		return (0);
	return (1);
}

int			co_intersect(t_vector dir, t_vector orig, t_obj obj, double *t)
{
	double		a;
	double		b;
	double		c;
	t_vector	x;
	t_vector	v;

	v = obj.tr_rot;
	x = orig - obj.tr_pos;
	a = dot_product(dir, dir) - (1.0f + obj.tg2) * dot_product(dir, v) \
	* dot_product(dir, v);
	b = 2.0f * (dot_product(dir, x) - (1.0f + obj.tg2) * dot_product(dir, v) \
		* dot_product(x, v));
	c = dot_product(x, x) - (1.0f + obj.tg2) * dot_product(x, v) * \
	dot_product(x, v);
	if (!solve_qvadratic(a, b, c, t) || *t < T_COEF)
		return (0);
	return (1);
}
