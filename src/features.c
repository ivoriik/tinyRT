/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   features.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbespalk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/11 14:45:17 by vbespalk          #+#    #+#             */
/*   Updated: 2019/02/11 14:45:29 by vbespalk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vector		get_refl_dir(t_vector incident, t_vector norm)
{
	t_vector	r;
	// double	dot_2;

	// dot_2 = 2.0f * dot_product(incident, norm);
	r = incident - vec_scalar_mult(norm, 2.0f \
		* dot_product(incident, norm));
	r = *(normilize_vec(&r));
	return (r);
}

t_vector		get_refr_dir(t_vector incident, t_vector norm, double ind_in, double ind_out)
{
	t_vector	r;
	double		ind;
	double		icos;
	double		rcos;

	// if (L_X(L_N(dot = dot_product(norm, incident), -1.0f), 1.0f) < 0)
	// printf("INCIDENT %f,%f,%f NORM %f,%f,%f\n", incident[0], incident[1], incident[2], norm[0], norm[1], norm[2]);
	icos = dot_product(norm, incident);
	if (icos < 0.0f)
	{
		icos = -1.0f * icos;
		ind = ind_in / ind_out;
		// printf("in\n");
	}
	else
	{
		ind = ind_out / ind_in;
		norm = (t_vector){0, 0, 0} - norm;
		// printf("out\n");
	}
	if ((rcos = 1.0f - ind * ind * (1.0f - icos * icos)) < 0)
		return ((t_vector){0, 0, 0});
	r = vec_scalar_mult(incident, ind) + vec_scalar_mult(norm, ind * icos - sqrtf(rcos));
	r = *(normilize_vec(&r));
	// printf("dot %f refr_dir %f,%f,%f\n", dot, r[0], r[1], r[2]);
	return (r);
}

double		fresnel(t_vector incident, t_vector norm, double ind_in, double ind_out)
{
	double		icos; // INCIDENT COS: DOT PROD OF INCIDENT RAY & NORMAL
	double		rcos; // SQUARE OF OUT COS
	double		kr_par; // KOEFFICIENT REFLECTION PARRALEL LIGHT WAVES
	double		kr_per; // KOEFFICIENT REFLECTION PERPENDICULAR LIGHT WAVES

	icos = dot_product(norm, incident);
	if (icos < 0.0f)
		icos = -1.0f * icos;
	else
		ft_swap(&ind_in, &ind_out, sizeof(double));
	if ((rcos = 1.0f - (ind_in * ind_in) / (ind_out * ind_out) * (1.0f - icos * icos)) < 0)
		return (1.0f); // CASE OF TOTAL REFLECTION
	else
		rcos = sqrtf(rcos);
	kr_par = (ind_out * icos - ind_in * rcos) * (ind_out * icos - ind_in *  rcos) / (ind_out * icos + ind_out * rcos) * (ind_out * icos + ind_out * rcos);
	kr_per = (ind_in * rcos - ind_out * icos) * (ind_in *  rcos - ind_out * icos) / (ind_out * icos + ind_out * rcos) * (ind_out * icos + ind_out * rcos);
	return ((kr_par + kr_per) / 2.0f);
}